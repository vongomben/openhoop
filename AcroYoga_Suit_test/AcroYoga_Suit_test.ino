// MultiArrays - see https://github.com/FastLED/FastLED/wiki/Multiple-Controller-Examples for more info on
// using multiple controllers.  In this example, we're going to set up four NEOPIXEL strips on three
// different pins, each strip getting its own CRGB array to be played with

#include "FastLED.h"

#define COLOR_ORDER BGR
#define CHIPSET     APA102 

#define DATA_PIN_F 2
#define CLOCK_PIN_F 3

#define DATA_PIN_DX 4
#define CLOCK_PIN_DX 5

#define DATA_PIN_B 6
#define CLOCK_PIN_B 7

#define DATA_PIN_SX 8
#define CLOCK_PIN_SX 9

#define NUM_LEDS 10
#define NUM_LEDS_PER_STRIP_F 28
#define NUM_LEDS_PER_STRIP_DX 37
#define NUM_LEDS_PER_STRIP_B 35
#define NUM_LEDS_PER_STRIP_SX 37

#define BRIGHTNESS  200
#define FRAMES_PER_SECOND 120

CRGB ledsF[NUM_LEDS_PER_STRIP_F];
CRGB ledsDX[NUM_LEDS_PER_STRIP_DX];
CRGB ledsB[NUM_LEDS_PER_STRIP_B];
CRGB ledsSX[NUM_LEDS_PER_STRIP_SX];

CRGBPalette16 gPal;

//CRGB redLeds[NUM_LEDS_PER_STRIP];
//CRGB greenLeds[NUM_LEDS_PER_STRIP];
//CRGB blueLeds[NUM_LEDS_PER_STRIP];

// For mirroring strips, all the "special" stuff happens just in setup.  We
// just addLeds multiple times, once for each strip
void setup() {
  // tell FastLED there's 60 NEOPIXEL leds on pin 10
//  FastLED.addLeds<APA102, 10>(redLeds, NUM_LEDS_PER_STRIP);
//
//  // tell FastLED there's 60 NEOPIXEL leds on pin 11
//  FastLED.addLeds<APA102, 11>(greenLeds, NUM_LEDS_PER_STRIP);
//
//  // tell FastLED there's 60 NEOPIXEL leds on pin 12
//  FastLED.addLeds<APA102, 12>(blueLeds, NUM_LEDS_PER_STRIP);

FastLED.addLeds<APA102, DATA_PIN_F, CLOCK_PIN_F, COLOR_ORDER>(ledsF, NUM_LEDS_PER_STRIP_F);
FastLED.addLeds<APA102, DATA_PIN_DX, CLOCK_PIN_DX, COLOR_ORDER>(ledsDX, NUM_LEDS_PER_STRIP_DX);
FastLED.addLeds<APA102, DATA_PIN_B, CLOCK_PIN_B, COLOR_ORDER>(ledsB, NUM_LEDS_PER_STRIP_B);
FastLED.addLeds<APA102, DATA_PIN_SX, CLOCK_PIN_SX, COLOR_ORDER>(ledsSX, NUM_LEDS_PER_STRIP_SX);
FastLED.setBrightness( BRIGHTNESS );
  //gPal = CRGBPalette16( CRGB::Black, CRGB::Red, CRGB::Yellow, CRGB::White);

}

void loop() {

  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

 // Fourth, the most sophisticated: this one sets up a new palette every
  // time through the loop, based on a hue that changes every time.
  // The palette is a gradient from black, to a dark color based on the hue,
  // to a light color based on the hue, to white.
  //
     static uint8_t hue = 0;
     hue++;
     CRGB darkcolor  = CHSV(hue,255,192); // pure hue, three-quarters brightness
     CRGB lightcolor = CHSV(hue,128,255); // half 'whitened', full brightness
     gPal = CRGBPalette16( CRGB::Black, darkcolor, lightcolor, CRGB::White);

Fire2012WithPalette(); // run simulation frame, using palette colors
  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);

  
//  for(int i = 0; i < NUM_LEDS_PER_STRIP; i++) {
//    // set our current dot to red, green, and blue
//    redLeds[i] = CRGB::Red;
//    greenLeds[i] = CRGB::Green;
//    blueLeds[i] = CRGB::Blue;
//    FastLED.show();
//    // clear our current dot before we move on
//    redLeds[i] = CRGB::Black;
//    greenLeds[i] = CRGB::Black;
//    blueLeds[i] = CRGB::Blue;
//    delay(100);
//  }

//  for(int i = NUM_LEDS_PER_STRIP-1; i >= 0; i--) {
//    // set our current dot to red, green, and blue
//    redLeds[i] = CRGB::Red;
//    greenLeds[i] = CRGB::Green;
//    blueLeds[i] = CRGB::Blue;
//    FastLED.show();
//    // clear our current dot before we move on
//    redLeds[i] = CRGB::Black;
//    greenLeds[i] = CRGB::Black;
//    blueLeds[i] = CRGB::Blue;
//    delay(100);
//  }
}



// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 55, suggested range 20-100 
#define COOLING  55

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 120


void Fire2012WithPalette()
{
// Array of temperature readings at each simulation cell
  static byte heat1[NUM_LEDS_PER_STRIP_F];
  static byte heat2[NUM_LEDS_PER_STRIP_DX];
  static byte heat3[NUM_LEDS_PER_STRIP_B];
  static byte heat4[NUM_LEDS_PER_STRIP_SX];

  // Step 1.  Cool down every cell a little
    for( int i = 0; i < NUM_LEDS ; i++) {
      heat1[i] = heat2[i] = heat3[i] = heat4[i] = qsub8( heat1[i],  random8(0, ((COOLING * 10) / 10) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= NUM_LEDS - 1; k >= 2; k--) {
      heat1[k] = heat2[k] = heat3[k] = heat4[k] = (heat1[k - 1] + heat1[k - 2] + heat1[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat1[y] = heat2[y] = heat3[y] = heat4[y] = qadd8( heat1[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = 0; j < NUM_LEDS; j++) {
      // Scale the heat value from 0-255 down to 0-240
      // for best results with color palettes.
      byte colorindex = scale8( heat1[j], 240);
      ledsDX[j] = ColorFromPalette( gPal, colorindex);
    }
}

