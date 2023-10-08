#include <Preferences.h>
#include <BluetoothSerial.h>

const int LED_PIN = 13;
const char* PREF_NAMESPACE = "led_state";
const char* PREF_KEY = "state";

Preferences preferences;
BluetoothSerial SerialBT;

bool currentState = false;

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  
  preferences.begin(PREF_NAMESPACE, false);
  currentState = preferences.getBool(PREF_KEY, false);
  digitalWrite(LED_PIN, currentState ? HIGH : LOW);
  Serial.println(currentState ? "LED is ON" : "LED is OFF");

  SerialBT.begin("ESP32_LED_Control"); // Set the Bluetooth device name

  // Check if there is an existing connection
  if (SerialBT.connected()) {
    Serial.println("Bluetooth connected");
  }
}

void loop() {
  // Check for Bluetooth commands
  if (SerialBT.available()) {
    String command = SerialBT.readStringUntil('\n');
    command.trim();

    if (command == "ON") {
      currentState = true;
      digitalWrite(LED_PIN, LOW);
      preferences.putBool(PREF_KEY, currentState);
      preferences.end();
      Serial.println("LED is ON");
    } else if (command == "OFF") {
      currentState = false;
      digitalWrite(LED_PIN, HIGH);
      preferences.putBool(PREF_KEY, currentState);
      preferences.end();
      Serial.println("LED is OFF");
    }
  }
}
