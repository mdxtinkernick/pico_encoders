#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "encoders.pio.h"

#define HARD_UART_INST uart1



#define PIO_ENCODER_PIN 3

int main() {

    // Set up the state machine.
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &encoders_program);
    encoders_program_init(pio, sm, offset, PIO_ENCODER_PIN);

}
