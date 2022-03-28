int sensor1;
int sensor2;
int sensor3;

void setup() {
  Serial.begin(9600);
}

void loop() {
  sensor1 = analogRead(A0);
  sensor2 = analogRead(A1);
  sensor3 = analogRead(A2);

  if (Serial.available() > 0) {
    int incoming = Serial.read();
    Serial.print(sensor1);
    Serial.print(",");
    Serial.print(sensor2);
    Serial.print(",");
    Serial.print(sensor3);
    Serial.print('\n');
  }

}
