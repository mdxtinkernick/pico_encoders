#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "encoders.pio.h"

uint encoder_pin[] = {0, 2, 4, 6, 8, 10, 12, 14};
uint number_of_encoders =8;

PIO pio;
PIO pio2;

void dma_handler() {
    uint i = 1;
    int interrupt_channel = 0; 
    while ((i & dma_hw->ints0) == 0) {  
        i = i << 1; 
        ++interrupt_channel; 
    } 
    dma_hw->ints0 = 1u << interrupt_channel;
    if (interrupt_channel < 4){
        dma_channel_set_read_addr(interrupt_channel, &pio->rxf[interrupt_channel], true);
    }else{
        dma_channel_set_read_addr(interrupt_channel, &pio2->rxf[interrupt_channel%4], true);
    }
}

int main() {
    stdio_init_all();
    // Set up the state machine.
    pio = pio0;
    pio2 = pio1;
    
    uint32_t capture_buf[8] = {0, 0, 0, 0, 0, 0, 0, 0};
    
    uint offset = pio_add_program(pio, &encoders_program);
    uint offset2 = pio_add_program(pio2, &encoders_program);
    
    for (int i = 0; i<number_of_encoders; i++){
        if (i<4){
            encoders_program_init(pio, i, offset, encoder_pin[i], false);
        }else{
            encoders_program_init(pio2, i%4, offset2, encoder_pin[i], false);
        }  
        dma_channel_config c = dma_channel_get_default_config(i);
        channel_config_set_read_increment(&c, false);
        channel_config_set_write_increment(&c, false);
        
        
        
        if (i<4){
            channel_config_set_dreq(&c, pio_get_dreq(pio, i, false));
            dma_channel_configure(i, &c,
                &capture_buf[i],        // Destinatinon pointer
                &pio->rxf[i],      // Source pointer
                0x10, // Number of transfers
                true                // Start immediately
            );
        }else{
            channel_config_set_dreq(&c, pio_get_dreq(pio2, i%4, false));
            dma_channel_configure(i, &c,
                &capture_buf[i],        // Destinatinon pointer
                &pio2->rxf[i%4],      // Source pointer
                0x10, // Number of transfers
                true                // Start immediately
            );
        }
        irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
        irq_set_enabled(DMA_IRQ_0, true);
        dma_channel_set_irq0_enabled(i, true);
    }

    while (true){   
        for (int i = 0; i < number_of_encoders; i++){
            printf("%lu ", capture_buf[i]);
        }
        printf("\n");
        sleep_ms(10);
    }
}