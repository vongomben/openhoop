//  https://www.sparkfun.com/datasheets/Components/General/MSGEQ7.pdf

#define msg7RESET 3
#define msg7Strobe 2
#define msg7DCout A0
//const int LEDpins[7] = {3,5,5,6,9,9,10};    // there are 5 LEDs and 7 freq bands.  So, repeat LEDs
 
//#define pushButton 2
 
int msg7bands[7];
 
void setup() {
  SerialUSB.begin(115200);
  // initialize the digital pin as an output.
  // Pin 13 has an LED connected on most Arduino boards:
  //for (int x=0; x<7; x++) {
  //    pinMode(LEDpins[x], OUTPUT);
 // }
  pinMode(msg7RESET, OUTPUT);
  pinMode(msg7Strobe, OUTPUT);
 
  //pinMode(pushButton, INPUT);       // never actually used in this example.
  //digitalWrite(pushButton, HIGH);  // Enable internal pull-up
}
 
void loop() {
    digitalWrite(msg7RESET, HIGH);          // reset the MSGEQ7's counter
    delay(5);
    digitalWrite(msg7RESET, LOW);
 
    for (int x = 0; x < 7; x++){
        digitalWrite(msg7Strobe, LOW);      // output each DC value for each freq band
        delayMicroseconds(35); // to allow the output to settle
        int spectrumRead = analogRead(msg7DCout);
         SerialUSB.print(x);
         SerialUSB.print(" ");
         SerialUSB.print(spectrumRead);
         SerialUSB.print(" ");

        //int PWMvalue = map(spectrumRead, 0, 1024, 0, 255);  // scale analogRead's value to Write's 255 max
        //if (PWMvalue < 50)
        //    PWMvalue = PWMvalue / 2;        // bit of a noise filter, so the LEDs turn off at low levels
 
        //analogWrite(LEDpins[x], PWMvalue);
        digitalWrite(msg7Strobe, HIGH);
        //delay(1000);
    }
    SerialUSB.println();
    delay(100);
}

