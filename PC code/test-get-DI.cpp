#include "settings.hpp"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>

#define NUM_SAMPLES 17 * 2 * 2 * 4

// g++ test-get-DI.cpp -lgdi32 -o test.exe

// to measure time elapsed
using namespace std::chrono_literals;

template <
    class result_t   = std::chrono::milliseconds,
    class clock_t    = std::chrono::steady_clock,
    class duration_t = std::chrono::milliseconds
>

auto since(std::chrono::time_point<clock_t, duration_t> const& start)
{
    return std::chrono::duration_cast<result_t>(clock_t::now() - start);
}


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

    // set up array for samples
    int arr[NUM_SAMPLES];

    for (int i = 0; i < NUM_SAMPLES; i++) {
        arr[i] = i * 100;
    }

    // start time measurement
    auto start = std::chrono::steady_clock::now();

    BitBlt(hCaptureDC, 0, 0, SC_WIDTH, SC_HEIGHT, hDesktopDC, 0,0, SRCCOPY|CAPTUREBLT);

    // Call GetDIBits to copy the bits from the device dependent bitmap
    // into the buffer allocated above, using the pixel format you
    // chose in the BITMAPINFO.
    ::GetDIBits(hCaptureDC,
                hCaptureBitmap,
                0,  // starting scanline
                SC_HEIGHT,  // scanlines to copy
                pPixels,  // buffer for your copy of the pixels
                &bmi,  // format you want the data in
                DIB_RGB_COLORS);  // actual pixels, not palette references

    for (int i = 0; i < NUM_SAMPLES; i++) {



        // You can now access the raw pixel data in pPixels.  Note that they are
        // stored from the bottom scanline to the top, so pPixels[0] is the lower
        // left pixel, pPixels[1] is the next pixel to the right,
        // pPixels[nScreenWidth] is the first pixel on the second row from the
        // bottom, etc.
        printf("Color: (%d, %d, %d)\n", pPixels[i].rgbRed, pPixels[i].rgbGreen, pPixels[i].rgbBlue);
    }

    //end time measurement
    printf("Time elapsed: %d us\n", since<std::chrono::microseconds>(start).count());

    // Don't forget to free the pixel buffer.
    delete [] pPixels;

    return 0;
}