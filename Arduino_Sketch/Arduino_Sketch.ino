// Uses VR Glove controller temporarily while EMG sensor ships

#include <ArduinoBLE.h>
#include <Servo.h>

Servo servo1; // fires projectile
Servo servo2; // linear actuator

int servo1Pin = 2;
int servo2Pin = 3;

bool state1 = false; // false = 0, true = 90
bool state2 = true;  // true = 180, false = 0 (starts at 180)

bool lastButton1 = false;
bool lastButton2 = false;

BLEDevice peripheral;
BLECharacteristic inputCharacteristic;

void setup() {
  Serial.begin(9600);

  if (!BLE.begin()) {
    Serial.println("Starting BLE failed!");
    while (1);
  }

  BLE.scan(); // Start scanning

  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);

  servo1.write(0);
  servo2.write(180); // default state

  Serial.println("Scanning for peripheral...");
}

void loop() {
  // Connect to peripheral if not connected
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

  // Process incoming value
  if (inputCharacteristic.valueUpdated()) {
    String value = (const char*)inputCharacteristic.value();  // Cast required
    Serial.println("Received: " + value);

    // Extract button values (last 2 entries)
    int lastComma = value.lastIndexOf(',');
    int secondLastComma = value.lastIndexOf(',', lastComma - 1);
    int b1 = value.substring(secondLastComma + 1, lastComma).toInt();
    int b2 = value.substring(lastComma + 1).toInt();

    bool currentButton1 = b1 == 1;
    bool currentButton2 = b2 == 1;

    // Handle Servo 1 (0 ↔ 90)
    if (currentButton1 && !lastButton1) {
      state1 = !state1;
      int angle1 = state1 ? 90 : 0;
      servo1.write(angle1);
      Serial.print("Servo1 set to ");
      Serial.println(angle1);
    }

    // Handle Servo 2 (180 ↔ 0)
    if (currentButton2 && !lastButton2) {
      state2 = !state2;
      int angle2 = state2 ? 180 : 0;
      servo2.write(angle2);
      Serial.print("Servo2 set to ");
      Serial.println(angle2);
    }

    lastButton1 = currentButton1;
    lastButton2 = currentButton2;
  }

  delay(20);
}
