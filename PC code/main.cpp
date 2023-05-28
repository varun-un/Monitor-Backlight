#include <iostream>
using namespace std;
#include <string>
#include <stdlib.h>
#include "SerialPort.hpp"
#include "settings.hpp"

// g++ main.cpp SerialPort.cpp -lgdi32 -o main.exe

// change the name of the port with the port name of your computer
// must remember that the backslashes are essential so do not remove them
char port[] = "\\\\.\\COM6";

int pixels[] = {
    3840, 4000, 4382, 192500
};

int num_samples = (int)(sizeof(pixels) / sizeof(pixels[0]));


int main(int argc, char **argv) {

    HINSTANCE _hGDI = LoadLibraryA("gdi32.dll");

    HWND hDesktopWnd = GetDesktopWindow();
    HDC hDesktopDC = GetDC(hDesktopWnd);
    HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
    HBITMAP hCaptureBitmap = CreateCompatibleBitmap(hDesktopDC, SC_WIDTH, SC_HEIGHT);
    SelectObject(hCaptureDC, hCaptureBitmap);  

    // Create a BITMAPINFO specifying the format you want the pixels in.
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

            int pix = pixels[i];
            printf("Pixel %d: %d, %d, %d\n", pix, pPixels[pix].rgbRed, pPixels[pix].rgbGreen, pPixels[pix].rgbBlue);
            arduino.writeSerialPort(pPixels[pix].rgbRed, 1);
            arduino.writeSerialPort(pPixels[pix].rgbGreen, 1);
            arduino.writeSerialPort(pPixels[pix].rgbBlue, 1);

        }

        Sleep(500);
	}


	return 0;
}