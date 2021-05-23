# DMT Biofuel Engine Control System

This is the repository for the software used for the DMT Biofuel Engine Sensors & Control Team. The Sensors & Control team are developing a control system that will allow a motorcycle engine to run on E85 fuel using an Arduino Micro to control the fuel injection and spark timings of the engine.

This is based off camshaft and crankshaft encoders from the engine to determine the precise angle of the shafts. By modelling the engine using 1D analysis, the optimal shaft angles for charging/discharging the ignition coils or opening/closing the fuel injectors can be found. When the shaft positions reach these optimal angles, the program will open or close the appropriate circuit controlling one of the ignition coils or fuel injectors.

![Control System main loop flowchart](./main_flowchart.png)

## Pre-requisites

__For testing prerequisites, go to the readme within the relevant test folder.__

In order to run this software, you must have:

- Arduino Micro.
- Arduino IDE.
- `arduino-cli` (optional).
- GitHub Desktop (optional).

You can install the Arduino IDE from [here](https://www.arduino.cc/en/software). Make sure you have the latest versions of all libraries, especially `SoftwareSerial`. These come as part of the IDE when it is first downloaded.

You can install `arduino-cli` from [here](https://www.arduino.cc/pro/cli) or use Homebrew:

```bash
brew install arduino-cli
```

Also make sure you have installed the latest core libraries, which can be done using the following commands:

```bash
arduino-cli core install arduino:avr
arduino-cli upgrade
```

You can download and install GitHub Desktop from [here](https://desktop.github.com/).

## Installation & Board Uploading

This repository can be downloaded using:

```bash
git clone git@github.com:Loumstar/DMT-Control-System.git
```

Alternatively, you can download this repository by clicking the __code__ button on the repository homepage and selecting either __Open with GitHub Desktop__ or __Download ZIP__.

Open `bioengine.ino` on the Arduino IDE. Connect your computer to the Arduino via USB. From the __Tools__ dropdown menu, set the board type to __Arduino Micro__ and select the relevant __Port__. The port is often selected automatically.

To upload the program to the arduino, click __Upload__ on the Arduino IDE. Alternatively, you can use `arduino-cli` for more information on the process.

Within `bioengine/` use the following commands:

```bash
arduino-cli compile bioengine -v --fqbn arduino:avr:micro
arduino-cli upload bioengine -v --port [PORT_NAME] --fqbn arduino:avr:micro
```

## Usage



## Testing

This repository contains three tests.

### PCB Test

The PCB test is a test script that is run on the Arduino while it is housed in the PCB of the control system.

This test is necessary for checking all the transistors function as they should, by producing a square wave for a particular injector or coil circuit, at a particular engine speed.

For more information, go to the readme within `pcb_test/`.

### Unit test

More information will be added shortly.

### Engine Simulation (integration) test

More information will be added shortly.

## Repository structure

```text
DMT-Control-System/
    readme.md  
    bioengine/
        bioengine.ino
        src/
            control_system/
                control_system.h
                control_system.c 
            engine_map/
                engine_map.h
                engine_map.c
            messages/
                messages.h
                messages.c
    tests/
        pcb_test/
            readme.md
            pcb_test.ino
            src/
                control_system/
                    control_system.h
                    control_system.c
                messages/
                    messages.h
                    messages.c
        unit_test/
            bioengine.test.c
            Makefile
            src/
                unittest/
                    unittest.c
                    unittest.h
        engine_simulator/
            engine_simulator.ino
```

Note some of these libraries are copied over to the test folders. This is due to a quirk of Arduino when compiling, where local libraries can only be found if they are in a `src/` folder within the Arduino sketch.

## Contact

If you need any information or help, please email me at [louis.manestar18@imperial.ac.uk](mailto:louis.manestar18@imperial.ac.uk).
