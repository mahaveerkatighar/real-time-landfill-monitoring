# Real-Time Landfill Methane & Environmental Monitoring

This project is a real-time environmental monitoring system designed for landfill sites. It utilizes an ESP32 microcontroller and Python to create a practical IoT-based solution for pollution control and early intervention.

## Abstract

This project focuses on real-time environmental monitoring in landfill sites using the ESP32 microcontroller and Python programming. The system is designed to measure methane concentration using the MQ-4 sensor, carbon dioxide and other harmful gases with the MQ-135 sensor, and ambient conditions like temperature and humidity using the DHT11 sensor. All collected data is transmitted via Wi-Fi to the ThingSpeak cloud platform. A Python application then accesses this data for real-time analysis and triggers automated email alerts using SMTP. The project effectively demonstrates the integration of embedded systems and Python to build impactful IoT solutions for environmental monitoring.

## Authors

* Mahaveer Katighar
* Arnesh Chauhan
* Akshay Anthannagari

## System Architecture and Workflow

The system operates by collecting sensor data, displaying it locally, sending it to the cloud, and then using a Python script for remote monitoring and alerting.

![Block Diagram of the Monitoring System](image/Block%20Diagram.png)

### Working Principle

1.  **Data Acquisition**: The ESP32 microcontroller reads analog values from the MQ-4 (Methane) and MQ-135 (CO₂) sensors, and digital data from the DHT11 (Temperature & Humidity) sensor.
2.  **Local Display & Alert**: The OLED screen displays current readings. A buzzer is triggered locally if the methane level is high.
3.  **Cloud Transmission**: The collected data is formatted and sent to a ThingSpeak channel every few seconds via an HTTP POST request.
4.  **Remote Monitoring & Alerts**: A Python script fetches the data from ThingSpeak using an HTTP GET request. If any value exceeds a safe threshold, an email alert is automatically sent using the SMTP protocol.

## Hardware & Software

### Hardware Components

* ESP-32 Microcontroller
* MQ-4 Methane Gas Sensor
* MQ-135 CO₂ Sensor
* DHT11 Temperature & Humidity Sensor
* OLED 0.96 Inch Display
* Buzzer

### Software, Platforms, and Libraries

* **IoT Platform**: ThingSpeak
* **ESP32 Code**: Arduino C++
* **Monitoring Script**: Python
* **Python Libraries**: `requests`, `time`, `matplotlib.pyplot`, `pytz`, `smtplib`
* **Communication**: HTTP, SMTP Protocol

## Setup and Installation

### 1. ESP32 Setup

1.  Open the `.ino` file in the Arduino IDE.
2.  Ensure you have the necessary libraries installed (`WiFi`, `HTTPClient`, `DHT`, `Adafruit_SSD1306`).
3.  Modify the code with your specific credentials:
    ```cpp
    const char* ssid = "YourSSID"; // Your WiFi network name
    const char* password  "YourPassword"; // Your WiFi password
    const char* server "[http://api.thingspeak.com/update?api_key=YOUR_API_KEY](http://api.thingspeak.com/update?api_key=YOUR_API_KEY)"; // Your ThingSpeak Write API Key
    ```
4.  Connect the ESP32, select the correct board and port, and upload the sketch.

### 2. Python Script Setup

1.  Install the required Python packages:
    ```bash
    pip install requests matplotlib pytz
    ```
2.  Open the Python script and update the following variables with your details:
    ```python
    # ThingSpeak details
    CHANNEL_ID = "Your_Channel_ID" 
    READ_API_KEY= "Your_Read_API_Key"
    
    # Email setup
    SENDER_EMAIL = "your_sender_email@gmail.com"
    RECEIVER_EMAIL = "your_receiver_email@gmail.com"
    EMAIL_PASSWORD = "your_app_password" 
    ```
3.  Run the script from your terminal to begin monitoring:
    ```bash
    python your_script_name.py
    ```

## Project Features

* Real-time display of Methane, CO₂, Temperature, and Humidity on an OLED screen.
* Live data logging and visualization on a ThingSpeak channel.
* An automatic buzzer sounds an alarm when methane levels cross the threshold.
* The Python script provides console output and sends email alerts for hazardous gas detection.

## Conclusion

This project successfully integrates an ESP32 with Python programming to monitor landfill gases and environmental conditions in real time. It serves as a cost-effective, scalable, and educational model for IoT-based environmental monitoring. Furthermore, it demonstrates the practical application of Python for handling HTTP requests, data parsing, and email automation.
