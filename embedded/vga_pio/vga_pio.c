#include "vga_pio.h"



char int_string[10];

int COLOR_BITMASKS[3] = {COLOR_GPIO_MASK_256, COLOR_GPIO_MASK_64, COLOR_GPIO_MASK_8};

int colors[8] = {0x0, 0x07, 0x3F, 0x38, 0xF8, 0xF, 0xC0, 0xFF};
int chess[2] = {0x0, 0xFF};


char vga_data_array[N_PIXEL + PIXEL_OFFSET] = {0};
// initial starting adress of vga data array
char * vga_data_array_start = &vga_data_array[0];

int vga_data_array_address[FRAME_HEIGHT];

//int px_idx, colorline_idx, tmp_idx = 0;

int color_shift_bits = 0;

int color_gpoio_bitmask = COLOR_GPIO_MASK_256;

int color_animation, color, color_bitmask_idx = 0;


PIO pio_0 = pio0_hw;

void load_scanline_adress()
{
    
}

void configure_pio()
{
    // Choose which PIO instance to use (there are two instances, each with 4 state machines)
    

    // Our assembled program needs to be loaded into this PIO's instruction
    // memory. This SDK function will find a location (offset) in the
    // instruction memory where there is enough space for our program. We need
    // to remember these locations!
    //
    // We only have 32 instructions to spend! If the PIO programs contain more than
    // 32 instructions, then an error message will get thrown at these lines of code.
    //
    // The program name comes from the .program part of the pio file
    // and is of the form <program name_program>
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

    vga_hsync_program_init(pio_0, HSYNC_PIO_SM, hsync_offset, HSYNC_GPIO_OFFSET);
    vga_vsync_program_init(pio_0, VSYNC_PIO_SM, vsync_offset, VSYNC_GPIO_OFFSET);
    vga_rgb_program_init(pio_0, RGB_PIO_SM, rgb_offset, COLOR_GPIO_OFFSET);

    // Initialize PIO state machine counters. This passes the information to the state machines
    // that they retrieve in the first 'pull' instructions, before the .wrap_target directive
    // in the assembly. Each uses these values to initialize some counting registers.
    pio_sm_put_blocking(pio_0, HSYNC_PIO_SM, HSYNC_ACTIVE);
    pio_sm_put_blocking(pio_0, VSYNC_PIO_SM, VSYNC_ACTIVE);
    pio_sm_put_blocking(pio_0, RGB_PIO_SM, RGB_ACTIVE);


    // Start the two pio machine IN SYNC
    // Note that the RGB state machine is running at full speed,
    // so synchronization doesn't matter for that one. But, we'll
    // start them all simultaneously anyway.
    pio_enable_sm_mask_in_sync(pio_0, ((1u << HSYNC_PIO_SM) | (1u << VSYNC_PIO_SM) | (1u << RGB_PIO_SM)));

#ifdef VGA_UART_DEBUG
    uart_puts(UART_ID,"\n\rHello SYNC!");
#endif
}


inline void drawPixel(int px, char color) {

    vga_data_array[px] = (color) ;
    
}

void draw_function_1()
{
    int x, y, xcounter, ycounter, idx, px_idx;

    idx = 0;
    px_idx = 0;
    xcounter = 0;
    ycounter = 0;

    for (y = 0; y < FRAME_HEIGHT; y++) {                     // For each y-coordinate . . .
        if (ycounter == (FRAME_HEIGHT / 8)) {                     //   If the y-counter is 60 . . .
            ycounter = 0 ;                      //     Zero the counter
            idx = (idx + 3) % 8 ;               //     Increment the color index
        }                                       //
        ycounter +=1 ;                          //   Increment the y-counter
        for (x = 0; x < FRAME_WIDTH; x++) {                 //   For each x-coordinate . . .
            if (xcounter == (FRAME_WIDTH / 16)) {               //     If the x-counter is 80 . . .
                xcounter = 0;                   //        Zero the x-counter
                idx = (idx + 1) % 8 ;         //        Increment the color index
            }                                   //
            xcounter += 1 ;                     //     Increment the x-counter

            px_idx = ((FRAME_WIDTH * y) + x) + PIXEL_OFFSET;

            drawPixel(px_idx, colors[idx]) ;    //     Draw a pixel to the screen
        }
    }
}


void draw_function_2(int frame_counter)
{

    int color, x, y, px_idx;

    color = 0;
    px_idx = 0;

    for (y = 0; y < FRAME_HEIGHT; y++) {                     // For each y-coordinate . . .
       
        for (x = 0; x < FRAME_WIDTH; x++) {                 //   For each x-coordinate . . .
            
            color = (((y ^~ (x * 2)) * frame_counter) >> 16) & (0xFF);

            px_idx = ((FRAME_WIDTH * y) + x) + PIXEL_OFFSET;

            drawPixel(px_idx, color) ;    //     Draw a pixel to the screen
        }
    }
    
}

void draw_function_3(int frame_counter)
{

    int color, x, y, px_idx;



    color = 0;
    px_idx = 0;

    for (y = 0; y < FRAME_HEIGHT; y++) {                     // For each y-coordinate . . .
       
        for (x = 0; x < FRAME_WIDTH + 0; x++) {                 //   For each x-coordinate . . .
            
            color = ((((y / x) ^~ (x / y)) ^ frame_counter) >> (color_shift_bits)) & (0xFF);

            px_idx = ((FRAME_WIDTH * (y)) + (x - 0)) + PIXEL_OFFSET;

            drawPixel(px_idx, color) ;    //     Draw a pixel to the screen
        }
    }
    
}

int main()
{
    //stdio_init_all();

    configure_uart();

    configure_gpio();

    configure_pio();

    configure_dma();


    for (int i = 0; i < FRAME_HEIGHT; i++)
    {
        
        vga_data_array_address[i] = &vga_data_array[i * FRAME_WIDTH];

#ifdef VGA_UART_DEBUG
    uart_puts(UART_ID,"\n\rvga scan line start adress ");
    uart_puts(UART_ID, itoa(i, int_string, 10));
    uart_puts(UART_ID,": ");
    uart_puts(UART_ID, itoa(vga_data_array_address[i], int_string, 16));
#endif
    }


    int frame_counter;

    frame_counter = 0;

    while (true) {
        
        draw_function_3(frame_counter);

        frame_counter++;
    };
}