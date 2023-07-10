#ifndef VGA_H
#define	VGA_H


#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include <string.h>

#include "vga_sync.h"
#include "vga_pixel.h"
#include "vga_uart.h"

//#include "pico/time.h"
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "hardware/uart.h"
#include "hardware/dma.h"



// clocks
#define _XTAL_FREQ 125e6

#define SLEEP_MS 1000



extern char int_string[];

typedef struct {
    int hsync_counter;
    int vsync_counter;

    bool vsync_level;

    bool display_on;

    //bool hsync_blanking;

    //unsigned int hsync_blanking_cc;

} VGA;

extern VGA vga;

/*

static uint8_t framebuffer[FRAME_HEIGHT][FRAME_WIDTH + 1] = {{0}};

static bool frameactive;





const uint8_t colormap[64] = {
    0b00000000,
    0b00000001,
    0b00000010,
    0b00000011,
    0b00000100,
    0b00000101,
    0b00000110,
    0b00000111,
    0b00001000,
    0b00001001,
    0b00001010,
    0b00001011,
    0b00001100,
    0b00001101,
    0b00001110,
    0b00001111,
    0b00010000,
    0b00010001,
    0b00010010,
    0b00010011,
    0b00010100,
    0b00010101,
    0b00010110,
    0b00010111,
    0b00011000,
    0b00011001,
    0b00011010,
    0b00011011,
    0b00011100,
    0b00011101,
    0b00011110,
    0b00011111,
    0b00100000,
    0b00100001,
    0b00100010,
    0b00100011,
    0b00100100,
    0b00100101,
    0b00100110,
    0b00100111,
    0b00101000,
    0b00101001,
    0b00101010,
    0b00101011,
    0b00101100,
    0b00101101,
    0b00101110,
    0b00101111,
    0b00110000,
    0b00110001,
    0b00110010,
    0b00110011,
    0b00110100,
    0b00110101,
    0b00110110,
    0b00110111,
    0b00111000,
    0b00111001,
    0b00111010,
    0b00111011,
    0b00111100,
    0b00111101,
    0b00111110,
    0b00111111
};


static dma_channel_config dma_config;

static uint16_t slice_num_px;

static pwm_config px_config = {0, 0, 0};

static uint16_t px_duty;
/////

static uint16_t slice_num_hsync;

static uint16_t slice_num_vsync;

static pwm_config hsync_config = {0, 0, 0};

static pwm_config vsync_config = {0, 0, 0};

static uint16_t hsync_duty, vsync_duty;



*/

/*
char* itoa(int value, char* result) 
{

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';

    do 
    {
        tmp_value = value;
        value /= 10;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * 10)];
    } while ( value );

    return result;
}

char* itoa(int value, char* result, int base) 
{
    // check that the base if valid
    if (base < 2 || base > 36) { *result = '\0'; return result; }

    char* ptr = result, *ptr1 = result, tmp_char;
    int tmp_value;

    do 
    {
        tmp_value = value;
        value /= base;
        *ptr++ = "zyxwvutsrqponmlkjihgfedcba9876543210123456789abcdefghijklmnopqrstuvwxyz" [35 + (tmp_value - value * base)];
    } while ( value );

    // Apply negative sign
    if (tmp_value < 0) *ptr++ = '-';
    *ptr-- = '\0';
    while(ptr1 < ptr) 
    {
        tmp_char = *ptr;
        *ptr--= *ptr1;
        *ptr1++ = tmp_char;
    }
    return result;
}
*/

#endif