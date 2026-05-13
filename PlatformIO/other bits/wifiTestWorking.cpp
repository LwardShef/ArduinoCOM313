#include <WiFi.h>
//#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>

// Replace with your network credentials
const char* ssid = "TNCAP1E424B";
const char* password = "CmtkkYCnf42rJqMc";
const char* serverAddress = "http://192.168.1.212:5000/data";

const int YELLOW_PIN = 10;
const int GREEN_PIN = 11;
const int RED_PIN = 12;

void initWIFI(){
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi..");
    }
    // Print ESP Local IP Address
    Serial.println(WiFi.localIP());
}


void setup() {
    Serial.begin(115200);
    Serial.println("setup");
    initWIFI();
}

void loop() {
  // put your main code here, to run repeatedly:
    float test = 200.2;
     JsonDocument jsonDoc;
     jsonDoc["test"] = test;

    HTTPClient http;
    Serial.println(serverAddress);
    http.begin(serverAddress);

    http.addHeader("Content-Type", "application/json");
    String payload;
    serializeJson(jsonDoc, payload);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
    Serial.printf("HTTP Response code: %d\n", httpResponseCode);
    String response = http.getString();
    Serial.println(response);
    } else {
    Serial.printf("HTTP Request failed: %s\n", http.errorToString(httpResponseCode).c_str());
    }

    http.end();

    delay(5000);
}
