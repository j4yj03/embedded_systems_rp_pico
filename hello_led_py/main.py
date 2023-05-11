import machine
import utime

led_onboard = machine.Pin(25, machine.Pin.OUT)

led_extern = machine.Pin(1, machine.Pin.OUT)

button = machine.Pin(14, machine.Pin.IN, machine.Pin.PULL_DOWN)

#val = 1

while True:
    #led_onboard.value(val)
    led_onboard.toggle()
    led_extern.toggle()
    #val = not val
    if not button.value() == 1:
        utime.sleep(1)
    else:
        utime.sleep(0.1)
