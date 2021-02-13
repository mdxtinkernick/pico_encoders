code to read encoders on a pi pico using pio state machine

encoder is attached to pins 14 + 15

currently tested with simulated output from another micro

I believe the flip-flop debounce logic is enabled by default on input pins, if so it was not able to cope with the bounce on dial style switch based encoders I tried, would need hardware debouncing.Should be fine with optical or magnetic ones commonly used on motors. Currently testing with simulated encoder signal from another micro to enable single stepping through states and sending accurate numbers of pulses to check it is working.
