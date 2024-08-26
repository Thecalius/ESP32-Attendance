
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <MFRC522.h>
#include <map>

// INDICATORS
#define GREENLED_PIN 2
#define REDLED_PIN 16
#define BUZZER_PIN 14

// SET / RESET PINS
#define SDA_PIN 5  // Slave Select (SS) or Chip Select (CS): It selects the RFID reader as the active device of the SPI bus
#define RST_PIN 22 // Bring this LOW to reset the module

// TAGS
const String BLACK_TAG = "5eca5651";
const String GREEN_TAG = "fc2c1fcf";
const String RED_TAG = "6e774161";
const String WHITE_TAG = "9cd1e05d";
const String BLUE_TAG = "8a1a165";

std::map<String, String> tagMessages = {
  {BLACK_TAG, "Hello Black Tag"},
  {GREEN_TAG, "Hello Green Tag"},
  {RED_TAG, "Hello Red Tag"},
  {WHITE_TAG, "Hello White Tag"},
  {BLUE_TAG, "Hello Blue Tag"}
};

const char* ssid = "Silence of the LANs";  // Network's name ( Service Set Identifier)
const char* wifi_password = "fetALEcTOneXHaid"; 
const char* server_name = "http://192.168.2.10:8000/log";

MFRC522 mfrc522(SDA_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  SPI.begin();
  mfrc522.PCD_Init();

  pinMode(GREENLED_PIN, OUTPUT);
  pinMode(REDLED_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(GREENLED_PIN, LOW);
  digitalWrite(REDLED_PIN, LOW);
  tone(BUZZER_PIN, 1000, 500);

  WiFi.begin(ssid, wifi_password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Connected to WiFi...");
  Serial.println("IP Address: ");
  Serial.print(WiFi.localIP());
  Serial.println();

}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String tagID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagID += String(mfrc522.uid.uidByte[i], HEX);
}


Serial.print("NFC Tag UID: ");
Serial.println(tagID);

auto it = tagMessages.find(tagID);
if(it != tagMessages.end()) {
  Serial.println(it->second);
  digitalWrite(GREENLED_PIN, HIGH);
  tone(BUZZER_PIN, 1000, 500);
  delay(500);
  digitalWrite(GREENLED_PIN, LOW);
} else {
  Serial.println("Unknown tag");
  digitalWrite(REDLED_PIN, HIGH);
  tone(BUZZER_PIN, 5000, 500);
  delay(500);
  digitalWrite(REDLED_PIN, LOW);
}

if (WiFi.status() == WL_CONNECTED) {
        HTTPClient http;
        http.begin(server_name);

        http.addHeader("Content-Type", "application/x-www-form-urlencoded");

        String httpRequestData = "uid=" + tagID;
        int httpResponseCode = http.POST(httpRequestData);

        if (httpResponseCode > 0) {
            String response = http.getString();
            Serial.println(httpResponseCode);
            Serial.println(response);
        } else {
            Serial.print("Error on sending POST: ");
            Serial.println(httpResponseCode);
        }

        http.end();
    }

    mfrc522.PICC_HaltA();
}