// Basic (Cave-like) juxtaposition
// of MPU-6050 I2C data to LED STRIP behaviour
//
// based on fastled.io examples 
// and MPU code ound here https://plus.google.com/114004081524062063116/posts/E1aWjLT7xqt 
// 


// Wire stuff
#include<Wire.h>
const int MPU = 0x68; // I2C address of the MPU-6050
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
// end of wire stuff

// Fastled stuff
#include <FastLED.h>
#define COLOR_ORDER RGB
#define CHIPSET     LPD8806
#define NUM_LEDS    18
#define DATA_PIN    10
#define CLOCK_PIN   8 
CRGB leds[NUM_LEDS];

#define BRIGHTNESS  200
#define UPDATES_PER_SECOND 100

#define qsubd(x, b)  ((x>b)?wavebright:0)                     // Digital unsigned subtraction macro. if result <0, then => 0. Otherwise, take on fixed value.
#define qsuba(x, b)  ((x>b)?x-b:0)                            // Analog Unsigned subtraction macro. if result <0, then => 0


// https://www.sparkfun.com/datasheets/Components/General/MSGEQ7.pdf
// The seven band graphic equalizer IC is a CMOS chip that divides
// the audio spectrum into seven bands.
// 63Hz, 160Hz, 400Hz, 1kHz, 2.5kHz, 6.25kHz and 16kHz.
// We are using this module for the prototype
//

const int msg7RESET = 5;  
const int msg7Strobe = 4; 
const int msg7DCout = A5; 

int msg7bands[7];


// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your 
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through 
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of 
// FastLED compact palettes are at the bottom of this file.



CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;


void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LPD8806, DATA_PIN , CLOCK_PIN , RGB>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  
  currentPalette = RainbowColors_p;
  currentBlending = BLEND;
   
   
  // Wire stuff
 
  Serial.begin(9600); 
  Wire.begin();
  Wire.beginTransmission(MPU);
  Wire.write(0x6B);  // PWR_MGMT_1 register
  Wire.write(0);     // set to zero (wakes up the MPU-6050)
  Wire.endTransmission(true);
  // end of wire stuff
  
  //  equalizer
  pinMode(msg7RESET, OUTPUT);
  pinMode(msg7Strobe, OUTPUT);
}


void loop()
{
  readMSGEQ();
  
  /*
  for(int i= 0; i < NUM_LEDS; i++){
    leds[i] = CHSV(msg7bands[i%7], 255, 255); 
  }
  */
  
  Wire.beginTransmission(MPU);
  Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission(false);
  Wire.requestFrom(MPU, 14, true); // request a total of 14 registers
  AcX = Wire.read() << 8 | Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
  AcY = Wire.read() << 8 | Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read() << 8 | Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read() << 8 | Wire.read(); // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
  GyX = Wire.read() << 8 | Wire.read(); // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read() << 8 | Wire.read(); // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read() << 8 | Wire.read(); // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
  /*
  Serial.print("AcX = "); Serial.print(AcX);
  Serial.print(" | AcY = "); Serial.print(AcY);
  Serial.print(" | AcZ = "); Serial.print(AcZ);
  Serial.print(" | Tmp = "); Serial.print(Tmp / 340.00 + 36.53); //equation for temperature in degrees C from datasheet
  Serial.print(" | GyX = "); Serial.print(GyX);
  Serial.print(" | GyY = "); Serial.print(GyY);
  Serial.print(" | GyZ = "); Serial.println(GyZ);
  */
  delay(3);
  
 //ChangePalettePeriodically();
 int XXX = map(AcX, -16000, 16000, 0,255);
 int YYY = map(AcY, -16000, 16000, 0,255);
 int ZZZ = map(AcZ, -16000, 16000, 0,255);
 //ChangePalettePeriodically(XXX);
 //purpleEdit(XXX);
 //noChange(XXX);
 //Serial.print(" - ");
 //Serial.print(XXX);
 //Serial.println();

//  Xcolor(AcX);
  //one_sine(XXX);
  
  //one_sine(msg7bands[4]);
  audioEdit();
  
  static uint8_t startIndex = 0;
  //startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors(startIndex);

  FastLED.show();
  //FastLED.delay(1000 / UPDATES_PER_SECOND);
}

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3;
  }
}


// this is mine, not important :)
void noChange(int hue){
   for (int i = 0; i < NUM_LEDS; i++ ){
   leds[i].setHSV( hue, 255, 255);

   //currentPalette = SetupPurpleAndGreenPalette();        
   currentBlending = NOBLEND;
   }
}

///START sine

// Initialize changeable global variables. Play around with these!!!
  uint8_t wavebright = 128;                                     // You can change the brightness of the waves/bars rolling across the screen.
  uint8_t thishue = 0;                                          // You can change the starting hue value for the first wave.
  uint8_t thisrot = 1;                                          // You can change how quickly the hue rotates for this wave. Currently 0.
  uint8_t allsat = 255;                                         // I like 'em fully saturated with colour.
  int8_t thisspeed = 8;                                         // You can change the speed of the wave, and use negative values.
  uint8_t allfreq = 32;                                         // You can change the frequency, thus distance between bars.
  int thisphase = 0;                                            // Phase change value gets calculated.
  uint8_t thiscutoff = 192;                                     // You can change the cutoff value to display this wave. Lower value = longer wave.
  int thisdelay = 30;                                           // You can change the delay. Also you can change the allspeed variable above. 
  uint8_t bgclr = 0;                                            // A rotating background colour.
  uint8_t bgbri = 16;                                           // Brightness of background colour

void one_sine(uint8_t hue) {                                                             // This is the heart of this program. Sure is short.

  thishue = hue;

  thisphase += thisspeed;                                                     // You can change direction and speed individually.
  thishue = thishue + thisrot;                                                // Hue rotation is fun for thiswave.
  for (int k=0; k<NUM_LEDS-1; k++) {                                          // For each of the LED's in the strand, set a brightness based on a wave as follows:
    int thisbright = qsubd(cubicwave8((k*allfreq)+thisphase), thiscutoff);    // qsub sets a minimum value called thiscutoff. If < thiscutoff, then bright = 0. Otherwise, bright = 128 (as defined in qsub)..
    leds[k] = CHSV(bgclr, 255, bgbri);                                        // First set a background colour, but fully saturated.
    leds[k] += CHSV(thishue, allsat, thisbright);                             // Then assign a hue to any that are bright enough.
  }
  bgclr++;                                                                    // You can change the background colour or remove this and leave it fixed.
} // one_sine()


//END sine 

void purpleEdit(int hue){
  SetupPurpleAndGreenPalette(hue); 
  currentBlending = BLEND;
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically(int hue)
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    //if( secondHand ==  0)  { currentPalette = RainbowColors_p;         currentBlending = BLEND; }
    //if( secondHand == 10)  { currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  }
    //if( secondHand == 15)  { currentPalette = RainbowStripeColors_p;   currentBlending = BLEND; }
    if( secondHand == 20)  { SetupPurpleAndGreenPalette(hue);             currentBlending = BLEND; }
    else { SetupTotallyRandomPalette(hue);              currentBlending = BLEND; }
    //if( secondHand == 25)  { SetupTotallyRandomPalette(hue);              currentBlending = BLEND; }
    //if( secondHand == 30)  { SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; }
    //if( secondHand == 35)  { SetupBlackAndWhiteStripedPalette();       currentBlending = BLEND; }
    //if( secondHand == 40)  { currentPalette = CloudColors_p;           currentBlending = BLEND; }
    //if( secondHand == 45)  { currentPalette = PartyColors_p;           currentBlending = BLEND; }
    //if( secondHand == 50)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  }
    //if( secondHand == 55)  { currentPalette = myRedWhiteBluePalette_p; currentBlending = BLEND; }
  }
}

// this function tries to change the color of the whole strip
// based on the X position 
void Xcolor(int hue){
for( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( hue, 255, random8());
  }

}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette(int hue)
{
  for( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(hue), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette(int hue)
{
  CRGB purple = CHSV( hue, 255, 255);
//  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( hue-255, 255, 255);
  CRGB black  = CRGB::Black;
  
  currentPalette = CRGBPalette16( 
    green,  green,  black,  black,
    purple, purple, black,  black,
    green,  green,  black,  black,
    purple, purple, black,  black );
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more 
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

// This function sets up a palette of purple and green stripes.
void SetupAudioPalette()
{
  CRGB bandA = CHSV( msg7bands[3], 255, 255);
  CRGB bandB  = CHSV( msg7bands[6], 255, 255);
  CRGB bandC  = CHSV( msg7bands[7], 255, 255);
  CRGB black  = CRGB::Black;
  
  
  currentPalette = CRGBPalette16( 
    bandA,  bandB,  bandC,  black,
    bandA,  bandB,  bandC,  black,
    bandA,  bandB,  bandC,  black,
    bandA,  bandB,  bandC,  black );
}
void audioEdit(){
  SetupAudioPalette(); 
  currentBlending = BLEND;
}



// Additionl notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes. 
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette 
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.


void readMSGEQ() {
  digitalWrite(msg7RESET, HIGH);          // reset the MSGEQ7's counter
  delay(5);
  digitalWrite(msg7RESET, LOW);

  for (int x = 0; x < 7; x++) {
    digitalWrite(msg7Strobe, LOW);      // output each DC value for each freq band
    delayMicroseconds(35); // to allow the output to settle
    int spectrumRead = analogRead(msg7DCout);
    msg7bands[x] = map(spectrumRead, 0, 1024, 0, 254);
    Serial.print(x);
     Serial.print(" ");
     Serial.print(spectrumRead);
     Serial.print(" ");
    digitalWrite(msg7Strobe, HIGH);
    delay(1);
  }
  Serial.println();
}

