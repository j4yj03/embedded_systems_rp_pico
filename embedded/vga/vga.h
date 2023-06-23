#ifndef VGA_H
#define	VGA_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

#define SLEEP_MS 100

//
uint8_t framebuffer[FRAME_HEIGHT][FRAME_WIDTH] = {{0}};


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
/*
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