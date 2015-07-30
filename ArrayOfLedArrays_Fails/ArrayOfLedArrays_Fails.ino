// ArrayOfLedArrays - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples for more info on
// using multiple controllers.  In this example, we're going to set up four NEOPIXEL strips on three
// different pins, each strip getting its own CRGB array to be played with, only this time they're going
// to be all parts of an array of arrays.

#include "FastLED.h"


#define COLOR_ORDER BGR
#define CHIPSET     APA102 

#define NUM_STRIPS 4
#define NUM_LEDS_PER_STRIP 10
CRGB leds[NUM_STRIPS][NUM_LEDS_PER_STRIP];

#define DATA_PIN_F 2
#define CLOCK_PIN_F 3

#define DATA_PIN_DX 4
#define CLOCK_PIN_DX 5

#define DATA_PIN_B 6
#define CLOCK_PIN_B 7

#define DATA_PIN_SX 8
#define CLOCK_PIN_SX 9

// For mirroring strips, all the "special" stuff happens just in setup.  We
// just addLeds multiple times, once for each strip
void setup() {
//  // tell FastLED there's 60 NEOPIXEL leds on pin 10
//  FastLED.addLeds<NEOPIXEL, 10>(leds[0], NUM_LEDS_PER_STRIP);
//
//  // tell FastLED there's 60 NEOPIXEL leds on pin 11
//  FastLED.addLeds<NEOPIXEL, 11>(leds[1], NUM_LEDS_PER_STRIP);
//
//  // tell FastLED there's 60 NEOPIXEL leds on pin 12
//  FastLED.addLeds<NEOPIXEL, 12>(leds[2], NUM_LEDS_PER_STRIP);
FastLED.addLeds<CHIPSET, DATA_PIN_F, CLOCK_PIN_F, COLOR_ORDER>(leds[0], NUM_LEDS_PER_STRIP);
FastLED.addLeds<CHIPSET, DATA_PIN_DX, CLOCK_PIN_DX, COLOR_ORDER>(leds[1], NUM_LEDS_PER_STRIP);
FastLED.addLeds<CHIPSET, DATA_PIN_B, CLOCK_PIN_B, COLOR_ORDER>(leds[2], NUM_LEDS_PER_STRIP);
FastLED.addLeds<CHIPSET, DATA_PIN_SX, CLOCK_PIN_SX, COLOR_ORDER>(leds[3], NUM_LEDS_PER_STRIP);

}

void loop() {
  // This outer loop will go over each strip, one at a time
  for(int x = 0; x < NUM_STRIPS; x++) {
    // This inner loop will go over each led in the current strip, one at a time
    for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
      leds[x][i] = CRGB::Red;
      FastLED.show();
      leds[x][i] = CRGB::Black;
      delay(100);
    }
  }
}
