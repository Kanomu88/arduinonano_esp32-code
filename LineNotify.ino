#include <TinyGPS++.h>
#include <LoRa.h>

TinyGPSPlus gps;
#define GPS_SERIAL Serial1

#define WAKE_INTERVAL_SECONDS 1  // 1 second wake interval
#define DEEP_SLEEP_INTERVAL_SECONDS 60  // Deep sleep for 60 seconds

void setup() {
  Serial.begin(9600);
  GPS_SERIAL.begin(9600);
  LoRa.begin(433E6);
}

void loop() {
  // Ensure GPS module is on
  GPS_SERIAL.begin(9600);

  // Allow the GPS module to stabilize (can take a few seconds)
  delay(2000);

  for (int i = 0; i < WAKE_INTERVAL_SECONDS; ++i) {
    // Get GPS data
    while (GPS_SERIAL.available() > 0) {
      if (gps.encode(GPS_SERIAL.read())) {
        if (gps.location.isValid()) {
          float latitude = gps.location.lat();
          float longitude = gps.location.lng();
          float homeLatitude = 00.1;
          float homeLongitude = 00.5;
          float distance = TinyGPSPlus::distanceBetween(latitude, longitude, homeLatitude, homeLongitude);

          if (distance > 100.0) {
          // Dog is outside the range
          sendMessageToBase("01", latitude, longitude);
        }
      }
    }
  }
    // Delay for 1 second
    delay(1000);
  }

  // Turn off GPS module
  GPS_SERIAL.end();

  // Go to deep sleep
  delay(1000);
  ESP.deepSleep(DEEP_SLEEP_INTERVAL_SECONDS * 1e6);
}

void sendMessageToBase(String command, float latitude, float longitude) {
  // Send LoRa message to ESP32 in the house
  LoRa.beginPacket();
  LoRa.print(command);
  LoRa.print(",");
  LoRa.print(latitude, 6);
  LoRa.print(",");
  LoRa.print(longitude, 6);
  LoRa.endPacket();
}
