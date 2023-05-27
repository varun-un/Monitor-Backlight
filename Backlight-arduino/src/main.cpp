#include <Arduino.h>
#include "FastLED.h"

#define NUM_LEDS 54
#define DATA_PIN 9

CRGB leds[NUM_LEDS];

void setup() {
  
    Serial.begin(9600);
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

    for (int i = 0; i < NUM_LEDS; i++) {
        leds[i % 54].r = 255;
        FastLED.show();
        delay(100);
    }

}

int j = 0;

void loop() {
    
    if (Serial.available() > 0) {
        Serial.read();
        leds[j].b = 255;
        j++;
    }

    FastLED.show();


}
