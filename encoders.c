#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/dma.h"
#include "hardware/irq.h"
#include "encoders.pio.h"

#define PIO_ENCODER_PIN 14

uint dma_chan;
uint sm;
PIO pio;

void dma_handler() {
    dma_hw->ints0 = 1u << dma_chan;
    dma_channel_set_read_addr(dma_chan, &pio->rxf[sm], true);
}

int main() {
    stdio_init_all();
    // Set up the state machine.
    pio = pio0;
    sm = 0;
    dma_chan = 0;
    uint32_t capture_buf = 0;
    
    uint offset = pio_add_program(pio, &encoders_program);
    encoders_program_init(pio, sm, offset, PIO_ENCODER_PIN);
    dma_channel_config c = dma_channel_get_default_config(dma_chan);
    channel_config_set_read_increment(&c, false);
    channel_config_set_write_increment(&c, false);
    channel_config_set_dreq(&c, pio_get_dreq(pio, sm, false));
    
    dma_channel_configure(dma_chan, &c,
        &capture_buf,        // Destinatinon pointer
        &pio->rxf[sm],      // Source pointer
        0xFFFFFFFF, // Number of transfers
        true                // Start immediately
    );
    irq_set_exclusive_handler(DMA_IRQ_0, dma_handler);
    irq_set_enabled(DMA_IRQ_0, true);
    dma_channel_set_irq0_enabled(dma_chan, true);

    while (true){
        printf("%lu\n", capture_buf);
        sleep_ms(1000);
    }
}