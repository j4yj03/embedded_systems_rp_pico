#include "vga_pio.h"



char int_string[10];
int frame_counter;

// choose pio instance 0 with 4 channels
PIO pio_0 = pio0_hw;


/** \brief configure pio co-processor using pio instance 0 and 3 channels
*/
void configure_pio()
{

    // We only have 32 instructions to spend per instance

    // store program offsets
    uint hsync_offset = pio_add_program(pio_0, &vga_hsync_program);
    uint vsync_offset = pio_add_program(pio_0, &vga_vsync_program);
    uint rgb_offset = pio_add_program(pio_0, &vga_rgb_program);

#ifdef VGA_UART_DEBUG
    uart_puts(UART_ID,"\n\rhsync_offset: ");
    uart_puts(UART_ID, itoa(hsync_offset, int_string, 10));
    uart_puts(UART_ID," - vsync_offset: ");
    uart_puts(UART_ID, itoa(vsync_offset, int_string, 10));
    uart_puts(UART_ID," - rgb_offset: ");
    uart_puts(UART_ID, itoa(rgb_offset, int_string, 10));
#endif

    // init pio programs
    vga_hsync_program_init(pio_0, HSYNC_PIO_SM, hsync_offset, HSYNC_GPIO_OFFSET);
    vga_vsync_program_init(pio_0, VSYNC_PIO_SM, vsync_offset, VSYNC_GPIO_OFFSET);
    vga_rgb_program_init(pio_0, RGB_PIO_SM, rgb_offset, COLOR_GPIO_OFFSET);

    // init state machine counters (sync active timings). retrieved in the first 'pull' instructions
    pio_sm_put_blocking(pio_0, HSYNC_PIO_SM, HSYNC_ACTIVE);
    pio_sm_put_blocking(pio_0, VSYNC_PIO_SM, VSYNC_ACTIVE);
    pio_sm_put_blocking(pio_0, RGB_PIO_SM, RGB_ACTIVE);


    // start the two pio machine IN SYNC
    pio_enable_sm_mask_in_sync(pio_0, ((1u << HSYNC_PIO_SM) | (1u << VSYNC_PIO_SM) | (1u << RGB_PIO_SM)));

#ifdef VGA_UART_DEBUG
    uart_puts(UART_ID,"\n\rHello SYNC!");
#endif
}

/** \brief main function
*/ 
int main()
{

    configure_uart();

    configure_gpio();

    configure_pio();

    configure_dma();

    load_scanline_adress();


    frame_counter = 0;


    while (true)    // main cpu loop
    {              
        switch(vga_animation)
        {
            default: break;
            case 0: draw_function_0(); break;
            case 1: draw_function_1(); break;
            case 2: draw_function_2(frame_counter); break;
            case 3: draw_function_3(frame_counter); break;
            case 4: draw_function_4(frame_counter); break;
            case 5: draw_function_5(frame_counter); break;
        }
        
        frame_counter++;

#ifdef VGA_UART_DEBUG
    uart_puts(UART_ID,"\n\rdma read addr: ");
    uart_puts(UART_ID, itoa( (int) * &dma_hw->ch[DMA_CHAN_DATA].read_addr, int_string, 16));
    uart_puts(UART_ID," - dma conf addr: ");
    uart_puts(UART_ID, itoa( (int) * &dma_hw->ch[DMA_CHAN_CONF].read_addr, int_string, 16));
#endif
    };
}