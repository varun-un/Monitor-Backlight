#include <Arduino.h>
#include "FastLED.h"

#define NUM_LEDS 54
#define DATA_PIN 9

CRGB leds[NUM_LEDS];

void setup() {
  
    Serial.begin(9600);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

}

int i = 0;
int j = 0;

void loop() {
    
    if (Serial.available() > 0) {
        leds[j].b = 255;
        j++;
    }

    leds[i % 54].r = 255;
    FastLED.show();
    delay(100);
    i++;

}
