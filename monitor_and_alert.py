import requests
import time
import smtplib
import pytz
from datetime import datetime
from email.mime.text import MIMEText
import matplotlib.pyplot as plt

# --- REPLACE WITH YOUR DETAILS ---
CHANNEL_ID = "Your_Channel_ID" 
READ_API_KEY = "Your_Read_API_Key"
BASE_URL = f"https://api.thingspeak.com/channels/{CHANNEL_ID}/feeds.json?api_key={READ_API_KEY}&results=10"

SENDER_EMAIL = "your_sender_email@gmail.com"
RECEIVER_EMAIL = "your_receiver_email@gmail.com"
EMAIL_PASSWORD = "your_app_password" # Use an App Password from your email provider
# ---------------------------------

def send_email_alert(subject, message):
    msg = MIMEText(message)
    msg["Subject"] = subject
    msg["From"] = SENDER_EMAIL
    msg["To"] = RECEIVER_EMAIL
    
    try:
        with smtplib.SMTP("smtp.gmail.com", 587) as server:
            server.starttls()
            server.login(SENDER_EMAIL, EMAIL_PASSWORD)
            server.sendmail(SENDER_EMAIL, RECEIVER_EMAIL, msg.as_string())
            print("Email alert sent.")
    except Exception as e:
        print(f"Error sending email: {e}")

def log_alert(message):
    with open("alerts_log.txt", "a", encoding="utf-8") as log_file:
        log_file.write(f"{message}\n")

def fetch_data():
    try:
        response = requests.get(BASE_URL)
        if response.status_code == 200:
            data = response.json()
            feeds = data['feeds']
            
            methane, air_quality, temperature, humidity, timestamps = [], [], [], [], []
            ist = pytz.timezone('Asia/Kolkata')
            
            for entry in feeds:
                methane.append(float(entry['field1']))
                air_quality.append(float(entry['field2']))
                temperature.append(float(entry['field3']))
                humidity.append(float(entry['field4']))
                
                created_time = datetime.strptime(entry['created_at'], '%Y-%m-%dT%H:%M:%SZ')
                local_time = created_time.replace(tzinfo=pytz.utc).astimezone(ist)
                timestamps.append(local_time.strftime('%Y-%m-%d %H:%M:%S'))
            
            return methane, air_quality, temperature, humidity, timestamps
    except Exception as e:
        print(f"Error fetching data: {e}")
    return [], [], [], [], []

def plot_live():
    plt.ion() # Enable interactive mode
    
    while True:
        methane, air_quality, temperature, humidity, timestamps = fetch_data()
        
        if not timestamps:
            print("No data fetched, retrying...")
            time.sleep(5)
            continue
        
        plt.clf()
        
        # Methane Plot
        plt.subplot(2, 2, 1)
        plt.plot(timestamps, methane, 'g-o')
        plt.title('Methane (PPM)')
        plt.xticks(rotation=45)
        
        # Air Quality Plot
        plt.subplot(2, 2, 2)
        plt.plot(timestamps, air_quality, 'r-o')
        plt.title('Air Quality (PPM)')
        plt.xticks(rotation=45)
        
        # Temperature Plot
        plt.subplot(2, 2, 3)
        plt.plot(timestamps, temperature, 'b-o')
        plt.title('Temperature (°C)')
        plt.xticks(rotation=45)

        # Humidity Plot
        plt.subplot(2, 2, 4)
        plt.plot(timestamps, humidity, 'm-o')
        plt.title('Humidity (%)')
        plt.xticks(rotation=45)
        
        plt.tight_layout()
        plt.pause(2)
        
        # Check for threshold crossing
        if methane[-1] > 1200 or temperature[-1] > 50 or humidity[-1] > 70:
            alert_message = f"""
            Landfill Safety Alert!
            Methane: {methane[-1]:.2f} PPM
            Air Quality: {air_quality[-1]:.2f} PPM
            Temperature: {temperature[-1]:.2f} C
            Humidity: {humidity[-1]:.2f} %
            Time: {timestamps[-1]}
            Please check immediately!
            """
            send_email_alert("Landfill Environment Alert", alert_message)
            log_alert(f"{timestamps[-1]} - {alert_message.strip()}")
        
        print(f"Updated at: {timestamps[-1]} | Press Ctrl+C to stop.")

if __name__ == "__main__":
    try:
        plot_live()
    except KeyboardInterrupt:
        print("Program stopped.")
