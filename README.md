# Gas-Detection-System-using-IOT
An IoT-based gas leakage detection system using NodeMCU ESP8266, MQ2 sensor, and Blynk platform for real-time monitoring and alerting.

## 🔧 Hardware Components
- NodeMCU ESP8266
- MQ2 Gas Sensor
- 2x Red LEDs
- Buzzer
- Jumper Wires
- Breadboard
- Wi-Fi connection (Hotspot/Router)

## 🌐 Software Used
- Arduino IDE
- Blynk IoT Platform
- ESP8266 Board Manager
- GitHub (version control)

## 💡 Features
- Detects gas levels using MQ2 sensor
- Sends real-time readings to Blynk every 3 seconds (only if value changes)
- Triggers local buzzer and LED blinking if gas level exceeds threshold
- Sends gas leak alerts via Blynk log event
- Optimized to avoid reaching Blynk free plan limit

## 📊 Sensor Logic
- **Threshold:** 750 (analog value)
- **Gauge update:** Every 3 seconds if change > ±3
- **Log event:** Once every 15 seconds if gas leak persists
- **Blink interval:** 500ms (buzzer + LEDs)

## 🧠 How It Works
When gas concentration crosses the safe threshold, the system:
1. Blinks LEDs and buzzer
2. Sends event notification to Blynk app
3. Updates gas readings on Blynk dashboard

## 📝 Setup Instructions
1. Open Arduino IDE and install ESP8266 board via board manager
2. Add required libraries:
   - `ESP8266WiFi`
   - `BlynkSimpleEsp8266`
3. Replace Wi-Fi credentials and Blynk Auth token in the `.ino` file
4. Upload code to NodeMCU
5. Open Serial Monitor to view live sensor values
