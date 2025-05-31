#include <ArduinoBLE.h>
#include <Servo.h>

Servo servo1; // Rotational: toggles 0 ↔ 90
Servo servo2; // Continuous: extend() ↔ retract()

int servo1Pin = 2;
int servo2Pin = 3;

bool state1 = false; // servo1: false = 0, true = 90
bool state2 = false;  // servo2: true = extended, false = retracted

bool lastButton1 = false;
bool lastButton2 = false;

BLEDevice peripheral;
BLECharacteristic inputCharacteristic;

const int rotationTime = 400; // ms to spin 360° for servo2

void setup() {
  Serial.begin(9600);

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  servo1.write(0);   // initial angle
  servo2.write(96);  // stop signal for continuous servo

  delay(200);

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  BLE.scan(); // Start scanning

  Serial.println("Scanning for peripheral...");
}

void loop() {
  // Try to connect to peripheral if not connected
  if (!peripheral || !peripheral.connected()) {
    peripheral = BLE.available();

    if (peripheral && peripheral.localName() == "Hand Peripheral Device") {
      BLE.stopScan();
      Serial.println("Connecting to peripheral...");

      if (peripheral.connect()) {
        Serial.println("Connected!");

        if (peripheral.discoverAttributes()) {
          inputCharacteristic = peripheral.characteristic("d66f7f33-24ae-4dc0-bf22-b9c54f92cdd0");

          if (!inputCharacteristic) {
            Serial.println("Characteristic not found.");
            peripheral.disconnect();
            BLE.scan();
            return;
          }

          inputCharacteristic.subscribe();
        }
      } else {
        Serial.println("Connection failed.");
        BLE.scan();
      }
    }
    return;
  }

  // Handle data from peripheral
  if (inputCharacteristic.valueUpdated()) {
    String value = (const char*)inputCharacteristic.value();
    //Serial.println("Received: " + value);

    // Parse buttons from value
    int lastComma = value.lastIndexOf(',');
    int secondLastComma = value.lastIndexOf(',', lastComma - 1);
    int b1 = value.substring(secondLastComma + 1, lastComma).toInt();
    int b2 = value.substring(lastComma + 1).toInt();

    bool currentButton1 = b1 == 1;
    bool currentButton2 = b2 == 1;

    // === Servo 1 (Rotational) ===
    if (currentButton1 && !lastButton1) {
      state1 = !state1;
      int angle = state1 ? 90 : 0;
      servo1.write(angle);
      Serial.print("Servo1 angle: ");
      Serial.println(angle);
    }

    // === Servo 2 (Continuous Rotation) ===
    if (currentButton2 && !lastButton2) {
      state2 = !state2;
      if (state2) {
        extend();
      } else {
        retract();
      }
    }
    else {
      int potValue = analogRead(A0);
      int servoSpeed = map(potValue, 0, 1023, 82, 108);
      if (abs(servoSpeed - 96) < 6) {
        servoSpeed = 96; // Stay still if within deadzone
      }
      servo2.write(servoSpeed);
    }

    lastButton1 = currentButton1;
    lastButton2 = currentButton2;
  }

  delay(70);
}

// === Servo 2 Control ===

void extend() {
  servo2.write(0); // Full speed clockwise
  Serial.println("Extending...");
  delay(rotationTime);  // Calibrated
  servo2.write(96); // Stop
}

void retract() {
  servo2.write(180); // Full speed counter-clockwise
  Serial.println("Retracting...");
  delay(rotationTime-5);
  servo2.write(96); // Stop
}
