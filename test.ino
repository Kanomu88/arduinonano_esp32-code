#include <WiFi.h>
#include <LoRa.h>
#include <TridentTD_LineNotify.h>

#define SSID        "PREAWZA_2.4G"      // บรรทัดที่ 11 ให้ใส่ ชื่อ Wifi ที่จะเชื่อมต่อ
#define PASSWORD    "P0819157034"     // บรรทัดที่ 12 ใส่ รหัส Wifi
#define LINE_TOKEN  "K4Rw7mlQtH4gr5M70nTPiLGmtnm5o2fW20PpksELu3a"   // บรรทัดที่ 13 ใส่ รหัส TOKEN ที่ได้มาจากข้างบน

#define LORA_TIMEOUT 5000  // 5 seconds timeout for Lora signal

void setup() {
  Serial.begin(115200); Serial.println();
  Serial.println(LINE.getVersion());

  WiFi.begin(SSID, PASSWORD);
  Serial.printf("WiFi connecting to %s\n",  SSID);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(400);
  }
  Serial.printf("\nWiFi connected\nIP : ");
  Serial.println(WiFi.localIP());

  LINE.setToken(LINE_TOKEN);

  LoRa.begin(433E6);
}



void loop() {
  // Check for LoRa messages
  if (LoRa.parsePacket()) {
    // Reset the timeout counter when receiving Lora signal
    unsigned long startTime = millis();

    // Process Lora messages
    String command = LoRa.readStringUntil(',');
    float latitude = LoRa.parseFloat();
    float longitude = LoRa.parseFloat();

    if (command == "01") {
      // Dog is outside the range
      LINE.notify("Dog is outside the range. Last position: Lat: " );
    } 
  

  // Check if Lora signal is lost
  if (millis() - startTime > LORA_TIMEOUT) {
    LINE.notify("Lost Lora signal. Dog may be out of range.");
  }

  delay(1000);
  }
}