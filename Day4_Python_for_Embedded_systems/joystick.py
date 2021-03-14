import utime
from machine import Pin, ADC

JOYSTICK_X_PIN = 33
JOYSTICK_Y_PIN = 35
JOYSTICK_SW_PIN = 25

while True:
    # set 11dB input attenuation (voltage range roughly 0.0v - 3.6v)
    adc_x = ADC(Pin(JOYSTICK_X_PIN))
    adc_x.atten(ADC.ATTN_11DB)
    adc_y = ADC(Pin(JOYSTICK_Y_PIN))
    adc_y.atten(ADC.ATTN_11DB)
    print("adc_x", adc_x.read(), "adc_y", adc_y.read())
    # read() function are between 0 (for 0.0 volts) and 1024 (for 1.0 volts)
    utime.sleep(1)
