//#include <ESP8266WiFi.h>
//#include "DHT.h"
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>
#include <ThingSpeak.h>

// ----- Wi-Fi -----
const char* ssid = "YOUR_WIFI_NAME";
const char* password = "YOUR_WIFI_PASSWORD";

// ----- ThingSpeak -----
unsigned long channelID = YOUR_CHANNEL_ID; // e.g., 1234567
const char* writeAPIKey = "YOUR_WRITE_API_KEY";
WiFiClient client;

// ----- Sensor and I/O Pins -----
#define DHTPIN D4
#define DHTTYPE DHT11
#define GAS_PIN A0
#define BUZZER D5
#define GREEN_LED D6
#define RED_LED D7

// ----- OLED Setup -----
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// ----- DHT Sensor -----
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(GAS_PIN, INPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  digitalWrite(BUZZER, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);

  // OLED init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ OLED not found!");
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println("🍱 Food Spoilage System");
  display.display();
  delay(2000);

  // WiFi connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // ThingSpeak init
  ThingSpeak.begin(client);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int gasValue = analogRead(GAS_PIN);

  Serial.println("------------------------------");
  Serial.print("Temp: "); Serial.print(temperature); Serial.println(" °C");
  Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");
  Serial.print("Methane: "); Serial.println(gasValue);

  bool spoiled = (gasValue > 680 || temperature > 35 || humidity > 80);

  // OLED Display
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Temp: "); display.print(temperature); display.println("C");
  display.print("Hum:  "); display.print(humidity); display.println("%");
  display.print("Gas:  "); display.println(gasValue);

  if (spoiled) {
    Serial.println("⚠️ FOOD SPOILAGE DETECTED!");
    display.println("Status: SPOILED!");
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);
    tone(BUZZER, 1000, 500);
  } else {
    Serial.println("✅ Food is Fresh and Safe.");
    display.println("Status: FRESH");
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(RED_LED, LOW);
    noTone(BUZZER);
  }

  display.display();

  // ----- Send to ThingSpeak -----
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, gasValue);
  ThingSpeak.setField(4, spoiled ? 1 : 0);

  int statusCode = ThingSpeak.writeFields(channelID, writeAPIKey);

  if (statusCode == 200) {
    Serial.println("✅ Data sent to ThingSpeak!");
  } else {
    Serial.print("❌ Error sending data: ");
    Serial.println(statusCode);
  }

  delay(20000); // ThingSpeak minimum interval = 15 seconds
}