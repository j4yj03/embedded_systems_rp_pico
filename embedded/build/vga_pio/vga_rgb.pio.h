// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ------- //
// vga_rgb //
// ------- //

#define vga_rgb_wrap_target 2
#define vga_rgb_wrap 8

static const uint16_t vga_rgb_program_instructions[] = {
    0x80a0, //  0: pull   block                      
    0xa047, //  1: mov    y, osr                     
            //     .wrap_target
    0xa0e3, //  2: mov    osr, null                  
    0x6308, //  3: out    pins, 8                [3] 
    0xa022, //  4: mov    x, y                       
    0x23c1, //  5: wait   1 irq, 1               [3] 
    0x80a0, //  6: pull   block                      
    0x6708, //  7: out    pins, 8                [7] 
    0x0046, //  8: jmp    x--, 6                     
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program vga_rgb_program = {
    .instructions = vga_rgb_program_instructions,
    .length = 9,
    .origin = -1,
};

static inline pio_sm_config vga_rgb_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + vga_rgb_wrap_target, offset + vga_rgb_wrap);
    return c;
}

static inline void vga_rgb_program_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_config c = vga_rgb_program_get_default_config(offset);
    sm_config_set_out_pins(&c, pin, 8);
    int n_pins = 8;
    for(int pin_num = 0; pin_num < n_pins; pin_num++ )
    {
        pio_gpio_init(pio, pin + pin_num);
    }
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 8, true);
    pio_sm_init(pio, sm, offset, &c);
}

#endif

