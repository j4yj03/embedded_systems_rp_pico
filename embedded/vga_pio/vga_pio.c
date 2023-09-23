#include "vga_pio.h"



int colors[8] = {0x0, 0x07, 0x3F, 0x38, 0xF8, 0xF, 0xC0, 0xFF};
int chess[2] = {0x0, 0xFF};

char vga_data_array[N_PIXEL];


void configure_pio()
{
    // Choose which PIO instance to use (there are two instances, each with 4 state machines)
    pio = pio0_hw;

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
    uint hsync_offset = pio_add_program(pio, &vga_hsync_program);
    uint vsync_offset = pio_add_program(pio, &vga_vsync_program);
    uint rgb_offset = pio_add_program(pio, &vga_rgb_program);

    vga_hsync_program_init(pio, HSYNC_PIO_SM, hsync_offset, HSYNC_GPIO_OFFSET);
    vga_vsync_program_init(pio, VSYNC_PIO_SM, vsync_offset, VSYNC_GPIO_OFFSET);
    vga_rgb_program_init(pio, RGB_PIO_SM, rgb_offset, COLOR_GPIO_OFFSET);

    // Initialize PIO state machine counters. This passes the information to the state machines
    // that they retrieve in the first 'pull' instructions, before the .wrap_target directive
    // in the assembly. Each uses these values to initialize some counting registers.
    pio_sm_put_blocking(pio, HSYNC_PIO_SM, HSYNC_ACTIVE);
    pio_sm_put_blocking(pio, VSYNC_PIO_SM, VSYNC_ACTIVE);
    pio_sm_put_blocking(pio, RGB_PIO_SM, RGB_ACTIVE);


    // Start the two pio machine IN SYNC
    // Note that the RGB state machine is running at full speed,
    // so synchronization doesn't matter for that one. But, we'll
    // start them all simultaneously anyway.
    pio_enable_sm_mask_in_sync(pio, ((1u << HSYNC_PIO_SM) | (1u << VSYNC_PIO_SM) | (1u << RGB_PIO_SM)));
}


void drawPixel(int x, int y, char color) {
    // Range checks
    if (x > 639) x = 639 ;
    if (x < 0) x = 0 ;
    if (y < 0) y = 0 ;
    if (y > 479) y = 479 ;

    // Which pixel is it?
    int pixel = ((640 * y) + x) ;

    // Is this pixel stored in the first 3 bits
    // of the vga data array index, or the second
    // 3 bits? Check, then mask.
    if (pixel & 1) {
        vga_data_array[pixel>>1] |= (color << 3) ;
    }
    else {
        vga_data_array[pixel>>1] |= (color) ;
    }
}


int main()
{
    //stdio_init_all();

    configure_uart();

    configure_gpio();

    //configure_pio();

    configure_dma();




    while (true) {

        int x = 0 ; // VGA x coordinate
        int y = 0 ; // VGA y coordinate

        int index = 0;

        // A couple of counters
        int xcounter = 0;
        int ycounter = 0;

        for (y=0; y<480; y++) {                     // For each y-coordinate . . .
            if (ycounter==60) {                     //   If the y-counter is 60 . . .
                ycounter = 0 ;                      //     Zero the counter
                index = (index+1)%8 ;               //     Increment the color index
            }                                       //
            ycounter +=1 ;                          //   Increment the y-counter
            for (x=0; x<640; x++) {                 //   For each x-coordinate . . .
                if (xcounter == 80) {               //     If the x-counter is 80 . . .
                    xcounter = 0;                   //        Zero the x-counter
                    index = (index + 1)%8 ;         //        Increment the color index
                }                                   //
                xcounter += 1 ;                     //     Increment the x-counter
                drawPixel(x, y, colors[index]) ;    //     Draw a pixel to the screen
            }
        }
    }

}