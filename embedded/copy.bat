openocd -f "\interface\cmsis-dap.cfg" -f "\target\rp2040.cfg" -c "adapter speed 5000" -c "program D:/Documents/Uni/embeddedpico/embedded_systems_rp_pico/embedded/build/vga/vga.elf verify reset exit"
