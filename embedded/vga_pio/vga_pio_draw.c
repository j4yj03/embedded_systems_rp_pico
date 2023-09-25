#include "vga_pio.h"


int COLOR_BITMASKS[3] = {COLOR_GPIO_MASK_256, COLOR_GPIO_MASK_64, COLOR_GPIO_MASK_8};

int colors[8] = {0x0, 0x07, 0x3F, 0x38, 0xF8, 0xF, 0xC0, 0xFF};
int chess[2] = {0x0, 0xFF};


char vga_data_array[N_PIXEL] = {0};
// initial starting adress of vga data array
char * vga_data_array_start = &vga_data_array[0];

int color_gpoio_bitmask = COLOR_GPIO_MASK_256;
int px_idx, tmp_idx = 0;
int vga_animation, color, color_bitmask_idx, color_shift_bits = 0;


/** \brief main called by the mainloop to fill the vga_data_array with pixeldata
    \param frame_counter the current frame counter value
*/
void fill_vga_data_array(int frame_counter)
{

    // which animation should be calculated.
    switch(vga_animation)
    {
        default: break;
        case 0: draw_function_0(); break;   // berechnetes s/w schachbrettmuster
        case 1: draw_function_1(); break;   // berechnetes farb schachbrettmuster mit verwzeigungen 
        case 2: draw_function_2(); break;   // horizontale streifen
        case 3: draw_function_3(frame_counter); break;  // buntes Kreuz im zentrum
        case 4: draw_function_4(frame_counter); break;  // diagonal wachsende rechtecke
        case 5: draw_function_5(frame_counter); break;  // diagonal wachsende rechtecke #2
        case 6: draw_function_6(frame_counter); break;  // chaos
        case 7: draw_function_7(frame_counter); break;  // exponentielles mosaik muster
    }

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
/** \brief berechnetes s/w schachbrettmuster
*/
void draw_function_0()
{
    int color, x, y;

    color = 0;
    px_idx = 0;

    for (y = 0; y < FRAME_HEIGHT; y++) {
       
        for (x = 0; x < FRAME_WIDTH; x++) {

            // XOR result is alternating between even/uneven 
            color = chess[((y / (24)) ^ (x / 16)) % 2];

            px_idx = ((FRAME_WIDTH * y) + x);
            drawPixel(px_idx, color) ;    //     Draw a pixel to the screen
        }
    }
}


/** \brief berechnetes farb schachbrettmuster mit verwzeigungen 
*/
void draw_function_1()
{
    int x, y, xcounter, ycounter, idx;

    idx = 0;
    px_idx = 0;
    xcounter = 0;
    ycounter = 0;

    for (y = 0; y < FRAME_HEIGHT; y++) {                     // For each y-coordinate . . .
        if (ycounter == (FRAME_HEIGHT / 16)) {                     //   If the y-counter is 60 . . .
            ycounter = 0 ;                      //     Zero the counter
            idx = (idx + 3) % 8 ;               //     Increment the color index
        }                                       //
        ycounter++ ;                          //   Increment the y-counter
        for (x = 0; x < FRAME_WIDTH; x++) {                 //   For each x-coordinate . . .
            if (xcounter == (FRAME_WIDTH / 16)) {               //     If the x-counter is 80 . . .
                xcounter = 0;                   //        Zero the x-counter
                idx = (idx + 1) % 8 ;         //        Increment the color index
            }                                   //
            xcounter++ ;                     //     Increment the x-counter
            px_idx = ((FRAME_WIDTH * y) + x);
            drawPixel(px_idx, colors[idx]) ;    //     Draw a pixel to the screen
        }
    }
}

/** \brief horizontale streifen
*/
void draw_function_2()
{
    int color, x, y;

    color = 0;
    px_idx = 0;

    for (y = 0; y < FRAME_HEIGHT; y++)
    {
        for (x = 0; x < FRAME_WIDTH; x++) 
        {
            color = colors[(y / 60) % 8];
            px_idx = ((FRAME_WIDTH * y) + x);
            drawPixel(px_idx, color) ;    //     Draw a pixel to the screen
        }
    }

}

/** \brief buntes Kreuz im zentrum
*/
void draw_function_3()
{
    int color, x, y;

    color = 0;
    px_idx = 0;

    for (y = 0; y < FRAME_HEIGHT; y++)
    {
        for (x = 0; x < FRAME_WIDTH; x++) 
        {

            if((y > 230 && y < 250) && (x > 155 && x < 165))   // weißer Mittelpunkt (top layer)
            {
                color = chess[1];
            } 
            else if((y > 80 && y < 400) && (x > 140 && x < 180)) // vertikale Streifen (middle layer)
            {
                color = colors[((x / 32) + 1) % 8];
            }
            else if ((y > 200 && y < 280) && (x > 40 && x < 280)) // horizontal Streifen (middle layer)
            {
                color = colors[((y / 32) + 3) % 8];
            } 
            else                                                  // weißer hintergrund (bottom layer)
            {
                color = chess[1];
            }

            px_idx = ((FRAME_WIDTH * y) + x);
            drawPixel(px_idx, color) ;    //     Draw a pixel to the screen
        }
    }

}

/** \brief diagonal wachsende rechtecke
    \param frame_counter the current frame counter value
*/
void draw_function_4(int frame_counter)
{
    int color, x, y;

    color = 0;
    px_idx = 0;

    for (y = 0; y < FRAME_HEIGHT; y++)
    {
        for (x = 0; x < FRAME_WIDTH; x++) 
        {
            color = (((y ^~ (x * 2)) * frame_counter) >> 16) & (0xFF);
            px_idx = ((FRAME_WIDTH * y) + x);
            drawPixel(px_idx, color) ;    //     Draw a pixel to the screen
        }
    }
}

/** \brief diagonal wachsende rechtecke #2
    \param frame_counter the current frame counter value
*/
void draw_function_5(int frame_counter)
{

    int color, x, y;

    color = 0;
    px_idx = 0;

    for (y = 0; y < FRAME_HEIGHT; y++) 
    {                     
        for (x = 0; x < FRAME_WIDTH + 0; x++) 
        { 
            color = ((((y * 0xFF) ^ (x * 0xFF)) *~ frame_counter) >> color_shift_bits) & (0xFF);
            px_idx = ((FRAME_WIDTH * (y)) + (x - 0));
            drawPixel(px_idx, color) ;    //     Draw a pixel to the screen
        }
    }
}

/** \brief chaos
    \param frame_counter the current frame counter value
*/
void draw_function_6(int frame_counter)
{

    int color, xy, value;

    color = 0;
    px_idx = 0;



    for (xy = 0; xy < FRAME_HEIGHT * FRAME_WIDTH; xy++) 
    {     
        value = (int) * &dma_hw->ch[DMA_CHAN_DATA].read_addr;

        color = ((((xy * value) / 800) * (frame_counter)) >> (color_shift_bits)) & (0xFF);

        drawPixel(xy, color) ;    //     Draw a pixel to the screen
        
    }
}

/** \brief exponentielles mosaik muster
    \param frame_counter the current frame counter value
*/
void draw_function_7(int frame_counter)
{

    int color, x, y;

    color = 0;
    px_idx = 0;

    for (y = 0; y < FRAME_HEIGHT ; y++) 
    {  
        for (x = 0; x < FRAME_WIDTH; x++) 
        {             
            color = ((y * x) >> (color_shift_bits)) & (0xFF);
            px_idx = ((FRAME_WIDTH * (y)) + (x - 0));
            drawPixel(px_idx, color) ;    //     Draw a pixel to the screen
        }
    }
}