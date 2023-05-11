add_executable(servo_pwm
        servo_pwm_button.c
        )

# pull in common dependencies and additional pwm hardware support
target_link_libraries(servo_pwm pico_stdlib hardware_pwm)

# create map/bin/hex file etc.
pico_add_extra_outputs(servo_pwm)

# add url via pico_set_program_url
example_auto_set_url(servo_pwm)
