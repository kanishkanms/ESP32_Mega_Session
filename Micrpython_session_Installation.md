## What is Micropython?
MicroPython is a re-implementation of Python 3 targeted for microcontrollers and embedded systems. MicroPython is very similar with regular Python. So, if you already know how to program in Python, you also know how to program in MicroPython.

## Python vs MicroPython
Apart from a few exceptions, the language features of Python are also available in MicroPython. The biggest difference between Python and MicroPython is that MicroPython was designed to work under constrained conditions.

Because of that, MicroPython does not come with the full standard library. It only includes a small subset of the Python standard library. However, it does include modules to access low-level hardware – this means that there are libraries to easily access and interact with the GPIOs.

Additionally, devices with Wi-Fi capabilities like the ESP8266 and ESP32 include modules to support network connections.

## Why MicroPython?
Python is one of the most widely used, simple and easy-to-learn programming languages around. So, the emergence of MicroPython makes it extremely easy and simple to program digital electronics. If you’ve never programmed digital electronics before, MicroPython is a good starting point.

## Setting up Environment for working with Micropython

### 1)Install Python3 and pip 
   - Installing Python: https://www.tutorialsteacher.com/python/install-python
   - Download and Install VS code: https://code.visualstudio.com/download
   - Once installed add this extension : https://marketplace.visualstudio.com/items?itemName=ms-python.python
   - And you are set :)

    

*Note:Before proceeding Connect you board and check whether the port is working properly if not install the necessary driver*

### 2) Get the .bin file from Micropython website(https://micropython.org/download/esp32)
   - Given below is the link for stable release
   - https://micropython.org/resources/firmware/esp32-idf4-20191220-v1.12.bin
### 3) Install esptool and flash esp32
   ```
   pip install esptool
   ```
   Note: with some Python installations that command may not work and you’ll receive an error. If that’s the case, try to install esptool.py with:
   ```
   pip3 install esptool
   python -m pip install esptool
   pip2 install esptool
   ```
   Go to folder where you downloaded the .bin file  
   Using esptool.py you can erase the flash with the command:
   ```
   esptool.py --port COM5 erase_flash                             # For Windows
   esptool.py --port /dev/ttyUSB0 erase_flash                     # For Linux
   esptool.py --port /dev/cu.SLAB_USBtoUART erase_flash           # For Mac
   ```
   And then deploy the new firmware using:
   ```
   esptool.py --chip esp32 --port COM5 write_flash -z 0x1000 esp32-idf4-20191220-v1.12.bin                     # For Windows
   esptool.py --chip esp32 --port /dev/ttyUSB0 write_flash -z 0x1000 esp32-idf4-20191220-v1.12.bin             # For Linux
   esptool.py --chip esp32 --port /dev/cu.SLAB_USBtoUART write_flash -z 0x1000 esp32-idf4-20191220-v1.12.bin    # For Mac
   ```
   
### To run Micropython code on ESP32

```
pip install adafruit-ampy
```
Sample code
```
from machine import Pin
import time

p=Pin(2,Pin.OUT)

def toggle(max):
    lap=0

    while(lap<max):
        p.value(1)
        time.sleep(1)
        p.value(0)
        time.sleep(1)
        lap+=1

toggle(5)

```
## ***Before proceeding with the below commands, go through the link attached in the last line***
For uploading the code
```
ampy --port COM5 put code.py                              # For Windows
ampy --port /dev/ttyUSB0 put code.py                      # For Linux
ampy --port /dev/cu.SLAB_USBtoUART put code.py            # For Mac
```
Then run the code using this command after going to the right directory
```
ampy --port COM5 run code.py                              # For Windows
ampy --port /dev/ttyUSB0 run code.py                      # For Linux
ampy --port /dev/cu.SLAB_USBtoUART run code.py            # For Mac
```

### Some useful commands used in ampy
https://learn.adafruit.com/micropython-basics-load-files-and-run-code/file-operations
