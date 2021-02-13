#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "encoders.pio.h"

#define SM1_ENCODER_PIN 14
#define SM2_ENCODER_PIN 12

int main() {
    stdio_init_all();
    // Set up the state machine.
    PIO pio = pio0;
    uint sm1 = 0;
    uint sm2 = 1;

    uint32_t val1 = 0;
    uint32_t val2 = 1;

    uint offset = pio_add_program(pio, &encoders_program);
    encoders_program_init(pio, sm1, offset, SM1_ENCODER_PIN);
    encoders_program_init(pio, sm2, offset, SM2_ENCODER_PIN);

    while (true){
        bool change = false; 
        if (pio_sm_get_rx_fifo_level(pio, sm1)>0){
            uint32_t val1 = pio_sm_get(pio, sm1);
            change = true;
        }
        if (pio_sm_get_rx_fifo_level(pio, sm2)>0){
            uint32_t val2 = pio_sm_get(pio, sm2);
            change = true;
        }
        if (change){
            printf("%lu %lu\n", val1, val2);
            change = false;
        }
    }
}