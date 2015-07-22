/*
openhoop UI test

 */



const int btn[] = {8, 7, 6, 5 ,4}; // RIGHT, CENTER, UP, LEFT, DOWN
int val[5];


void setup() {
  //start serial connection
  Serial.begin(9600);

  for (int i = 0; i < 5; i = i + 1) {
    pinMode(i, INPUT_PULLUP);
  }

}

void loop() {

  // read the values
  for (int i = 0; i < 5; i = i + 1) {
    val[i] = digitalRead(btn[i]);
  }

// print the values

  for (int i = 0; i < 5; i = i + 1) {
    Serial.print(val[i]);
  }
  Serial.println();

}



