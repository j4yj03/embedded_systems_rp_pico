#include "vga_pio.h"




void configure_dma()
{

    address_pointer = &vga_data_array[0];

    // Channel Zero (sends color data to PIO VGA machine)
    dma_channel_config c0 = dma_channel_get_default_config(DMA_CHAN_DATA);  // default configs
    channel_config_set_transfer_data_size(&c0, DMA_SIZE_8);              // 8-bit txfers
    channel_config_set_read_increment(&c0, true);                        // yes read incrementing
    channel_config_set_write_increment(&c0, false);                      // no write incrementing
    channel_config_set_dreq(&c0, DREQ_PIO0_TX2) ;                        // DREQ_PIO0_TX2 pacing (FIFO)
    channel_config_set_chain_to(&c0, DMA_CHAN_CONF);                        // chain to other channel

    dma_channel_configure(
        DMA_CHAN_DATA,                 // Channel to be configured
        &c0,                        // The configuration we just created
        &pio->txf[RGB_PIO_SM],          // write address (RGB PIO TX FIFO)
        &vga_data_array,            // The initial read address (pixel color array)
        N_PIXEL,                    // Number of transfers; in this case each is 1 byte.
        false                       // Don't start immediately.
    );

    // Channel One (reconfigures the first channel)
    dma_channel_config c1 = dma_channel_get_default_config(DMA_CHAN_CONF);   // default configs
    channel_config_set_transfer_data_size(&c1, DMA_SIZE_32);              // 32-bit txfers
    channel_config_set_read_increment(&c1, false);                        // no read incrementing
    channel_config_set_write_increment(&c1, false);                       // no write incrementing
    channel_config_set_chain_to(&c1, DMA_CHAN_DATA);                         // chain to other channel

    dma_channel_configure(
        DMA_CHAN_CONF,                         // Channel to be configured
        &c1,                                // The configuration we just created
        &dma_hw->ch[DMA_CHAN_DATA].read_addr,  // Write address (channel 0 read address)
        &address_pointer,                   // Read address (POINTER TO AN ADDRESS)
        1,                                  // Number of transfers, in this case each is 4 byte
        false                               // Don't start immediately.
    );


    dma_start_channel_mask((1u << DMA_CHAN_DATA)) ;
}