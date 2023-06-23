#include "vga.h"



/** \brief configure gpio for color output
 *   
*/
inline void configure_color_gpio() {

    gpio_init_mask(COLOR_GPIO_MASK);
    gpio_set_dir_out_masked(COLOR_GPIO_MASK);
}


/** \brief configure dma channel for transfering pixel data
 *   
 * 
*/
static inline void configure_dma_px()
{
    dma_config = dma_channel_get_default_config(DMA_CHANNEL);


    // 8 bit per transfers. read address increments after each transfer
    channel_config_set_transfer_data_size(&dma_config, DMA_SIZE_8);
    
    channel_config_set_write_increment(&dma_config, false);
    channel_config_set_read_increment(&dma_config, true);

    //channel_config_set_ring(&dma_config, false, MAX_STRING_LEN);
    // set data request
    channel_config_set_dreq(&dma_config, pwm_get_dreq(slice_num_px, true));

}

/** \brief configure pwm channel for pixel clock
 *   
 *
*/
static inline void configure_pwm_px()
{
    float px_divider = _XTAL_FREQ / _PIXEL_FREQ / 4096 / 16;

    uint16_t px_wrap = _XTAL_FREQ / vsync_divider / _PIXEL_FREQ - 1;

    slice_num_px = pwm_gpio_to_slice_num(PX_CLOCK);

    px_config = {0, 0, 0};

    pwm_config_set_phase_correct(&px_config, false);
    pwm_config_set_clkdiv(&px_config, px_divider);
    pwm_config_set_clkdiv_mode(&px_config, PWM_DIV_FREE_RUNNING);
    pwm_config_set_output_polarity(&px_config, false, false);
    pwm_config_set_wrap(&px_config, px_wrap);

    pwm_init(slice_num_px, &px_config, false);

}



/** \brief function to configure read and write adress for DMA and start DMA channel
 *   
*/
void output_color(uint color) {

    uint32_t * writeaddr; 

    uint32_t * readaddr;

    uint16_t vertical_counter = pwm_get_counter(slice_num_vsync);

    readaddr = &framebuffer[vertical_counter][0];

    writeaddr = &sio_hw->gpio_out << COLOR_GPIO_OFFSET;

    // Set color output
    //sio_hw->gpio_togl = (sio_hw->gpio_out ^ (color << COLOR_GPIO_OFFSET)) & (COLOR_GPIO_MASK);

        // dma transfer trigger
    dma_channel_configure(
        DMA_CHANNEL,    // channel
        &dma_config,    // configuration
        writeaddr,      // write address
        readaddr,       // read address
        FRAME_WIDTH,    // 640 transfers
        true            // start immediately.
    );


}
