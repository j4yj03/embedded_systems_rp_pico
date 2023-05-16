openocd -f "cmsis-dap.cfg" -f "rp2040.cfg" -c "adapter speed 5000" -c "D:\Documents\Uni\embeddedpico\embedded_systems_rp_pico\embedded\build\servo_pwm\servo_pwm.elf verify reset exit"
