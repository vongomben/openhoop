/*
    Basic MSGEQ7 Serial Out 
    https://www.sparkfun.com/datasheets/Components/General/MSGEQ7.pdf
    
*/

#define msg7RESET 9
#define msg7Strobe A0
#define msg7DCout A5
 
int msg7bands[7];
 
void setup() {
  Serial.begin(115200);

  pinMode(msg7RESET, OUTPUT);
  pinMode(msg7Strobe, OUTPUT);
 
}
 
void loop() {
    digitalWrite(msg7RESET, HIGH);          // reset the MSGEQ7's counter
    delay(5);
    digitalWrite(msg7RESET, LOW);
 
    for (int x = 0; x < 7; x++){
        digitalWrite(msg7Strobe, LOW);      // output each DC value for each freq band
        delayMicroseconds(35); // to allow the output to settle
        int spectrumRead = analogRead(msg7DCout);
         Serial.print(x);
         Serial.print(" ");
         Serial.print(spectrumRead);
         Serial.print(" ");

         digitalWrite(msg7Strobe, HIGH);

    }
    Serial.println();
    delay(10);
}
