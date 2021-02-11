#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "encoders.pio.h"





#define PIO_ENCODER_PIN 1

int main() {
    stdio_init_all();
    // Set up the state machine.
    PIO pio = pio0;
    uint sm = 0;
    uint offset = pio_add_program(pio, &encoders_program);
    encoders_program_init(pio, sm, offset, PIO_ENCODER_PIN);
    while (true){
        if (pio_sm_get_rx_fifo_level(pio, sm)>0){
            uint32_t val = pio_sm_get(pio, sm);
            printf("%lu\n", val);
        }
    }
}
