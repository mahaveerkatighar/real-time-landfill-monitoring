#include <WiFi.h> [cite: 83]
#include <HTTPClient.h> [cite: 85]
#include <DHT.h> [cite: 87]
#include <Adafruit_SSD1306.h> [cite: 89]

#define DHTPIN 4 [cite: 92]
#define DHTTYPE DHT11 [cite: 94]
#define MQ4 A0 [cite: 96]
#define MQ135 A1 [cite: 98]
#define BUZZER 13 [cite: 100]

// --- REPLACE WITH YOUR DETAILS ---
const char* ssid = "YourSSID"; [cite: 104]
const char* password = "YourPassword"; [cite: 106]
const char* server = "http://api.thingspeak.com/update?api_key=YOUR_API_KEY"; [cite: 108]
// ---------------------------------

DHT dht(DHTPIN, DHTTYPE); [cite: 111]
Adafruit_SSD1306 display(128, 64, &Wire, -1); [cite: 118]

void setup() {
  Serial.begin(115200); [cite: 116]
  pinMode(BUZZER, OUTPUT); [cite: 124]
  
  dht.begin(); [cite: 122]
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); [cite: 126]

  WiFi.begin(ssid, password); [cite: 120]
  // Note: Add logic here to wait for WiFi connection if needed
}

void loop() {
  // Read sensor values
  float temp = dht.readTemperature(); [cite: 134]
  float hum = dht.readHumidity(); [cite: 135]
  int mq4_val = analogRead(MQ4); [cite: 137]
  int mq135_val = analogRead(MQ135); [cite: 139]

  // Display data on OLED
  display.clearDisplay(); [cite: 143]
  display.setCursor(0,0); [cite: 144]
  display.print("Temp: "); display.println(temp); [cite: 146]
  display.print("Hum: "); display.println(hum); [cite: 148]
  display.print("CH4: "); display.println(mq4_val); [cite: 149]
  display.print("CO2: "); display.println(mq135_val); [cite: 149]
  display.display(); [cite: 150]

  // Check methane threshold for local buzzer alert
  if(mq4_val > 400) { [cite: 151]
    digitalWrite(BUZZER, HIGH); [cite: 151]
  } else {
    digitalWrite(BUZZER, LOW); [cite: 160]
  }

  // Send data to ThingSpeak
  if(WiFi.status() == WL_CONNECTED) { [cite: 152]
    HTTPClient http; [cite: 175]
    String url = String(server) + "&field1=" + String(mq4_val) + "&field2=" + String(mq135_val) + "&field3=" + String(temp) + "&field4=" + String(hum); [cite: 176]
    
    http.begin(url); [cite: 177]
    http.GET(); [cite: 167]
    http.end(); [cite: 169]
  }
  
  delay(5000); // Wait 5 seconds before next loop
}
