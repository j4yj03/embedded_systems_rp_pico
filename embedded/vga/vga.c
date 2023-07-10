#include "vga.h"


VGA vga;

char int_string[10];

unsigned int old_frame = 0;

int main()
{
  
    configure_gpio();

    configure_uart();

    configure_pwm_hsync();

    //configure_dma_px();
    
    configure_irq();


    //sleep_ms(SLEEP_MS);

    
    
    // Everything after this point happens in the PWM interrupt handler
    while (1)
    {
         

        while(!vga.display_on)
        {
            
            busy_wait_us_32(1);
        }

        busy_wait_us_32(5);

        calculate_colorbits();

        gpio_clr_mask(COLOR_GPIO_MASK);

        busy_wait_us_32(1);

        if (vga.vsync_counter > old_frame)
        {
            uart_puts(UART_ID,"frame count: ");
            uart_puts(UART_ID, itoa(vga.vsync_counter, int_string, 10));
            uart_puts(UART_ID,"\n\r");
            old_frame = vga.vsync_counter;
        }
    
        
    }
        

}