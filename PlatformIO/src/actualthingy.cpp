#include <WiFi.h>
//#include <AsyncTCP.h>
//#include <ESPAsyncWebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Arduino.h>

// Replace with your network credentials
const char* ssid = "TNCAP1E424B";
const char* password = "CmtkkYCnf42rJqMc";
const char* serverSendAddress = "http://192.168.1.212:5000/send_data";
const char* serverGetAddress = "http://192.168.1.212:5000/get_data";

const int YELLOW_PIN = 10; //Yellow
const int GREEN_PIN = 11; //Green
const int RED_PIN = 12; //Red
const int BUTTON_PIN = 13; //Button

bool RedState = 0;
bool GreenState = 0;
bool YellowState = 0;

const int loopTime = 10; //How often the loop() function runs
const int updateFreq = 2000; //How often the esp32 updates from http
const int debounce = 100; //delay on the button triggering updates

enum Pattern {SolidYellow, SolidGreen, SolidRed, Rainbow, Chase, Flame, Off};
Pattern pattern = Off;

int timer = 0;

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

// STATIC PATTERNS

void patternSolidYellow(){
    Serial.println("solid yellow");
    digitalWrite(YELLOW_PIN, HIGH);
    YellowState = 1;
    digitalWrite(GREEN_PIN, LOW);
    GreenState = 0;
    digitalWrite(RED_PIN, LOW);
    RedState = 0;
}

void patternSolidGreen(){
    Serial.println("solid green");
    digitalWrite(YELLOW_PIN, LOW);
    YellowState = 0;
    digitalWrite(GREEN_PIN, HIGH);
    GreenState = 1;
    digitalWrite(RED_PIN, LOW);
    RedState = 0;
}

void patternSolidRed(){
    Serial.println("solid red");
    digitalWrite(YELLOW_PIN, LOW);
    YellowState = 0;
    digitalWrite(GREEN_PIN, LOW);
    GreenState = 0;
    digitalWrite(RED_PIN, HIGH);
    RedState = 1;
}

void patternOff(){
    Serial.println("all lights off");
    digitalWrite(YELLOW_PIN, LOW);
    YellowState = 0;
    digitalWrite(GREEN_PIN, LOW);
    GreenState = 0;
    digitalWrite(RED_PIN, LOW);
    RedState = 0;
}

// CHANGING PATTERNS - NEED TO WORK ON THIS

void patternRainbow(){
    Serial.println("rainbow pattern");
    int numPhases = 6;
    int ticksPerIteration = updateFreq / numPhases;

    switch(timer / ticksPerIteration) {
        case 0:
            digitalWrite(YELLOW_PIN, HIGH);
            YellowState = 1;
            digitalWrite(GREEN_PIN, LOW);
            GreenState = 0;
            digitalWrite(RED_PIN, LOW);
            RedState = 0;
            break;
        case 1:
            digitalWrite(YELLOW_PIN, HIGH);
            YellowState = 1;
            digitalWrite(GREEN_PIN, HIGH);
            GreenState = 1;
            digitalWrite(RED_PIN, LOW);
            RedState = 0;
            break;
        case 2:
            digitalWrite(YELLOW_PIN, HIGH);
            YellowState = 1;
            digitalWrite(GREEN_PIN, HIGH);
            GreenState = 1;
            digitalWrite(RED_PIN, HIGH);
            RedState = 1;
            break;
        case 3:
            digitalWrite(YELLOW_PIN, LOW);
            YellowState = 0;
            digitalWrite(GREEN_PIN, HIGH);
            GreenState = 1;
            digitalWrite(RED_PIN, HIGH);
            RedState = 1;
            break;
        case 4:
            digitalWrite(YELLOW_PIN, LOW); 
            YellowState = 0;
            digitalWrite(GREEN_PIN, LOW);
            GreenState = 0;
            digitalWrite(RED_PIN, HIGH);
            RedState = 1;
            break;
        default:
            digitalWrite(YELLOW_PIN, LOW);
            YellowState = 0;
            digitalWrite(GREEN_PIN, LOW);
            GreenState = 0;
            digitalWrite(RED_PIN, LOW);
            RedState = 0;
            break;
    }
}

void patternChase(){
    Serial.println("chasing lights pattern");
    int numPhases = 3;
    int ticksPerIteration = updateFreq / numPhases;

    switch(timer / ticksPerIteration) {
        case 0:
            digitalWrite(YELLOW_PIN, HIGH);
            YellowState = 1;
            digitalWrite(GREEN_PIN, LOW);
            GreenState = 0;
            digitalWrite(RED_PIN, LOW);
            RedState = 0;
            break;
        case 1:
            digitalWrite(YELLOW_PIN, LOW);
            YellowState = 0;
            digitalWrite(GREEN_PIN, HIGH);
            GreenState = 1;
            digitalWrite(RED_PIN, LOW);
            RedState = 0;
            break;
        default:
            digitalWrite(YELLOW_PIN, LOW);
            YellowState = 0;
            digitalWrite(GREEN_PIN, LOW);
            GreenState = 0;
            digitalWrite(RED_PIN, HIGH);
            RedState = 1;
            break;
    }
}

void patternFlame(){
    Serial.println("flame effect pattern");
    // TODO using maths to flicker n shit
    int yellow = digitalRead(YELLOW_PIN);
    int green = digitalRead(GREEN_PIN);
    int red = digitalRead(RED_PIN);

    if (rand() % 40 == 1) {
        digitalWrite(YELLOW_PIN, 1 - yellow);
    }
    if (rand() % 25 == 1) {
        digitalWrite(GREEN_PIN, 1 - green);
    }
    if (rand() % 60 == 1) {
        digitalWrite(RED_PIN, 1 - red);
    }    

    // digitalWrite(YELLOW_PIN, HIGH);
    // digitalWrite(GREEN_PIN, HIGH);
    // digitalWrite(RED_PIN, LOW);
}

// THE BIT WHERE WE REPEAT OURSELVES BC OF C++'S LIMITATIONS

String patternString(Pattern p){
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

Pattern stringPattern(String p){
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

void getData(){
    JsonDocument data;

    HTTPClient http;
    http.begin(serverGetAddress);
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
        Serial.printf("HTTP Response code on get : %d\n", httpResponseCode);
        String response = http.getString();
        deserializeJson(data, response);
        pattern = stringPattern(data["Pattern"]);
        //Serial.println(response);
    } else {
        Serial.printf("HTTP Request failed on get : %s\n", http.errorToString(httpResponseCode).c_str());
    }
    http.end();
}

void sendData(){

    float temperature = temperatureRead();
    JsonDocument jsonDoc;

    jsonDoc["Pattern"] = patternString(pattern);
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
            //Serial.println(response);
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
    applyPattern(); // update the pattern that we have switched to - with time
    if(digitalRead(BUTTON_PIN) == LOW){
        Serial.println("button pressed");
        pattern = switchPattern(pattern);
        applyPattern();
        sendData();
        delay(debounce);
    }
    if (timer > updateFreq){
        //Serial.printf("Before getData, pattern = %s\n", patternString(pattern));
        getData();
        //Serial.printf("Before sendData, pattern = %s\n", patternString(pattern));
        sendData();
        //Serial.printf("After sendData, pattern = %s\n", patternString(pattern));
        timer = 0;
        //Serial.printf("Current pattern: %s\n", patternString(pattern));
    }
    applyPattern();
    delay(loopTime);
    timer += loopTime;
}