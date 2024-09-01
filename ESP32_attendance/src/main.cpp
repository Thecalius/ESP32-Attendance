#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <MFRC522.h>
#include <map>
#include <time.h>

// INDICATORS
#define GREENLED_PIN 2
#define REDLED_PIN 16
#define BUZZER_PIN 14

// SET/ RESET PINS
#define SDA_PIN 5  // Slave Select (SS) or Chip Select (CS): It selects the RFID reader as the active device of the SPI bus
#define RST_PIN 22 // Bring this LOW to reset the module

// TAGS
const String BLACK_TAG = "5eca5651";
const String GREEN_TAG = "fc2c1fcf";
const String RED_TAG = "6e774161";
const String WHITE_TAG = "9cd1e05d";
const String BLUE_TAG = "8a1a165";

std::map<String, String> tagMessages = {
  {BLACK_TAG, "Black Tag"},
  {GREEN_TAG, "Green Tag"},
  {RED_TAG, "Red Tag"},
  {WHITE_TAG, "White Tag"},
  {BLUE_TAG, "Blue Tag"}
};

const char* ssid = "Network Name";  // Network's name (Service Set Identifier)
const char* wifi_password = "wifi_password"; 
const char* server_name = "http://192.168.2.10:8000/log";

std::map<String, bool> tagState; // Map to store the state of each tag (logged in or out)

MFRC522 mfrc522(SDA_PIN, RST_PIN);
String currentTime();

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
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (!mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  // Create a string that holds the ID
  String tagID = "";

  // Convert the UID to a Hex string
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    tagID += String(mfrc522.uid.uidByte[i], HEX);
  }

  Serial.print("NFC Tag UID: ");
  Serial.println(tagID);

  bool isLoggedIn = tagState[tagID]; // Check if the tag is currently logged in
  auto it = tagMessages.find(tagID);

  // Toggle the state and log the event
  if (isLoggedIn) {
    // If logged in, log out
    if(it != tagMessages.end()) {
      Serial.print("Logging out: ");
      Serial.println(it->second);
    }else {
      Serial.println("Unknown tag");
    }
    digitalWrite(REDLED_PIN, HIGH);
    tone(BUZZER_PIN, 4000, 100);  // Play a tone for 100ms
    delay(1000);  // Delay to simulate processing
    digitalWrite(REDLED_PIN, LOW);

    // Send log out request to the server
    HTTPClient http;
    http.begin(server_name);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String postData = "uid=" + tagID + "&action=log&out&time=" + currentTime();
    int httpResponseCode = http.POST(postData);
    Serial.println(httpResponseCode); // Print the response code
    http.end();
  } else {
    // If logged out, log in
    if(it != tagMessages.end()) {
      Serial.print("Logging in: ");
      Serial.println(it->second);
    }else {
      Serial.println("Unknown tag");
    }
    digitalWrite(GREENLED_PIN, HIGH);
    tone(BUZZER_PIN, 2000, 100);  // Play a different tone for 100ms
    delay(1000);  // Delay to simulate processing
    digitalWrite(GREENLED_PIN, LOW);

    // Send a log in request to the server
    HTTPClient http;
    http.begin(server_name);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String postData = "uid=" + tagID + "&action=log&in&time=" + currentTime();
    int httpResponseCode = http.POST(postData);
    Serial.println(httpResponseCode);
    http.end();
  }

  // Update the tag's state
  tagState[tagID] = !isLoggedIn;

  /*
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
*/

/*
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
    } 
    else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }
*/

  mfrc522.PICC_HaltA();
}

// Get current time from NTP server
String currentTime() {
  const char* ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = 0; // Adjust for your time zone
  const int daylightOffset_sec = 3600; // Adjust for daylight savings

  // Initialize and sync time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  // Wait for time to be set
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return "N/A";
  }

  // Format time as a string (e.g., "HH:MM:SS YYYY-MM-DD")
  char timeString[30];
  strftime(timeString, sizeof(timeString), "%H:%M:%S %Y-%m-%d", &timeinfo);

  return String(timeString);
}
