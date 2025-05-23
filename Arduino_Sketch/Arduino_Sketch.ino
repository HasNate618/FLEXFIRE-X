#include <Servo.h>

Servo servo1;  // linear actuator
Servo servo2;  // fires projectile

int servo1Pin = 3;
int servo2Pin = 2;

int lastServo1Angle = -1;
int servo1Angle = 180;
int servo2Angle = 0;

int emgPin = A0;
int emgValue = 1023;

void setup() {
  Serial.begin(9600);

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  servo1.write(0);
  servo2.write(0);
}

void loop() {
  emgValue = analogRead(emgPin);
  Serial.println(emgValue);

  servo1.write(map(emgValue, 1023, 0, 0, 180));

  delay(200);
}
