/*
  by David Rios
  NYU IMA - Designing Interfaces for Live Performance 2022
  Code for DILP Serial Buttons/ Arduino Class 2022
*/
// button pin Variables:
int b1 = 2;
int b2 = 3;
int b3 = 4;

void setup() {
  Serial.begin(9600);
  pinMode(b1, INPUT_PULLUP);
  pinMode(b2, INPUT_PULLUP);
  pinMode(b3, INPUT_PULLUP);
}

void loop() {
  
  if (Serial.available() > 0) {
    int incoming = Serial.read();
    Serial.print(digitalRead(b1));
    Serial.print(",");
    Serial.print(digitalRead(b2));
    Serial.print(",");
    Serial.print(digitalRead(b3));
    Serial.print("\n");
  }
}
