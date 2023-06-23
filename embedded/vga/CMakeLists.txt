add_executable(vga
        vga.c
        )

# pull in common dependencies and additional hardware support
target_link_libraries(vga pico_stdlib pico_time hardware_irq hardware_pwm hardware_gpio hardware_uart hardware_dma)

# create map/bin/hex file etc.
pico_add_extra_outputs(vga)

# add url via pico_set_program_url
example_auto_set_url(vga)
