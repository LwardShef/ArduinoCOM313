#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h> // bc it would be more complicated to use https

#include "driver/temp_sensor.h" // driver to read CPU core temp

// LED SETUP
const int red = 9;
const int yellow = 10;
const int green = 11;
const int button = 5;

// WIFI SETUP
const char* ssid = "TNCAP1E424B";
const char* password = "CmtkkYCnf42rJqMc";
const char* host = ""; // where we push these post requests to
const int httpPort = 80;

// LOOP FUNCTIONS
void sendSensorData(); // could change the return types
void getSensorData();

void receiveRequests(); // poll for light changes - something that requires an interrupt of some kind?

void changeLights();


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
  Serial.printf("\nConnected to Wifi network %s at IP %s\n", WiFi.SSID(), WiFi.localIP());

  // LOOK THRU WIFI, FIND WEB SERVER AND CONNECT
  // PUT INTO ITS OWN FUNCTION AS NEEDS TO BE RERUN IF CONNECTION LOST
}

void loop() {

  // hwefoofsjiojgwg ill set up http connections and breaking all this code out into separate fns later
}

void sendSensorData() {
  // for this we need to define what we are sending
}
void getSensorData() {
  // CHECK IF BUTTON IS LOW OR HIGH

  // CODE TO GET A CPU TEMP SENSOR READING
  float cpuTemp = 0;
  temp_sensor_read_celsius(&cpuTemp);
  delete(&cpuTemp); // dont think this is required
}

void receiveRequests() {

}

void changeLights() {

}
