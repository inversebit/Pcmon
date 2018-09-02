# Pcmon
Analog visualization for PC resources

![Image of Pcmon](/pcmon-device.jpg)

Further information is available on this post: [Pcmon](https://inversebit.github.io/projects/2018-09-02-pcmon)

Summary
=======

This is a device which shows the current CPU and RAM use of a PC. It has two parts, a .NET app acting as a server which reads the PC resources and pushes their values to the second part: An arduino board. The arduino board reads the values and shows them in two analog VU-meters.

.NET App
========
The .NET app is a simple console app. You should be able to open it in Visual Studio and compile it without problems. It doesn't have any dependencies.

Adjust the following values in [Program.cs](/pc-app/ArduinoPCMonitor/ArduinoPCMonitor/Program.cs) to make it work with your machine:
- `PC_MEM` to match your PC's max memory
- `_serialPort.PortName` (in line 48) to match your Arduino's serial port

Arduino client
==============
Dependencies
------------
You will need the [FastLED](http://fastled.io/) library to compile the code. Look for it in the Library Manager of the Arduino IDE.

VU-Meters
---------
The VU-meters are a pair of cheap 0-15V voltage meters I found on ebay. I then changed the resistor inside to make them work in a 0-3.3V range. Finally I scanned the original panel, and then designed and printed a new panel to show the units and ranges I needed.

Wiring
------
Arduino digital ports D5 and D6 go one to each of the VU-meters' positive pole. The VU-meters' negative pole is connected to a GND pin on the arduino.

The Neopixel LED sticks (or strips) are connected to GND and +5 volt. The signal is sent via digital port D11.

License
=======
The code is licensed under the MIT license. Check the [license](/LICENSE) file or get the full text at https://opensource.org/licenses/MIT.
