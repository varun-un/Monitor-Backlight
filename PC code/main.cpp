#include "settings.hpp"
#include <windows.h>
#include <stdio.h>
#include <iostream>


//g++ main.cpp -lgdi32

using namespace std ;

#define NUM_SAMPLES 17 * 2 * 2 * 3

int main(int argc, char **argv) {
    HINSTANCE _hGDI = LoadLibrary("gdi32.dll");

    HDC dc = GetDC(NULL);

    int arr[NUM_SAMPLES];

    for (int i = 0; i < NUM_SAMPLES; i++) {
        arr[i] = i * 2;
    }

    for (int i = 0; i < NUM_SAMPLES; i++) {
        COLORREF color = GetPixel(dc, i, 500);
        printf("Color: (%d, %d, %d)\n", GetRValue(color), GetGValue(color), GetBValue(color));
        Sleep(100);
    }

    ReleaseDC(NULL, dc);
    return 0;
}