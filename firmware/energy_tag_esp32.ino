#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>

// Define LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define RFID
#define RST_PIN         4        
#define SS_PIN          5        
MFRC522 rfid(SS_PIN, RST_PIN);

// Define WiFi
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
WebServer server(80);

// Define relay
#define RELAY_PIN  2  // Define the relay pin

// Battery variables
#define VOLTAGE_PIN_1   32       
#define VOLTAGE_PIN_2   33       
#define VOLTAGE_PIN_3   27       
#define VOLTAGE_PIN_4   35       

float batteryVoltage = 0.0;
float batteryPercentage = 0.0;
float pricePerPercent = 1.0; // 1 rupee per percent

// Slot booking
String slots[4] = {"", "", "", ""}; // Store user IDs for slots
float initialCharges[4] = {100.0, 100.0, 100.0, 100.0}; // Assuming initial charge was 100%

// Forward declarations
void displayMessage(const char* line1, const char* line2, int delayMs, bool turnOffRelay = false);

void setup() {
  Serial.begin(115200);

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  displayMessage("Battery Swap", "", 2000);

  // Initialize RFID
  SPI.begin();
  rfid.PCD_Init();

  // Initialize WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Display IP address on LCD
  displayMessage("IP Address:", WiFi.localIP().toString().c_str(), 10000);

  // Setup relay
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH); // Relay is initially on

  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/book", HTTP_POST, handleBooking);
  server.begin();
}

void loop() {
  // Read RFID
  if (rfid.PICC_IsNewCardPresent() && rfid.PICC_ReadCardSerial()) {
    handleRFID();
  }

  // Handle web server
  server.handleClient();
}

void handleRFID() {
  String userID = "";
  digitalWrite(RELAY_PIN, LOW);
  for (byte i = 0; i < rfid.uid.size; i++) {
    userID += String(rfid.uid.uidByte[i] < 0x10 ? "0" : "");
    userID += String(rfid.uid.uidByte[i], HEX);
  }
  userID.toUpperCase();
  rfid.PICC_HaltA();

  String userName = "User_" + userID; // Assuming user name is generated from user ID

  // Check if the user already booked a slot
  for (int i = 0; i < 4; i++) {
    if (slots[i] == userID) {
      // Process return
      measureBattery(i);
      float price = batteryPercentage * pricePerPercent;
      refundAmount(initialCharges[i], batteryPercentage);
      float refund = initialCharges[i] - batteryPercentage;
      slots[i] = ""; // Make the slot available

      // Update Google Sheet
      updateGoogleSheet(userName, userID, i, batteryPercentage, price, refund);

      displayMessage("Return Success", ("Refund: Rs" + String(price)).c_str(), 5000, true);
      displayMessage("Battery Swap", "", 2000);
      return;
      digitalWrite(RELAY_PIN, HIGH);
    }
  }

  // Book a new slot if available
  for (int i = 0; i < 4; i++) {
    if (slots[i] == "") {
      slots[i] = userID;
      measureBattery(i);
      initialCharges[i] = batteryPercentage; // Store the initial charge
      float price = batteryPercentage * pricePerPercent;

      // Update Google Sheet
      updateGoogleSheet(userName, userID, i, batteryPercentage, price, 0);

      String message1 = "User_" + userID;
      String message2 = "Ch:" + String(batteryPercentage) + "% Rs:" + String(price);
      displayMessage(message1.c_str(), message2.c_str(), 5000, true);
      displayMessage("Battery Swap", "", 2000);
      return;
    }
  }

  // If no slots are available
  displayMessage("No Slots Avail", "", 5000);
  displayMessage("Battery Swap", "", 2000);
}

void handleRoot() {
  String html = "<html><head><title>Battery Swap Centre</title></head><body>";
  html += "<h1>Battery Swap Centre</h1>";
  for (int i = 0; i < 4; i++) {
    html += "<h2>Slot " + String(i+1) + ": ";
    if (slots[i] == "") {
      html += "Available</h2>";
      html += "<form action=\"/book\" method=\"POST\">";
      html += "Name: <input type=\"text\" name=\"name\"><br>";
      html += "<input type=\"hidden\" name=\"slot\" value=\"" + String(i) + "\">";
      html += "<input type=\"submit\" value=\"Book\">";
      html += "</form>";
    } else {
      html += "Booked</h2>";
    }
  }
  html += "</body></html>";
  server.send(200, "text/html", html);
}

void handleBooking() {
  if (server.method() == HTTP_POST) {
    String name = server.arg("name");
    int slot = server.arg("slot").toInt();
    if (slots[slot] == "") {
      slots[slot] = name;
    }
    server.sendHeader("Location", "/");
    server.send(303);
  }
}

void measureBattery(int slot) {
  int voltagePin;
  switch (slot) {
    case 0:
      voltagePin = VOLTAGE_PIN_1;
      break;
    case 1:
      voltagePin = VOLTAGE_PIN_2;
      break;
    case 2:
      voltagePin = VOLTAGE_PIN_3;
      break;
    case 3:
      voltagePin = VOLTAGE_PIN_4;
      break;
  }
   digitalWrite(RELAY_PIN, LOW); 
  // Read the analog value from the voltage pin
  int adc_value = analogRead(voltagePin);

  // Calculate the ADC voltage
  float adc_voltage = (adc_value * 3.3) / 4095.0;

  // Resistor values in the voltage divider
  float R1 = 30000.0;
  float R2 = 7500.0;

  // Calculate the input voltage at the divider
  float in_voltage = adc_voltage / (R2 / (R1 + R2));

  // Calculate the battery percentage
  batteryVoltage = in_voltage; // Set the battery voltage to the calculated input voltage
  batteryPercentage = (batteryVoltage / 8) * 100;
  digitalWrite(RELAY_PIN, HIGH);
}

void refundAmount(float initialCharge, float currentCharge) {
  digitalWrite(RELAY_PIN, LOW);
  float usedPercentage = initialCharge - currentCharge;
  float refund = usedPercentage * pricePerPercent;
  
   // Turn off relay
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Refund: Rs");
  lcd.print(refund);
  delay(5000); // Display refund amount for 5 seconds
  
  digitalWrite(RELAY_PIN, HIGH); // Turn on relay
}

void displayMessage(const char* line1, const char* line2, int delayMs, bool turnOffRelay) {
  if (turnOffRelay) {
    digitalWrite(RELAY_PIN, LOW); // Turn off relay
  }
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  delay(delayMs);
  
  if (turnOffRelay) {
    digitalWrite(RELAY_PIN, HIGH); // Turn on relay after displaying the message
  }
}

void updateGoogleSheet(String userName, String userID, int slotNumber, float batteryCharge, float price, float refund) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin("[https://script.google.com/macros/s/YOUR_SCRIPT_ID/exec](https://script.google.com/macros/s/YOUR_SCRIPT_ID/exec)"); // Replace with your Google Apps Script Web App URL
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"userName\":\"" + userName + "\",\"userID\":\"" + userID + "\",\"slotNumber\":" + String(slotNumber) + ",\"batteryCharge\":" + String(batteryCharge) + ",\"price\":" + String(price) + ",\"refund\":" + String(refund) + "}";

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      Serial.println("Data sent to Google Sheet");
    } else {
      Serial.println("Error sending data to Google Sheet");
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
  }
}
