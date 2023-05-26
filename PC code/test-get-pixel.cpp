#include "settings.hpp"
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <thread>

#define NUM_SAMPLES 17 * 2 * 2 * 4

// g++ test-get-pixel.cpp -lgdi32 -o test.exe

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

    HDC dc = GetDC(NULL);

    int arr[NUM_SAMPLES];

    for (int i = 0; i < NUM_SAMPLES; i++) {
        arr[i] = i * 2;
    }

    // start time measurement
    auto start = std::chrono::steady_clock::now();

    for (int i = 0; i < NUM_SAMPLES; i++) {
        COLORREF color = GetPixel(dc, arr[i], arr[i]);
        printf("Color: (%d, %d, %d)\n", GetRValue(color), GetGValue(color), GetBValue(color));
    }

    //end time measurement
    printf("Time elapsed: %d us\n", since<std::chrono::microseconds>(start).count());

    ReleaseDC(NULL, dc);
    return 0;
}