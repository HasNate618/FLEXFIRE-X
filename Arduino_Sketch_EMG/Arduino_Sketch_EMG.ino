#include <Servo.h>

Servo servo1; // Rotational: toggles 0 ↔ 90
Servo servo2; // Continuous: extend() ↔ retract()

int servo1Pin = 2;
int servo2Pin = 3;

bool state1 = false; // servo1: false = 0, true = 90
bool state2 = false;  // servo2: true = extended, false = retracted

const int rotationTime = 400; // ms to spin 360° for servo2

const int emgPin = A1;
const int numSamples = 8;
int emgReadings[numSamples];
int emgIndex = 0;
long total = 0;

void setup() {
  Serial.begin(9600);

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  servo1.write(0);   // initial angle
  servo2.write(96);  // stop signal for continuous servo
}

void loop() {
  // === EMG Moving Average ===
  total -= emgReadings[emgIndex];
  emgReadings[emgIndex] = analogRead(emgPin);
  total += emgReadings[emgIndex];
  emgIndex = (emgIndex + 1) % numSamples;
  int emgAvg = total / numSamples;
  Serial.println(emgAvg);

  // EMG-based control for servo1 (toggle) and servo2 (extend/retract)
  if (emgAvg > 60) {
    int emgValue = analogRead(emgPin);  // Read raw EMG again for spike detection

    // Extend servo2 if not already extended
    if (!state2) {
      extend();
      state2 = true;
    }

    // Toggle servo1 if spike is detected
    if (emgValue > 500) {
      state1 = !state1;
      int angle = state1 ? 90 : 0;
      servo1.write(angle);
      Serial.print("Servo1 toggled by EMG spike. New angle: ");
      Serial.println(angle);
      delay(300); // Debounce delay
    }
  } else {
    // Retract servo2 if not already retracted
    if (state2) {
      retract();
      state2 = false;
    }
  }

  int potValue = analogRead(A0);
  int servoSpeed = map(potValue, 0, 1023, 82, 108);
  if (abs(servoSpeed - 96) < 6) {
    servoSpeed = 96; // Stay still if within deadzone
  }
  servo2.write(servoSpeed);

  delay(70);
}

// === Servo 2 Control ===

void extend() {
  servo2.write(0); // Full speed clockwise
  Serial.println("Extending...");
  delay(rotationTime-5);  // Calibrated
  servo2.write(96); // Stop
}

void retract() {
  servo2.write(180); // Full speed counter-clockwise
  Serial.println("Retracting...");
  delay(rotationTime);
  servo2.write(96); // Stop
}