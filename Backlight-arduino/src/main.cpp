#include <Arduino.h>
#include "FastLED.h"

#define NUM_LEDS 54
#define DATA_PIN 9

CRGB leds[NUM_LEDS];
int j = 0;
int num_colors = NUM_LEDS * 3;                                              

//create pointer array of leds and values to change
uint8_t *led_pointers[162];                                         //num_colors

void setup() {
  
    Serial.begin(9600);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    //set up pointer array
    for (int i = 0; i < 162; i++) {                                  //num_colors

        if (i % 3 == 0)
            led_pointers[i] = &(leds[i / 3].r);
        if (i % 3 == 1)
            led_pointers[i] = &(leds[i / 3].g);
        if (i % 3 == 2)
            led_pointers[i] = &(leds[i / 3].b);
    }
}

void loop() {
    
    while (Serial.available() > 0) {
        *(led_pointers[j]) = Serial.read();
        j = (j + 1) % 162;                                   //num_colors
    }

    FastLED.show();


}
