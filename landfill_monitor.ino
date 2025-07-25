#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>
#include <Adafruit_SSD1306.h>

#define DHTPIN 4
#define DHTTYPE DHT11
#define MQ4 A0
#define MQ135 A1
#define BUZZER 13

// --- REPLACE WITH YOUR DETAILS ---
const char* ssid = "YourSSID";
const char* password = "YourPassword";
const char* server = "http://api.thingspeak.com/update?api_key=YOUR_API_KEY";
// ---------------------------------

DHT dht(DHTPIN, DHTTYPE);
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);
  pinMode(BUZZER, OUTPUT);
  
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

  WiFi.begin(ssid, password);
  // Note: Add logic here to wait for WiFi connection if needed
}

void loop() {
  // Read sensor values
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  int mq4_val = analogRead(MQ4);
  int mq135_val = analogRead(MQ135);

  // Display data on OLED
  display.clearDisplay();
  display.setCursor(0,0);
  display.print("Temp: "); display.println(temp);
  display.print("Hum: "); display.println(hum);
  display.print("CH4: "); display.println(mq4_val);
  display.print("CO2: "); display.println(mq135_val);
  display.display();

  // Check methane threshold for local buzzer alert
  if(mq4_val > 400) {
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(BUZZER, LOW);
  }

  // Send data to ThingSpeak
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String url = String(server) + "&field1=" + String(mq4_val) + "&field2=" + String(mq135_val) + "&field3=" + String(temp) + "&field4=" + String(hum);
    
    http.begin(url);
    http.GET();
    http.end();
  }
  
  delay(5000); // Wait 5 seconds before next loop
}
