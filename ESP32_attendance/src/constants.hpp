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

const size_t MAX_CONNECT_ATTEMPTS = 20;

bool connected = False;