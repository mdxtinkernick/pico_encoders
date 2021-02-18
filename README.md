Code to read encoders on a pi pico using pio state machine, incrementing and decrementing a count on all four transitions based on the direction of movement andtransferring the count values to a variable on the core using dma.

The project has been amended so that it shoild not be put in the pico-examples folder any more outside of the the pico-examples folder. It should now be cloned inside the pico folder. I have been testing it on a Pi400 and the PICO_SDK_PATH is hard coded inside the CMakesList.txt file. You will need to the path in the file to reflect your setup if it is different.

Introductory video is here https://www.play.mdx.ac.uk/media/pio+encoders+on+pi+pico/1_hjqrrr1d (the camera providing the view of boards is of course a pihq on a Pi A+ https://github.com/mdxtinkernick/camera_py/blob/master/README.md)

I will add one talking through the assembler code soon.

The main branch is the original simple code, getting the hang of how to get things to work, runs one state machine and on the c side polls the fifo to see when an update is available and then reads it. After that development is on seperate branches, and most development is on the c side of the code.

multi points four state machine in one POI to the same code, and still checks whether values are in the fifos waiting to be pulled.

dma implemented dma transfers so the c code just has an array of values that update automatically. This was then extended across both PIOs to get 8 channels.

Encoders are attached to pins 0+1, 2+3, 4+5, 6+7, 8+9 10+11, 12+13 14+15, 16+17 in the 8 channel version (change values in encoder_pins[] at top of encoders.c to change these)

I believe the flip-flop debounce logic is enabled by default on input pins, if so it was not able to cope with the bounce on dial style switch based encoders I tried, would need hardware debouncing. Is fine with optical or magnetic ones commonly used on motors. 

Currently testing with simulated encoder signalfor multiple encoders from another micro to enable single stepping through states and sending accurate numbers of pulses to check it is working. Tested with real encoder on 1 input, simulated encoders for 4 and 8. Tested fine with transitions down to 2.2 uS apart so far.

I have been building the project using cmake and make commands in the terminal.

Assumption in the assembler code.

When a change is detected jmp pin is used to get the current value in various places, and then reloads the value by running in pins at the top of the code again rather than storing and moving it around. The assumption is that the pins haven't changed state in the 17 or less clock cycles (at 120MHz) it takes to run from the in pins on line 9 through the checking code to the in pins at the top. As this only happens when the encoder has just hit a transition then it would have need to physically move round to the next transition in that time - I don't expect to use it with encoders that are spinning that fast. If they are then counting the pulses will be the least of my problems.
