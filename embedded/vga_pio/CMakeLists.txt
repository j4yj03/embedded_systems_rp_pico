
# cmake version
cmake_minimum_required(VERSION 3.13)


add_executable(vga_pio
        vga_pio.c
        vga_pio_uart.c
        vga_pio_gpio.c
        vga_pio_dma.c
        )

# must match with pio filename and executable name from above
pico_generate_pio_header(vga_pio ${CMAKE_CURRENT_LIST_DIR}/vga_hsync.pio)
pico_generate_pio_header(vga_pio ${CMAKE_CURRENT_LIST_DIR}/vga_vsync.pio)
pico_generate_pio_header(vga_pio ${CMAKE_CURRENT_LIST_DIR}/vga_rgb.pio)


# pull in common dependencvgaies and additional hardware support
target_link_libraries(vga_pio PRIVATE pico_stdlib hardware_gpio hardware_uart hardware_pio hardware_dma)

# create map/bin/hex file etc.
pico_add_extra_outputs(vga_pio)

# add url via pico_set_program_url
example_auto_set_url(vga_pio)


