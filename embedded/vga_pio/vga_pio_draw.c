#include "vga_pio.h"


int COLOR_BITMASKS[3] = {COLOR_GPIO_MASK_256, COLOR_GPIO_MASK_64, COLOR_GPIO_MASK_8};

int colors[8] = {0x0, 0x07, 0x3F, 0x38, 0xF8, 0xF, 0xC0, 0xFF};
int chess[2] = {0x0, 0xFF};


char vga_data_array[N_PIXEL + PIXEL_OFFSET] = {0};
// initial starting adress of vga data array
char * vga_data_array_start = &vga_data_array[0];
char * vga_data_array_offsets[FRAME_HEIGHT * 2];
char * vga_data_array_offsets_start = &vga_data_array_offsets[0];
int vga_data_array_offsets_size;

//int px_idx, colorline_idx, tmp_idx = 0;

int color_shift_bits = 0;
int color_gpoio_bitmask = COLOR_GPIO_MASK_256;

int vga_animation, color, color_bitmask_idx = 0;



/** \brief store addresses of first pixel of each scanline in array
*/
void load_scanline_adress()
{

    for (int i = 0; i < FRAME_HEIGHT; i = i + 2)
    {
        
        vga_data_array_offsets[i] = &vga_data_array[i * FRAME_WIDTH];
        vga_data_array_offsets[i + 1] = &vga_data_array[i * FRAME_WIDTH];

#ifdef VGA_UART_DEBUG
   // uart_puts(UART_ID,"\n\rvga scan line start adress ");
   // uart_puts(UART_ID, itoa(i, int_string, 10));
   // uart_puts(UART_ID,": ");
   // uart_puts(UART_ID, itoa(vga_data_array_offsets[i], int_string, 16));
#endif
    }

    vga_data_array_offsets_size = sizeof(vga_data_array_offsets) / 4 ;

#ifdef VGA_UART_DEBUG
    uart_puts(UART_ID,"\n\rvga offsets size: ");
    uart_puts(UART_ID, itoa(vga_data_array_offsets_size, int_string, 10));
#endif
}







//////////////////////////////////////////////////////////////////////////////////////////////////////////
////    Pixel calculation
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////

/** \brief inline function stores pixel value in vga_data buffer
 * \param px pixel position index 
 * \param color binary 8 bit color value
*/
inline void drawPixel(int px, char color) 
{
    vga_data_array[px] = (color) ;
}
//
/** \brief generated checker board
*/
void draw_function_0()
{
    int color, x, y, px_idx;

    color = 0;
    px_idx = 0;

    for (y = 0; y < FRAME_HEIGHT; y++) {
       
        for (x = 0; x < FRAME_WIDTH; x++) {

            // XOR result is alternating between even/uneven 
            color = chess[((y / (24)) ^ (x / 16)) % 2];

            px_idx = ((FRAME_WIDTH * y) + x) + PIXEL_OFFSET;
            drawPixel(px_idx, color) ;    //     Draw a pixel to the screen
        }
    }
}


/** \brief generated colorful checker board (different approach)
*/
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

/** \brief abc
*/
void draw_function_2(int frame_counter)
{

    int color, x, y, px_idx;

    color = 0;
    px_idx = 0;

    for (y = 0; y < FRAME_HEIGHT; y++)
    {
        for (x = 0; x < FRAME_WIDTH; x++) 
        {
            color = (((y ^~ (x * 2)) * frame_counter) >> 16) & (0xFF);
            px_idx = ((FRAME_WIDTH * y) + x) + PIXEL_OFFSET;
            drawPixel(px_idx, color) ;    //     Draw a pixel to the screen
        }
    }
}

/** \brief abc
*/
void draw_function_3(int frame_counter)
{

    int color, x, y, px_idx;

    color = 0;
    px_idx = 0;

    for (y = 0; y < FRAME_HEIGHT; y++) 
    {     
        for (x = 0; x < FRAME_WIDTH + 0; x++) 
        {
            color = ((((y & (0xFF)) ^ (x & (0xFF))) | frame_counter) >> (color_shift_bits)) & (0xFF);
            px_idx = ((FRAME_WIDTH * (y)) + (x - 0)) + PIXEL_OFFSET;
            drawPixel(px_idx, color) ;    //     Draw a pixel to the screen
        }
    }
}

/** \brief abc
*/
void draw_function_4(int frame_counter)
{

    int color, x, y, px_idx;

    color = 0;
    px_idx = 0;

    for (y = 0; y < FRAME_HEIGHT; y++) 
    {                     
        for (x = 0; x < FRAME_WIDTH + 0; x++) 
        { 
            color = ((((y * 0xFF) ^ (x * 0xFF)) - frame_counter) >> color_shift_bits) & (0xFF);
            px_idx = ((FRAME_WIDTH * (y)) + (x - 0)) + PIXEL_OFFSET;
            drawPixel(px_idx, color) ;    //     Draw a pixel to the screen
        }
    }
}

/** \brief diagonally expanding rectangles
*/
void draw_function_5(int frame_counter)
{

    int color, x, y, px_idx;

    color = 0;
    px_idx = 0;

    for (y = 0; y < FRAME_HEIGHT ; y++) 
    {  
        for (x = 0; x < FRAME_WIDTH; x++) 
        {             
            color = ((y * x) >> (color_shift_bits)) & (0xFF);
            px_idx = ((FRAME_WIDTH * (y)) + (x - 0)) + PIXEL_OFFSET;
            drawPixel(px_idx, color) ;    //     Draw a pixel to the screen
        }
    }
}