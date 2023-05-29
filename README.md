# Monitor Backlight
 
This is a project to create a custom, from-scratch, backlight for a monitor. It uses LEDs to extend whatever is displayed on the screen's colors in realtime, providing ambient backlighting. This project's code is Windows-specific, as it uses a lot of the libraries native to Windows in order to read pixel data from the screen.

![](/imgs/Backlight.gif)

## Hardware Setup
The physical set up of this project is that there is a program running locally on the computer whose screen is trying to be extended. Then, there is a USB cable that connects the computer to an Arduino Nano, and is used for Serial data communication at a 9600 baud rate. A 54 LED WS2812 RGB strip has its power and ground driven off this arduino, and its data cable is connected to digital pin 9 on the Nano. The LED strip is then stuck to the back of the monitor, as seen below:

![](/imgs/monitor-leds.jpeg)

### How to use
To use this code, a couple things may need to be changed. In the [settings.hpp](/PC%20code/settings.hpp) file, replace the `COM_PORT` name's number with the number of the port that the Arduino is connected to. Then, according to how the LEDs were attached, the pixel array values in the [main.cpp](/PC%20code/main.cpp) file will have to be changed. With the closest LED to the Arduino wiring wise being index 0, all the way to index 53, put the *int* pixel value corresponding to that LED. A value of 0 is the bottom left pixel, and it increments left to right, going up one row and going all the way to the left when the end of the row is reached. This way, the top left pixel, for example, is 1920 * 1079 for a 1080p display.

### Running the program
To run the program, simply run the [Monitor-Backlight.bat](/PC%20code/Monitor-Backlight.bat) batch file. This will recompile the executable if needed, which requires the g++ command, and then run it, starting up the program. The Arduino should be plugged in when the batch file is ran. To stop it, simply exit the terminal or unplug the Arduino.

## Software
For the software side, the PC program first obtains the pixel data. Originally, the `GetPixel()` function from `windows.h` was used, but due to the quantity of pixels whose information was needed, running this for each pixel for each frame was computationally expensive. As an alternative, `BitBlt` is used, combined with `GetDIBits()` to capture a screenshot of all pixel data in an array. This can be done once for each frame, with the values from this screenshot being used to get all 54 pixels' data in a cheaper way. A speed comparison can be seen below, with the times being in nanoseconds it took to run (averaged over 3 trials):

| # of Pixels | GetPixel()  | GetDIBits() |
| ----------- | ----------- | ----------- |
| 68          | 1133318 us  | 62217  us   |
| 136         | 2272363 us  | 82745  us   |
| 204         | 3449027 us  | 93393  us   |
| 272         | 4553573 us  | 119384 us   |

Then, this data was transferred over Serial to the Arduino at a baud rate of 9600, sending data byte by byte, so that was the Arduino could process new data as soon as it received, rather than sending data in chunks that could cause execution to be backed up and the Arduino's receiving buffer to overflow and overwrite not-yet-processed data. Each byte represents the 16-bit color value, for either red, green or blue, for a given pixel. The data is sent in the order of the first pixel's red value, then its green value, then its blue value, before sending the same for the second pixel. Once all 54 pixels to be processed have been sent, a new screencapture is made, with this process repeating for all the pixels.
