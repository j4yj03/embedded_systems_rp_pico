// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// --------- //
// vga_vsync //
// --------- //

#define vga_vsync_wrap_target 1
#define vga_vsync_wrap 9

static const uint16_t vga_vsync_program_instructions[] = {
    0x80a0, //  0: pull   block                      
            //     .wrap_target
    0xa027, //  1: mov    x, osr                     
    0x20c0, //  2: wait   1 irq, 0                   
    0xc001, //  3: irq    nowait 1                   
    0x0042, //  4: jmp    x--, 2                     
    0x30c0, //  5: wait   1 irq, 0        side 0     
    0x20c0, //  6: wait   1 irq, 0                   
    0xe05f, //  7: set    y, 31                      
    0x38c0, //  8: wait   1 irq, 0        side 1     
    0x0088, //  9: jmp    y--, 8                     
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program vga_vsync_program = {
    .instructions = vga_vsync_program_instructions,
    .length = 10,
    .origin = -1,
};

static inline pio_sm_config vga_vsync_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + vga_vsync_wrap_target, offset + vga_vsync_wrap);
    sm_config_set_sideset(&c, 2, true, false);
    return c;
}

static inline void vga_vsync_program_init(PIO pio, uint sm, uint offset, uint pin) {
    pio_sm_config c = vga_vsync_program_get_default_config(offset);
    sm_config_set_set_pins(&c, pin, 1);
    sm_config_set_sideset_pins(&c, pin);
    sm_config_set_clkdiv(&c, 5) ;
    pio_gpio_init(pio, pin);
    pio_sm_set_consecutive_pindirs(pio, sm, pin, 1, true);
    pio_sm_init(pio, sm, offset, &c);
}

#endif

