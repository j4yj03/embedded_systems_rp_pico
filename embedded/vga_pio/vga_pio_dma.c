#include "vga_pio.h"



/** \brief configure dma channel 0 for transfering pixel data to pio0 tx fifo and dma channel 1 to reconfigure channel 0
*/
void configure_dma()
{
    
    // chan 0 (send color data to pio0 tx fifo)
    dma_channel_config c0 = dma_channel_get_default_config(DMA_CHAN_DATA);
    channel_config_set_transfer_data_size(&c0, DMA_SIZE_8);
    channel_config_set_read_increment(&c0, true);                        //read incrementing
    channel_config_set_write_increment(&c0, false);
    channel_config_set_dreq(&c0, DREQ_PIO0_TX2) ;                        // DREQ_PIO0_TX2 pacing (FIFO)
    channel_config_set_chain_to(&c0, DMA_CHAN_CONF);                        // chain DMA_CHAN_CONF

    dma_channel_configure(
        DMA_CHAN_DATA,                 
        &c0,                       
        &pio_0->txf[RGB_PIO_SM],          // write address
        &vga_data_array[0],            // the initial read address
        N_PIXEL, 
        //FRAME_WIDTH,                   
        false                       
    );

    // chan 1 (reconfigure the chan 0)
    dma_channel_config c1 = dma_channel_get_default_config(DMA_CHAN_CONF);
    channel_config_set_transfer_data_size(&c1, DMA_SIZE_32);
    channel_config_set_read_increment(&c1, false);                        // no read incrementing
    channel_config_set_write_increment(&c1, false);
    //channel_config_set_ring(&c1, false, vga_data_array_offsets_size);
    channel_config_set_chain_to(&c1, DMA_CHAN_DATA);                         // chain to other channel

    dma_channel_configure(
        DMA_CHAN_CONF,
        &c1,
        &dma_hw->ch[DMA_CHAN_DATA].read_addr,
        &vga_data_array_start,
        1,
        false
    );    

    dma_start_channel_mask((1u << DMA_CHAN_DATA)) ;
#ifdef VGA_UART_DEBUG
    uart_puts(UART_ID,"\n\rHllo DMA!!");
    //uart_puts(UART_ID, itoa(hsync_offset, int_string, 10));

#endif

}