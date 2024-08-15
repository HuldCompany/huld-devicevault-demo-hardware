# Arduino Notes
Details on how to set up and use Arduino as DeviceVault demo hardware

## Getting Started with Arduino
First time with Arduino, no problem!

### Install the Arduino IDE
Download IDE from https://www.arduino.cc/en/software 
Windows laptop - I used the MSI

### Install Arduino Driver

Beginners’ tutorial - Print hello world to COM port
https://arduinogetstarted.com/tutorials/arduino-hello-world 
Connect Arduino to USB on laptop
Follow tutorial

## Grove Beginner Kit For Arduino
Sensor Kit Tutorial https://wiki.seeedstudio.com/Grove-Beginner-Kit-For-Arduino/ 
Note: requires CP2102 USB Driver to be installed. I used the Universal Windows Driver
Download and move ZIP to tools, e.g. C:\dev\tools\arduino
Unpack ZIP
Right click silabser.inf and select install
Device manager should now list the COM port without a warning
Open Arduino IDE
Select board type Tools -> Board-> Arduino AVR Boards-> Arduino Uno
Select COM port
Copy the Hello World example, Verify and Upload
Tools -> Serial Monitor to view the output

## Using the DeviceVault Demo
