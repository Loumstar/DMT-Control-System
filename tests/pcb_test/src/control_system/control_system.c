#include "control_system.h"

void init_pin_modes(engine* e){
    for(size_t i = 0; i < 4; i++){
        pinMode(e->coils[i].pin, OUTPUT);
        pinMode(e->injs[i].pin, OUTPUT);
    }

    pinMode(e->thermistor.pin, INPUT);
    pinMode(e->cpg.pin, INPUT);
    pinMode(e->ipg.pin, INPUT);
}


void init_engine(engine* e){
    if(!e) return;

    e->coils[0] = COIL_1;
    e->coils[1] = COIL_2;
    e->coils[2] = COIL_3;
    e->coils[3] = COIL_4;

    e->injs[0] = INJECTOR_1;
    e->injs[1] = INJECTOR_2;
    e->injs[2] = INJECTOR_3;
    e->injs[3] = INJECTOR_4;

    e->thermistor = THERMISTOR;

    e->cpg = CAMSHAFT;
    e->ipg = CRANKSHAFT;

    init_pin_modes(e);

    e->crank = 0;

    e->speed = (float) 0;
    e->temp = get_internal_temp(e);

    e->is_running = false;

    e->_ipg_state = 0;
    e->_cpg_state = 0;

    e->_prev_ipg_state = 0;
    e->_prev_cpg_state = 0;
}

void shutdown(engine* e){
    for(size_t i = 0; i < 4; i++){
        open_circuit(&(e->injs[i]));
        open_circuit(&(e->coils[i]));
    }
}

char pin_state(pin* target){
    return (*(target->reg) >> target->num) & 1;
}

void open_circuit(pin* target){
    *(target->reg) &= ~(1 << target->num);
}

void close_circuit(pin* target){
    *(target->reg) |= 1 << target->num;
}


float get_internal_temp(engine* e){
    float pin_voltage = (float) SUPPLY * analogRead(e->thermistor.pin) / ADC_MAX;
    return TEMPERATURE((float) pin_voltage);
}

void update_engine_signals(engine* e){
    e->_prev_ipg_state = e->_ipg_state;
    e->_prev_cpg_state = e->_cpg_state;

    e->_ipg_state = pin_state(&(e->ipg));
    e->_cpg_state = pin_state(&(e->cpg));

    e->temp = get_internal_temp(e);
}

bool cpg_pulsed(engine* e){
    return e->_cpg_state && !e->_prev_cpg_state;
}

bool ipg_pulsed(engine* e){
    return e->_ipg_state && !e->_prev_ipg_state;
}

void set_crank(engine* e, int angle){
    e->crank = angle;
    e->is_running = true;
}

void increment_crank(engine* e, int angle){
    e->crank = (e->crank + angle) % 720;
}

/*
    Method to update the shaft speed of the engine, given the time
    between successive IPG pulses in microseconds. 
    
    This method currently only approximates across a single pulse. 
    However there will be another implementation where the shaft 
    speed is approximated over several pulses.
*/
void update_velocity(engine* e, unsigned long pulse_width){
    e->speed = IPG_PULSE_ANGLE / pulse_width;
}

/*
    Method to estimate the crankshaft angle between pulses using 
    linear interpolation of the last known angle, the shaft speed 
    and the time since the last known angle.

    Such that: theta(t) ~= theta_i + (w * (t - t_i))
    
    Where last_pulse is the time recorded of the last known angle in
    microseconds.

    Note that angle change cannot exceed IPG_PULSE_ANGLE, as a new 
    pulse will have been observed.
*/
float estimate_angle(engine* e, unsigned long last_pulse){
    if(!e) return -1;

    float angle_change = e->speed * (micros() - last_pulse);

    return angle_change < IPG_PULSE_ANGLE
        ? e->crank + angle_change
        : e->crank + IPG_PULSE_ANGLE;
}

/*
    Method to return the true crankshaft angle based on the number 
    of IPG pulses between successive CPG pulses.
    
    The CPG sends three pulses per cycle:
    - At the start of the cycle, when cylinder 1 is at TDC 
      before the intake stroke.
    - A reference pulse shortly after the start of the cycle.
    - At the midpoint of the cycle, when cylinder 1 reaches
      TDC before the power stroke.

    If the number of pulses found does not match any of these, 
    the function will return -1.
*/
int get_true_crank_angle(char pulses){
    switch(pulses){
        case START_OF_CYCLE_PULSES:
            return 0;
        case REFERENCE_PULSES:
            return 60;
        case MID_CYCLE_PULSES:
            return 360;
        default:
            return -1;
    }
}