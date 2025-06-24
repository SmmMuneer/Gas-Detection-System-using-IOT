#define BLYNK_TEMPLATE_ID "TMPL6gYFp5zW8"    // Replace with your Template ID from Blynk
#define BLYNK_TEMPLATE_NAME "Gas Detection System" // Replace with your Template Name from Blynk

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//----- Network Configuration -----//
char auth[] = "Uo-OR_CWWUXrZove8SSOclG5XVIN8sBh";     // Your Blynk Auth Token
char ssid[] = "Galaxy A52 753E";             // Your WiFi SSID
char pass[] = "33333333";         // Your WiFi password

//----- Pin Definitions -----//
const int mq2Pin = A0;       // MQ2 analog sensor on A0 pin
const int buzzerPin = D1;     // Buzzer on digital pin D1/GPIO5
const int ledPin = D2;      // Red LED on digital pin D2/GPIO4
const int ledPin2 = D3;      // Second Red LED on digital pin D3/GPIO0

//----- Calibration Parameters ----//
const int threshold = 800;  // Threshold for gas detection
int sensorValue = 0;
int oldSensorValue = -1000; // Track the previous sensor value
int sensorChangeThreshold = 3; // Update only if sensor value changes by more than 3
bool alarmActive = false;  // Variable to track if alarm is active
unsigned long lastGaugeUpdate = 0;          // Last time the gauge was updated
const unsigned long gaugeUpdateInterval = 3000; // Update gauge every 3 seconds
unsigned long lastLogEvent = 0;             // Last time a log event was sent
const unsigned long logEventInterval = 15000; // Log gas events every 15 seconds
unsigned long lastBlinkTime = 0;            // Last time the LED and buzzer blinked
const unsigned long blinkInterval = 500;    // Blink interval for LEDs and buzzer
bool blinkState = false;                    // State for blinking
unsigned long detectionStartTime = 0;       // Time when gas is detected
unsigned long alertTriggeredTime = 0;       // Time when alarm is activated

void setup() {
  
  Serial.begin(115200);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin2, OUTPUT); // Set the second LED pin to an OUTPUT
  digitalWrite(buzzerPin, LOW); // Start with buzzer off
  digitalWrite(ledPin, LOW);    // Start with LED off
  digitalWrite(ledPin2, LOW);   // Start with the second LED off

  Blynk.begin(auth, ssid, pass);
}

void loop() {
  Blynk.run();
  sensorValue = analogRead(mq2Pin);

  // Real-Time Gauge Updates: Send to Blynk every 3 seconds if value changed by more than 3
  if ((millis() - lastGaugeUpdate >= gaugeUpdateInterval) &&  // Update every 3 seconds
      (abs(sensorValue - oldSensorValue) > sensorChangeThreshold)) {  // Only if the value changes by more than 3
    Blynk.virtualWrite(V0, sensorValue); // Send sensor value to Blynk on Virtual Pin V0
    oldSensorValue = sensorValue; // Update the previous value
    lastGaugeUpdate = millis();   // Update the last update time
    Serial.print("Gauge Updated with Value: ");
    Serial.println(sensorValue);
  }

  if (sensorValue > threshold) {
    // Gas Detected, Activate Alert
    if (!alarmActive) {
      detectionStartTime = millis();  // Capture time when gas is detected
      Serial.println("Gas Detected!");
      alarmActive = true;  // Set alarm to active when threshold is reached

      // Log gas detection event to Blynk (if enough time has passed)
      if (millis() - lastLogEvent >= logEventInterval) {
        Blynk.logEvent("gas_detected", "Gas leak detected!");
        lastLogEvent = millis(); // Update last log event time
        Serial.println("Gas Leak Logged to Blynk.");
      }

      alertTriggeredTime = millis();  // Capture time when alarm is triggered
      unsigned long responseTime = alertTriggeredTime - detectionStartTime;
      Serial.print("Response Time: ");
      Serial.print(responseTime);
      Serial.println(" ms");
    }

    // Blink LEDs and Buzzer locally
    if (millis() - lastBlinkTime >= blinkInterval) {
      blinkState = !blinkState; // Toggle blink state
      digitalWrite(buzzerPin, blinkState);
      digitalWrite(ledPin, blinkState);
      digitalWrite(ledPin2, blinkState);
      lastBlinkTime = millis(); // Update last blink time
    }
  } else {
    // No Gas Detected
    if (alarmActive) {
      Serial.println("No Gas Detected, Alert OFF.");
    }
    alarmActive = false; // Set alarm to inactive
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    digitalWrite(ledPin2, LOW);
  }

  delay(100); // Small delay to reduce loop overloading
}
