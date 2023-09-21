#include "vga.h"


char int_string[10];

int COLOR_BITMASKS[3] = {COLOR_GPIO_MASK_256, COLOR_GPIO_MASK_64, COLOR_GPIO_MASK_8};

int color_line_1[FRAME_WIDTH] = {0};
int color_line_2[FRAME_WIDTH] = {0};


int bits[8] = {0x0, 0x07, 0x3F, 0x38, 0xF8, 0xF, 0xC0, 0xFF};
int chess[2] = {0x0, 0xFF};

//unsigned int old_frame = 0;

int temp_index, hysnc_count, line_counter, frame_counter, color, color_bit_depth_index, idx, value = 0;

int color_param_1 = 3;

int color_bit_mask = COLOR_GPIO_MASK_256;

int animation = 3;

semaphore_t display_on;

//bool hsync_high = false;



/*
    4000 clocks @ 125MHz TOP (32 µs)

    3200 clocks @ 125MHz displaytime (25.6µs)
    800 clocks @ 125MHz blanking (6.4µs) 
    
*/
int first_visible_col = 600;  // 18% (3.84µs PW + 1.92µs BP)
int last_visible_col = 3900; // 98% (32µs TOP - 0.640µs FP)



/** \brief generate n bins of pixeldata and store in array
 * \param width width of bins
 * 
*/
void generate_line(int n_bins)
{

    // each bin consists of 320 / width pixel
    int bin_size = FRAME_WIDTH / n_bins;
    int start_idx, end_idx, color_idx = 0;

    for (int bin = 0; bin < n_bins; bin++)
    {
        // calculate start idx
        start_idx = bin * bin_size;
        end_idx = (bin + 1) * bin_size;

        color_idx++; 

        for(int px_idx = start_idx; px_idx < end_idx; px_idx++)
        {
            color_line_1[px_idx] = chess[color_idx % 2]; //((((start_idx + px_idx) | end_idx) & hysnc_count)) & (0xFF);
            color_line_2[px_idx] = chess[(color_idx + 1) % 2]; //((((start_idx - px_idx) | end_idx) & hysnc_count)) & (0xFF);
        }
    };
    
    uart_puts(UART_ID, "\n\rPixels generated! n bins=");
    uart_puts(UART_ID, itoa(n_bins, int_string, 10));
    uart_puts(UART_ID, " binsize=");
    uart_puts(UART_ID, itoa(bin_size, int_string, 10));
}


int main()
{

    configure_uart();   // UART
  
    configure_gpio();   // GPIOs (colorpins, syncpins, buttons + button irq)

    configure_pwm_hsync(); // HSYNC pwm

    configure_pwm_irq();    // pwm interrupt handler config + start

    generate_line(12);    // calculate pixel array

    //configure_dma_px();
    sem_init(&display_on, 0, 1);


    //each loop has to be timed to run withing one hsync clock cycle
    while (true)
    {
        
        // wait for hsync pwm wrap
        sem_acquire_blocking(&display_on);

        // new line starts here
        
        // wait for hsync PW + BP
        while(hsync_get_counter() < first_visible_col) 
        {
            tight_loop_contents();
        };

        // generate and output pixeldata
        switch(animation)
        {
            default: break;

            case 0: // horizontale Streifen
                    while(hsync_get_counter() < last_visible_col)
                    {
                        temp_index = (int) (line_counter / (64)); 
                        color =  bits[temp_index % 8]; 
                        gpio_put_masked(color_bit_mask, (color << COLOR_GPIO_OFFSET));
                    };
                    break;
                
            case 1: // vertikale Streifen
                    while(hsync_get_counter() < last_visible_col)
                    {
                        temp_index = (int) ((hsync_get_counter()) / (4096 >> color_param_1));
                        gpio_put_masked(color_bit_mask, (bits[temp_index % 8] << COLOR_GPIO_OFFSET));

                    };
                    break;

            case 2: // schachbrett
                    while(hsync_get_counter() < last_visible_col)
                    {
                        temp_index = (int) (line_counter / (24)) ^ ((hsync_get_counter()) / 4);                       
                        gpio_put_masked(color_bit_mask, (chess[temp_index % 2] << COLOR_GPIO_OFFSET));
                
                    };
                    break;
            /*
            case 3: // aufsteigende horizontale Steifen
                    vga.color = (((vga.frame_counter ^ vga.line_counter) |~ 0x1F8) >> vga.color_param_1) & (0xFF); 
                    gpio_put_masked(vga.color_bit_mask, (vga.color << COLOR_GPIO_OFFSET)); 
                    break;
                    */
            case 3: // generiertes Schachbrett

                    value = (int) (line_counter / 40) % 2;
    
                    if (value == 0)
                    {
                        for (idx = 0; idx < FRAME_WIDTH; idx++)          
                        {       
                            color = color_line_1[idx];
                            gpio_put_masked(color_bit_mask, (color << COLOR_GPIO_OFFSET));
                        }
                        break;
                    }else
                    {
                        for (idx = 0; idx < FRAME_WIDTH; idx++)          
                        {       
                            color = color_line_2[idx];
                            gpio_put_masked(color_bit_mask, (color << COLOR_GPIO_OFFSET));
                        }
                        break;
                    }
                    

            case 4: // (ver)laufende Farben im diagonalmuster
                    while(hsync_get_counter() < last_visible_col)
                    {
                        color = (((frame_counter * ((hsync_get_counter() / 2) ^~ line_counter)))  >> (color_param_1 + 5)) & (0xFF);
                        gpio_put_masked(color_bit_mask, (color << COLOR_GPIO_OFFSET));
                    }
                    break;   

            

            case 5: // logarithmisch mosaik
                    while(hsync_get_counter() < last_visible_col)
                    {
                        color = (frame_counter * (((hsync_get_counter() / 2)  * line_counter)) >> (color_param_1 + 14)) & (0xFF);
                        gpio_put_masked(color_bit_mask, (color << COLOR_GPIO_OFFSET)); 
                    }
                    break;


        }

    };     

}