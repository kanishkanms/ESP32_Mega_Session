## Setting up Environment for working with Micropython

### 1)Install Python3 and pip

Note:Before proceeding Connect you board and check whether the port is working properly if not install the necessary driver

### 2) Get the .bin file from Micropython website(https://micropython.org/download/esp32)
   - Given below is the link for stable release
   - https://micropython.org/resources/firmware/esp32-idf4-20210202-v1.14.bin
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

   ```
   esptool --chip esp32 --port COM5 erase_flash
   ```
   ```
   esptool --chip esp32 --port COM5 --baud 460800 write_flash -z 0x1000 esp32-idf4-20210202-v1.14.bin
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
[Link](https://learn.adafruit.com/micropython-basics-load-files-and-run-code/file-operations)

