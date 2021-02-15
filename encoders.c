#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "encoders.pio.h"

int main() {
    stdio_init_all();
    // Set up the state machine.
    PIO pio = pio0;
    
    uint32_t val[] = {0, 0, 0, 0};
    uint encoderPin[] = {14, 16, 18, 20};
    uint sm[] ={0,1,2,3};
    

    uint offset = pio_add_program(pio, &encoders_program);
    for (uint i = 0; i<4; i++){
    encoders_program_init(pio, sm[i], offset, encoderPin[i]);
    }

    while (true){
        bool change = false; 
        for (uint i = 0; i<4; i++){
            if (pio_sm_get_rx_fifo_level(pio, i)>0){
                val[i] = pio_sm_get(pio, i);
                change = true;
            }
        }
        
        if (change){
            printf("%lu %lu %lu %lu\n", val[0], val[1], val[2], val[3]);
            change = false;
        }
    }
}