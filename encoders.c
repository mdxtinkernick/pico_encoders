#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "encoders.pio.h"

#define SM1_ENCODER_PIN 14
#define SM2_ENCODER_PIN 12
#define SM3_ENCODER_PIN 10
#define SM4_ENCODER_PIN 8

int main() {
    stdio_init_all();
    // Set up the state machine.
    PIO pio = pio0;
    
    uint32_t val[] = {0, 0, 0, 0};

    uint sm1 =0;
    uint sm2 =1;
    uint sm3 =2;
    uint sm4 =3;

    uint offset = pio_add_program(pio, &encoders_program);
    encoders_program_init(pio, sm1, offset, SM1_ENCODER_PIN);
    encoders_program_init(pio, sm2, offset, SM2_ENCODER_PIN);
    encoders_program_init(pio, sm3, offset, SM3_ENCODER_PIN);
    encoders_program_init(pio, sm4, offset, SM4_ENCODER_PIN);

    while (true){
        bool change = false; 
        for (uint i = 0; i<4; i++){
        if (pio_sm_get_rx_fifo_level(pio, i)>0){
            uint32_t val1 = pio_sm_get(pio, i);
            change = true;
        }
        }
        
        if (change){
            printf("%lu %lu %lu %lu\n", val[0], val[1], val[2], val[3]);
            change = false;
        }
    }
}