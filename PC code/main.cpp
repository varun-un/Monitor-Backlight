#include <iostream>
using namespace std;
#include <string>
#include <stdlib.h>
#include "SerialPort.hpp"
#include "settings.hpp"

// g++ main.cpp SerialPort.cpp -lgdi32 -o main.exe

// change the name of the port with the port name of your computer
// must remember that the backslashes are essential so do not remove them
char port[] = COM_PORT;

int pixels[] = {
    1863, 1692, 1521, 1350, 1179, 1008, 837, 666, 495, 324, 153, 1,
    376320, 716160, 1056000, 1395840, 1733760, 2071680,
    2071836, 2072012, 2072188, 2072364, 2072540, 2072716, 2072892, 2073068, 2073244, 2073420, 2073599,
    1852799, 1520639, 1188479, 856319, 524159, 191999,
    1787, 1615, 1444, 1272,
    296915, 629075, 961235, 1293395, 1625555, 1957715,
    2072777, 2072601, 2072425,
    1847667, 1507827, 1167987, 828147, 488307, 148467

};

int num_samples = (int)(sizeof(pixels) / sizeof(pixels[0]));

int main(int argc, char **argv) {

    printf("num_samples: %d\n", num_samples);


    HINSTANCE _hGDI = LoadLibraryA("gdi32.dll");

    HWND hDesktopWnd = GetDesktopWindow();
    HDC hDesktopDC = GetDC(hDesktopWnd);
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, SC_WIDTH, SC_HEIGHT);
    SelectObject(hCaptureDC, hCaptureBitmap);  

    // To keep this simple, we'll use 32-bits per pixel (the high byte isn't
    // used).
    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
    bmi.bmiHeader.biWidth = SC_WIDTH;
    bmi.bmiHeader.biHeight = SC_HEIGHT;
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    // Allocate a buffer to receive the pixel data.
    RGBQUAD *pPixels = new RGBQUAD[SC_WIDTH * SC_HEIGHT];


    // establish serial connection
	SerialPort arduino(port);
	if (arduino.isConnected()) {
		cout<<"Connection made"<<endl<<endl;
	}
	else {
		cout<<"Error in port name"<<endl<<endl;
	}

    // outer loop runs once for each frame
	while (arduino.isConnected()) {

        BitBlt(hCaptureDC, 0, 0, SC_WIDTH, SC_HEIGHT, hDesktopDC, 0,0, SRCCOPY|CAPTUREBLT);

        ::GetDIBits(hCaptureDC,
                hCaptureBitmap,
                0,  // starting scanline
                SC_HEIGHT,  // scanlines to copy
                pPixels,  // buffer for your copy of the pixels
                &bmi,  // format you want the data in
                DIB_RGB_COLORS);  // actual pixels, not palette references


        // inner loop for each pixel each frame
        for (int i = 0; i < num_samples; i++) {

            RGBQUAD pix = pPixels[pixels[i]];
            // printf("Pixel %d: %d, %d, %d\n", pix, pPixels[pix].rgbRed, pPixels[pix].rgbGreen, pPixels[pix].rgbBlue);
            arduino.writeSerialPort(pix.rgbRed, 1);
            arduino.writeSerialPort(pix.rgbGreen, 1);
            arduino.writeSerialPort(pix.rgbBlue, 1);

            // if (i > 28) {
            //     Sleep(1);
            // }
        }

        Sleep(5);
	}


	return 0;
}