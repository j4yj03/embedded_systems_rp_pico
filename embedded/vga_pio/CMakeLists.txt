
add_executable(vga_pio
        vga_pio.c

        )

# pull in common dependencvgaies and additional hardware support
target_link_libraries(vga_pio pico_stdlib pico_sync pico_time hardware_irq hardware_pwm hardware_gpio hardware_uart hardware_dma)

# create map/bin/hex file etc.
pico_add_extra_outputs(vga_pio)

# add url via pico_set_program_url
example_auto_set_url(vga_pio)
