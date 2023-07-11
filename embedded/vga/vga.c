#include "vga.h"


VGA vga;

char int_string[10];

unsigned int old_frame = 0;

int main()
{
  
    configure_gpio();

    configure_button();

    configure_uart();

    configure_pwm_hsync();

    //configure_dma_px();
    
    configure_irq();

    vga.color_param_1 = 3;


    //systick_hw->csr = 0x5;
    //systick_hw->rvr = 0x00FFFFFF;

    
    sleep_ms(SLEEP_MS);

    
    //systick_hw->cvr = 0;

    // Everything after this point happens in the PWM interrupt handler
    while (true)
    //while (!(systick_hw->csr & M0PLUS_SYST_CSR_COUNTFLAG_BITS))
    {

        //gpio_clr_mask(COLOR_GPIO_MASK);
        /*
        if (vga.vsync_counter > old_frame)
        {
            uart_puts(UART_ID,"frame count: ");
            uart_puts(UART_ID, itoa(vga.vsync_counter, int_string, 10));
            uart_puts(UART_ID,"\n\r");
            old_frame = vga.vsync_counter;
        }
        */

        while(!vga.display_on)
        {

            
            busy_wait_us_32(1);
            //tight_loop_contents();
        };
        
        //busy_wait_us_32(2);

        calculate_colorbits();
        
        
    };
        

}