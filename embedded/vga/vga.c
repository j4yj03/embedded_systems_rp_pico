#include "vga.h"




int main()
{
    uint16_t slice_num_px;
  
    configure_color_gpio();

    configure_uart();

    
    printf("Hello UART! \n\r")

    configure_pwm_hsync();

    configure_pwm_vsync();

    slice_num_px = configure_pwm_px();

    configure_dma_px(slice_num_px);
    
    configure_irq();


    sleep_ms(SLEEP_MS);

    // enable PWM simultaneaously
    pwm_set_mask_enabled(0b00011000);


    // Everything after this point happens in the PWM interrupt handler
    while (1)
        tight_loop_contents();

}