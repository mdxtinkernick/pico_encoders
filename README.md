Code to read encoders on a pi pico using pio state machine, incrementing and decrementing a count on all four transitions based on the direction of movement andtransferring the count values to a variable on the core using dma.

Encoders are attached to pins 14+15, 16+17, 18+19, 20+21, 2+3, 4+5, 6+7, 8+9 (change valuesin encoder_pins[] at top of encoders.c to change these)

I believe the flip-flop debounce logic is enabled by default on input pins, if so it was not able to cope with the bounce on dial style switch based encoders I tried, would need hardware debouncing. Is fine with optical or magnetic ones commonly used on motors. 

Currently testing with simulated encoder signalfor multiple encoders from another micro to enable single stepping through states and sending accurate numbers of pulses to check it is working. Tested with real encoder on 1 input, simulated encoders for 4, not tested with all 8 but builds.

To build succesfully the project needs to be put in the pico-examples folder structure in the pio folder, and add it to the list of subdirectories in the CMakeList.txt file inside the pio folder so it gets built

When a change is detected jmp pin is used to use the current value in various places, and then reloads the value by running in pins at the top of the code again rather than storing and moving it around. The assumption is that the pins haven't changed state in the 17 or less clock cycles (at 120MHz) it takes to run from the in pins on line 9 through the checking code to the in pins at the top. As this only happens when the encoder has just hit a transition then it would have need to physically move round to the next transition in that time - I don't expect to use it with encoders that are spinning that fast - if they are then counting the pulses will be the least of my problems.
