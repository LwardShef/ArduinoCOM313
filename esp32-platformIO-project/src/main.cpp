#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h> // bc it would be more complicated to use https

#include "driver/temp_sensor.h" // driver to read CPU core temp

// LED SETUP CONSTS
const int red = 9;
const int yellow = 10;
const int green = 11;
const int button = 5;

// WIFI SETUP CONSTS
const char* ssid = "TNCAP1E424B";
const char* password = "CmtkkYCnf42rJqMc";
const char* host = ""; // where we push these post requests to
const int httpPort = 80;


void setup() {
  Serial.begin(115200);

  // LED SETUP
  pinMode(button, INPUT_PULLUP);
  pinMode(red, OUTPUT);
  pinMode(yellow, OUTPUT);
  pinMode(green, OUTPUT);

  // CPU TEMP SENSOR SETUP
  temp_sensor_config_t cpuTempSensor = TSENS_CONFIG_DEFAULT();
  temp_sensor_set_config(cpuTempSensor);
  temp_sensor_start();

  // WIFI CONNECTION
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { // print till connected
    delay(800);
    Serial.print(".");
  }
  Serial.printf("\nConnected to Wifi network %s\n", WiFi.SSID());
}

void loop() {
  // CODE TO GET A CPU TEMP SENSOR READING
  float cpuTemp = 0;
  temp_sensor_read_celsius(&cpuTemp);
  delete(&cpuTemp); // dont think this is required

  // hwefoofsjiojgwg ill set up http connections and breaking all this code out into separate fns later
}

