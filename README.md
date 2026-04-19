# FreshGuard - IoT Smart Container for Food Spoilage Detection System

This project is an IoT-based smart container system that monitors food freshness using sensors and sends data to ThingSpeak.

## Features
- Temperature & Humidity monitoring (DHT11)
- Gas detection (MQ sensor)
- OLED display (real-time data)
- Buzzer alert for spoilage
- Cloud monitoring via ThingSpeak

## Hardware Used
- ESP8266 (NodeMCU)
- DHT11 Sensor
- MQ Gas Sensor
- OLED Display (SSD1306)
- Buzzer & LEDs

## Setup
1. Create `config.h` file with your credentials
2. Upload code to ESP8266
3. Open Serial Monitor (115200 baud)

## Output
- Real-time data on OLED
- Alerts when food is spoiled
- Cloud graphs on ThingSpeak
