
# cmake version
cmake_minimum_required(VERSION 3.13)


add_executable(vga_pwm
        vga_pwm.c
        vga_uart.c
        vga_sync.c
        vga_gpio.c 
        )

# pull in common dependencvgaies and additional hardware support
target_link_libraries(vga_pwm pico_stdlib pico_sync pico_time hardware_irq hardware_pwm hardware_gpio hardware_uart hardware_dma)

# create map/bin/hex file etc.
pico_add_extra_outputs(vga_pwm)

# add url via pico_set_program_url
example_auto_set_url(vga_pwm)
