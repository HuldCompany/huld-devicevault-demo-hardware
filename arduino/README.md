# Getting Started
First time with Arduino, no problem!

These steps were taken from the excellent tutorial at https://wiki.seeedstudio.com/Grove-Beginner-Kit-For-Arduino/

## Install the Arduino IDE
Download Arduino IDE from https://www.arduino.cc/en/software
(On a Windows laptop, I downloaded the MSI)

Install by executing the downloaded file.

## Install Arduino Driver

The default USB Serial driver only allows communication with the Arduino Sketch.
For the Arduino IDE to communicate with the Arduino, you need a special Driver.

Download the CP2102 USB driver from https://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers

Steps for Windows Laptop:
1. Download and move ZIP to tools, e.g. C:\dev\tools\arduino
1. Unpack ZIP
1. Right click silabser.inf and select install

Device manager should now list the Arduino COM port without a warning

## Hello World

Follow the steps at https://wiki.seeedstudio.com/Grove-Beginner-Kit-For-Arduino/#start-the-arduino-ide

1. Open Arduino IDE
1. Select board type Tools -> Board-> Arduino AVR Boards-> Arduino Uno
1. Select COM port
1. Copy the Hello World example, Verify and Upload
1. Tools -> Serial Monitor to view the output

## Grove Beginner Kit For Arduino

All the sensor modules can be tested by following the tutorials at https://wiki.seeedstudio.com/Grove-Beginner-Kit-For-Arduino/#lesson-guide
Sensor Kit Tutorial https://wiki.seeedstudio.com/Grove-Beginner-Kit-For-Arduino/

# DeviceVault Demo Hardware

The DeviceVault Demo Hardware is an Arduino Sketch that allows interaction with the Arduino for the purposes of hardware testing.

## Preparation

The sketch can be downloaded from https://github.com/HuldCompany/huld-devicevault-demo-hardware

### Code Modification

The sketch may require modification depending on the type of Arduino you have.

#### Grove Beginner Kit For Arduino

The sketch requires no modification.

#### Arduino Uno R3

Comment out the following lines
```
//#define USE_TEMP_HUMIDITY_DHT20 true
//#define USE_PRESSURE_BMP280 true
//#define USE_ACCEL_LIS3DHTR true
//#define USE_OLED_U8X8 true
```
#### Arduino Uno R4

Comment out the following lines
```
//#define USE_TEMP_HUMIDITY_DHT20 true
//#define USE_PRESSURE_BMP280 true
//#define USE_ACCEL_LIS3DHTR true
//#define USE_OLED_U8X8 true

//#define USE_LIGHT_SENSOR_A6 true
```

Un-comment the following lines
```
#define USE_LED_MATRIX true
```

### Libraries

The sketch requires libraries to be added to the IDE, otherwise compilation errors will occur.

Note: Not all the libraries are needed, it depends on which options are uncommented in the code configuration block. 

- Grove Beginner Kit For Arduino
  - **Temperature and Humidity DHT20** - "Grove Temperature and Humidity Sensor" *by Seed Studio*
  - **Pressure BMP280** - "Grove - Barometer Sensor BMP280" *by Seed Studio*
  - **Acceleration LIS3DHTR** - "Grove-3-Axis-Digital-Accelerometer-2g-to-16g" *by Seed Studio*
  - **OLED Display** - "U8g2 library" *by oliver*
- Uno R4
  - **LED Matrix** (Uno R4) - "Arduino Graphics" *by Arduino*


## Using the Demo Hardware

By default, the Arduino prints a line of status data every few seconds.
Every time status data is printed, the onboard LED changes state.
Timing and content of the status data can be altered via the keyboard menu.

The keyboard menu is as follows:
```
DeviceVault Demo Hardware v1.0
Arduino Uno - 20 digital pins, 6 analog inputs
1: Toggle LED
2: Beep
3: OLED Status
4: Set Speed
5: Toggle periodic print state (LED, Speed, Sound, Light)
6: Toggle periodic print Temperature and Humidity DHT20
7: Toggle periodic print Pressure BMP280
8: Toggle periodic print Acceleration LIS3DHTR
9: Toggle periodic print Time
0: Print Status (Formatted)
x: Reset the Arduino
?: Show menu
```
Note: The keyboard menu only shows options applicable to the Arduino model.
