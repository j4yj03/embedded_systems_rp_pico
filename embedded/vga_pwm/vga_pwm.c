#include "vga_pwm.h"


char int_string[10];

int COLOR_BITMASKS[3] = {COLOR_GPIO_MASK_256, COLOR_GPIO_MASK_64, COLOR_GPIO_MASK_8};

const int n_colorlines = 4;

int colorlines[4][FRAME_WIDTH] = {{0}};


int bits[8] = {0x0, 0x07, 0x3F, 0x38, 0xF8, 0xF, 0xC0, 0xFF};
int chess[2] = {0x0, 0xFF};


int line_counter, frame_counter, color, color_bit_depth_index, px_idx, colorline_idx, tmp_idx = 0;

int color_shift_bits = 10;

int color_gpoio_mask = COLOR_GPIO_MASK_256;

int color_animation = 0;

semaphore_t display_on;




/*
    4000 clocks @ 125MHz TOP (32 µs)

    3200 clocks @ 125MHz displaytime (25.6µs)
    800 clocks @ 125MHz blanking (6.4µs) 
    
*/
int FIRST_VISIBLE_COL = 600;  // 18% (3.84µs PW + 1.92µs BP)
int LAST_VISIBLE_COL = 3900; // 98% (32µs TOP - 0.640µs FP)




/** \brief generate n bins of pixeldata and store in array
 * \param width width of bins
 * 
*/
void generate_line(int n_bins)
{

    // each bin consists of FRAME_WIDTH / n_bins pixel
    int bin_size = FRAME_WIDTH / n_bins;
    int bin, start_idx, end_idx, color_idx = 0;
    

    for (bin = 0; bin < n_bins; bin++)
    {
        // calculate start idx
        start_idx = bin * bin_size;
        end_idx = (bin + 1) * bin_size;

        color_idx++; 

        for(colorline_idx = 0; colorline_idx < n_colorlines; colorline_idx++)
        {
            for(px_idx = start_idx; px_idx < end_idx; px_idx++)
            {
                colorlines[colorline_idx][px_idx] = bits[(color_idx + colorline_idx + bin) % 8];
            }
        }

        
    };
    
    uart_puts(UART_ID, "\n\rPixels generated! n bins=");
    uart_puts(UART_ID, itoa(n_bins, int_string, 10));
    uart_puts(UART_ID, " binsize=");
    uart_puts(UART_ID, itoa(bin_size, int_string, 10));
}


/** \brief main initialisation and mainloop. Video generation in mainloop depends on color_animation value
*/
int main()
{
    configure_uart();   // UART

    configure_gpio();   // GPIOs (colorpins, syncpins, buttons + button irq)

    configure_pwm_hsync(); // HSYNC pwm

    configure_pwm_irq();    // pwm interrupt handler config + start

    generate_line(12);    // calculate pixel array with 12 bins

    //configure_dma_px();
    sem_init(&display_on, 0, 1);


    //each loop has to be timed to run withing one hsync clock cycle
    while (true)
    {
        
        // wait for hsync pwm wrap
        sem_acquire_blocking(&display_on);

        // new line starts here
        
        // wait for hsync PW + BP
        while(hsync_get_counter() < FIRST_VISIBLE_COL) 
        {
            tight_loop_contents();
        };

        // generate and output pixeldata
        switch(color_animation)
        {
            default: break;

            case 0: // horizontale Streifen
                    while(hsync_get_counter() < LAST_VISIBLE_COL)
                    {
                        tmp_idx = (int) (line_counter / 64); 
                        color =  bits[tmp_idx % 8]; 
                        gpio_put_masked(color_gpoio_mask, (color << COLOR_GPIO_OFFSET));
                    };
                    break;
                
            case 1: // vertikale Streifen
                    while(hsync_get_counter() < LAST_VISIBLE_COL)
                    {
                        tmp_idx = (int) (hsync_get_counter() / 256);
                        gpio_put_masked(color_gpoio_mask, (bits[tmp_idx % 8] << COLOR_GPIO_OFFSET));

                    };
                    break;

            case 2: // berechnetes Schachbrett aus n_colorlines farbarrays

                    // each colorline is displayed 40 times
                    colorline_idx = ((int) (line_counter / 40)) % n_colorlines;
    
                    for (px_idx = 0; px_idx < FRAME_WIDTH; px_idx++)          
                    {       
                        color = colorlines[colorline_idx][px_idx];
                        gpio_put_masked(color_gpoio_mask, (color << COLOR_GPIO_OFFSET));
                    }
                    break;
            /*
            case 3: // aufsteigende horizontale Steifen
                    vga.color = (((vga.frame_counter ^ vga.line_counter) |~ 0x1F8) >> vga.color_shift_bits) & (0xFF); 
                    gpio_put_masked(vga.color_gpoio_mask, (vga.color << COLOR_GPIO_OFFSET)); 
                    break;
                    */
            case 3: // zur Laufzeit berechnetes Schachbrett
                    while(hsync_get_counter() < LAST_VISIBLE_COL)
                    {
                        tmp_idx = (int) (line_counter / (24)) ^ (hsync_get_counter() / 4);                       
                        gpio_put_masked(color_gpoio_mask, (chess[tmp_idx % 2] << COLOR_GPIO_OFFSET));
                
                    };
                    break;

                    

            case 4: // (ver)laufende Farben im diagonalmuster
                    while(hsync_get_counter() < LAST_VISIBLE_COL)
                    {
                        color = (((frame_counter * ((hsync_get_counter() / 2) ^~ line_counter)))  >> (color_shift_bits + 5)) & (0xFF);
                        gpio_put_masked(color_gpoio_mask, (color << COLOR_GPIO_OFFSET));
                    }
                    break;   

            

            case 5: // logarithmisch mosaik
                    while(hsync_get_counter() < LAST_VISIBLE_COL)
                    {
                        color = (frame_counter * (((hsync_get_counter() / 2)  * line_counter)) >> (color_shift_bits + 14)) & (0xFF);
                        gpio_put_masked(color_gpoio_mask, (color << COLOR_GPIO_OFFSET)); 
                    }
                    break;


        }

    };     

}