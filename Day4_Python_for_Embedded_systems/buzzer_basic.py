from machine import Pin, PWM
import time

BUZZER_PIN = 23

beeper = PWM(Pin(BUZZER_PIN), freq=440, duty=512)
time.sleep(0.5)     # delay for 0.5 seconds
beeper.deinit()
