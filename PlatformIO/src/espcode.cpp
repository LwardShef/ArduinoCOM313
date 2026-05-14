#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>

// Wifi network and server credentials
const char* ssid = "TNCAP1E424B";
const char* password = "CmtkkYCnf42rJqMc";
// Address of the route to send data to
const char* serverSendAddress = "http://192.168.1.212:5000/send_data";
// Address of the route to get data from
const char* serverGetAddress = "http://192.168.1.212:5000/get_data";

void initWIFI(){
    // Connect to Wi-Fi
    WiFi.begin(ssid, password);
    Serial.println("Beginning Connection:");
    // Print string every second until connected
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    // Print ESP Local IP Address
    Serial.println(WiFi.localIP());
}

// LEDs and button definitions
const int YELLOW_PIN = 10;
const int GREEN_PIN = 11;
const int RED_PIN = 12;
bool ledBuffer[] = {0, 0, 0}; //stores values of LEDs in memory
const int BUTTON_PIN = 13;

// Timing variables
const int loopTime = 10; //How often the loop() function runs
const int updateFreq = 2000; //How often the esp32 updates from http
const int debounce = 100; //Delay on the button triggering updates, avoids detecting one button press as multiple
int timer = 0;


// Patterns defined as enums for more robust assignment
enum Pattern {SolidYellow, SolidGreen, SolidRed, Rainbow, Chase, Flame, Off};
Pattern pattern = Off;

// STATIC PATTERNS

void patternSolidYellow(){
    ledBuffer[0] = 1;
    ledBuffer[1] = 0;
    ledBuffer[2] = 0;
}

void patternSolidGreen(){
    ledBuffer[0] = 0;
    ledBuffer[1] = 1;
    ledBuffer[2] = 0;
}

void patternSolidRed(){
    ledBuffer[0] = 0;
    ledBuffer[1] = 0;
    ledBuffer[2] = 1;
}

void patternOff(){
    ledBuffer[0] = 0;
    ledBuffer[1] = 0;
    ledBuffer[2] = 0;
}

// DYNAMIC PATTERNS

void patternRainbow(){
    int numPhases = 6;
    int ticksPerIteration = updateFreq / numPhases;
    switch(timer / ticksPerIteration) {
        case 0:
            ledBuffer[0] = 1;
            ledBuffer[1] = 0;
            ledBuffer[2] = 0;
            break;
        case 1:
            ledBuffer[0] = 1;
            ledBuffer[1] = 1;
            ledBuffer[2] = 0;
            break;
        case 2:
            ledBuffer[0] = 1;
            ledBuffer[1] = 1;
            ledBuffer[2] = 1;
            break;
        case 3:
            ledBuffer[0] = 0;
            ledBuffer[1] = 1;
            ledBuffer[2] = 1;
            break;
        case 4:
            ledBuffer[0] = 0;
            ledBuffer[1] = 0;
            ledBuffer[2] = 1;
            break;
        default:
            ledBuffer[0] = 0;
            ledBuffer[1] = 0;
            ledBuffer[2] = 0;
            break;
    }
}

void patternChase(){
    int numPhases = 3;
    int ticksPerIteration = updateFreq / numPhases;
    switch(timer / ticksPerIteration) {
        case 0:
            ledBuffer[0] = 1;
            ledBuffer[1] = 0;
            ledBuffer[2] = 0;
            break;
        case 1:
            ledBuffer[0] = 0;
            ledBuffer[1] = 1;
            ledBuffer[2] = 0;
            break;
        default:
            ledBuffer[0] = 0;
            ledBuffer[1] = 0;
            ledBuffer[2] = 1;
            break;
    }
}

// flickering pattern based on randomly switching the LEDs every so often
void patternFlame(){
    if (rand() % 40 == 1) {
        ledBuffer[0] = !ledBuffer[0];
    }
    if (rand() % 25 == 1) {
        ledBuffer[1] = !ledBuffer[1];
    }
    if (rand() % 60 == 1) {
        ledBuffer[2] = !ledBuffer[2];
    }
}

// PATTERN ENUM-STRING BOILERPLATE
// Converts a Pattern to a corresponding String value
String patternToString(Pattern p){
    switch(p){
        case SolidYellow: return "SolidYellow"; 
        case SolidGreen : return "SolidGreen"; 
        case SolidRed   : return "SolidRed"; 
        case Rainbow    : return "Rainbow";
        case Chase      : return "Chase";
        case Flame      : return "Flame";
        case Off        : return "Off";
        default: return "Off";
    }
}

// Converts a string to the corresponding Pattern value
Pattern stringToPattern(String p){
    if (p == "SolidYellow"){
        return SolidYellow;
    }
    else if (p == "SolidGreen"){
        return SolidGreen;
    }
    else if (p == "SolidRed"){
        return SolidRed;
    }
    else if (p == "Rainbow"){
        return Rainbow;
    }
    else if (p == "Chase"){
        return Chase;
    }
    else if (p == "Flame"){
        return Flame;
    }
    else if (p == "Off"){
        return Off;
    }
    else{
        return Off;
    }
}

// Cycles to the next pattern in the sequence, triggered on button press
Pattern switchPattern(Pattern p){
    switch(p){
        case Off        : return SolidGreen; break;
        case SolidGreen : return SolidYellow; break;
        case SolidYellow: return SolidRed; break;
        case SolidRed   : return Rainbow; break;
        case Rainbow    : return Chase; break;
        case Chase      : return Flame; break;
        case Flame      : return Off; break;
        default: return Off; break;
    }
}

void applyPattern(){
    switch(pattern){
        case SolidYellow: patternSolidYellow(); break;
        case SolidGreen : patternSolidGreen(); break;
        case SolidRed   : patternSolidRed(); break;
        case Rainbow    : patternRainbow(); break;
        case Chase      : patternChase(); break;
        case Flame      : patternFlame(); break;
        case Off        : patternOff(); break;
    }
}

// Writing the contents of the LED buffer to the light pins
void changeLights() {
    digitalWrite(YELLOW_PIN, ledBuffer[0]);
    digitalWrite(GREEN_PIN, ledBuffer[1]);
    digitalWrite(RED_PIN, ledBuffer[2]);
}

// Requests data from Flask server. This tells the ESP32 if the pattern has been changed on the web page
void getData(){
    //Defines JSON variable to store data recieved
    JsonDocument data;
    //Begins HTTP communication with server
    HTTPClient http;
    http.begin(serverGetAddress);
    http.addHeader("Content-Type", "application/json");
    //Makes the GET request to the server
    int httpResponseCode = http.GET();
    if (httpResponseCode > 0) {
        //Unless connection fails, print the response code
        Serial.printf("HTTP Response code on get : %d\n", httpResponseCode);
        String response = http.getString();
        deserializeJson(data, response);
        pattern = stringToPattern(data["Pattern"]);
        //Serial.println(response);
    } else {
        Serial.printf("HTTP Request failed on get : %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();
}

// sending data about CPU temp, current pattern and whether the button is currently pressed
void sendData(){
    float temperature = temperatureRead();
    JsonDocument jsonDoc;
    jsonDoc["Pattern"] = patternToString(pattern);
    jsonDoc["Temperature"] = temperature;
    jsonDoc["ButtonPressed"] = digitalRead(BUTTON_PIN);

    HTTPClient http;
    http.begin(serverSendAddress);
        http.addHeader("Content-Type", "application/json");
        String payload;
        serializeJson(jsonDoc, payload);
        http.addHeader("Content-Type", "application/json");
        int httpResponseCode = http.POST(payload);
        if (httpResponseCode > 0) {
            Serial.printf("HTTP Response code on send: %d\n", httpResponseCode);
            String response = http.getString();
        } else {
            Serial.printf("HTTP Request failed on send: %s\n", http.errorToString(httpResponseCode).c_str());
        }
        http.end();
}

void setup() {
    Serial.begin(115200);
    Serial.println("setup");
    pinMode(YELLOW_PIN, OUTPUT);
    pinMode(GREEN_PIN, OUTPUT);
    pinMode(RED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    patternOff();
    initWIFI();
}

void loop() {
    // Button press handler
    if(digitalRead(BUTTON_PIN) == LOW){
        Serial.println("button pressed");
        pattern = switchPattern(pattern);
        applyPattern();
        sendData();
        delay(debounce);
    }
    // Sending/receiving data handler
    if (timer > updateFreq){
        getData();
        sendData();
        timer = 0;
    }
    applyPattern();
    changeLights();
    delay(loopTime);
    timer += loopTime;
}