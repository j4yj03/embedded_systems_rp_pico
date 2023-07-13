#include "vga.h"


VGA vga;

char int_string[10];

int COLOR_BITMASKS[3] = {COLOR_GPIO_MASK_256, COLOR_GPIO_MASK_64, COLOR_GPIO_MASK_8};

int bits[8] = {0x0, 0x07, 0x3F, 0x38, 0xF8, 0xF, 0xC0, 0xFF};
int chess[2] = {0x0, 0xFF};

unsigned int old_frame = 0;

int temp;

unsigned int hysnc_count;

int main()
{
  
    configure_gpio();

    //configure_uart();

    configure_button();

    configure_pwm_hsync();

    configure_irq();    

    //configure_dma_px();
    
    

    // startup
    // shift right by 2
    vga.color_param_1 = 2;
    // 8 bit
    vga.color_bit_mask = COLOR_BITMASKS[0];
    //
    gpio_put(PICO_DEFAULT_LED_PIN, 1);
    // start animation
    vga.animation = 5;

    
    

    // Everything after this point happens in the PWM interrupt handler
    while (true)
    {
        
            
        while(hsync_get_counter() < 550)
        {

            
            //busy_wait_us_32(1);
            tight_loop_contents();
        };
        

        
        switch (vga.animation)
        {
            default: break;

            case 0: // horizontale Streifen
                    temp = (int) (vga.line_counter / (64)); 
                    vga.color =  bits[temp % 8]; 
                    gpio_put_masked(vga.color_bit_mask, (vga.color << COLOR_GPIO_OFFSET)); 
                    break;
                
            case 1: // vertikale Streifen
                    while(hsync_get_counter() < 3800)
                    {
                        temp = (int) ((hsync_get_counter() - 500) / (4096 >> vga.color_param_1)); 
 
                        gpio_put_masked(vga.color_bit_mask, (bits[temp % 8] << COLOR_GPIO_OFFSET)); 
                    };
                    break;

            case 2: // schachbrett
                    while(hsync_get_counter() < 3800)
                    {

                        temp = (int) (vga.line_counter / (24)) ^ (hsync_get_counter() / 8);             
                        
                        gpio_put_masked(vga.color_bit_mask, (chess[temp % 2] << COLOR_GPIO_OFFSET));
                        
                    };
                    break;

            case 3: // aufsteigende Farbbalken
                    vga.color = (((vga.frame_counter ^ vga.line_counter) |~ 0x1F8) >> vga.color_param_1) & (0xFF); 
                    gpio_put_masked(vga.color_bit_mask, (vga.color << COLOR_GPIO_OFFSET)); 
                    break;

            case 4: // (ver)laufende Farben
                    vga.color = (((vga.frame_counter * ((hsync_get_counter() - 600) * 3 ^~ vga.line_counter)))  >> (vga.color_param_1 + 13)) & (0xFF);
                    gpio_put_masked(vga.color_bit_mask, (vga.color << COLOR_GPIO_OFFSET)); 
                    break;   

            

            case 5: // geordnetes Chaos
                    vga.color = (vga.frame_counter * (((hsync_get_counter() - 100 ) / 2 * vga.line_counter)) >> (vga.color_param_1 + 20)) & (0xFF);
                    gpio_put_masked(vga.color_bit_mask, (vga.color << COLOR_GPIO_OFFSET)); 
                    break;

            
            
                

        }


        
        

    };
        

}