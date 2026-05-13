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

const int PIN_1 = 10; //Yellow
const int PIN_2 = 11; //Green
const int PIN_3 = 12; //Red
const int PIN_4 = 13; //Button

const int loopTime = 10; //How often the loop() function runs
const int updateFreq = 2000; //How often the esp32 updates from http
const int debounce = 100; //delay on the button triggering updates

enum Pattern {Solid, Rainbow, Chase, Flame, Off};
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

void patternSolid(){
    //Use timer global variable
    digitalWrite(PIN_1, HIGH);
    digitalWrite(PIN_2, HIGH);
    digitalWrite(PIN_3, HIGH);
}
void patternRainbow(){
    //Use timer global variable
    digitalWrite(PIN_1, LOW);
    digitalWrite(PIN_2, HIGH);
    digitalWrite(PIN_3, LOW);
}
void patternChase(){
    //Use timer global variable
    digitalWrite(PIN_1, LOW);
    digitalWrite(PIN_2, LOW);
    digitalWrite(PIN_3, HIGH);
}
void patternFlame(){
    //Use timer global variable
    digitalWrite(PIN_1, HIGH);
    digitalWrite(PIN_2, HIGH);
    digitalWrite(PIN_3, LOW);
}

void patternOff(){
    //Use timer global variable
    digitalWrite(PIN_1, LOW);
    digitalWrite(PIN_2, LOW);
    digitalWrite(PIN_3, LOW);
}

String patternString(Pattern p){
    switch(p){
        case Solid  : return "Solid"; 
        case Rainbow: return "Rainbow";
        case Chase  : return "Chase";
        case Flame  : return "Flame";
        case Off    : return "Off";
        default: return "Off";
    }
}

Pattern stringPattern(String p){
    if (p == "Solid"){
        return Solid;
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
        case Off    : return Solid; break;
        case Solid  : return Rainbow; break;
        case Rainbow: return Chase; break;
        case Chase  : return Flame; break;
        case Flame  : return Off; break;
        default: return Off; break;
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

void applyPattern(){
    switch(pattern){
        case Solid  : patternSolid(); break;
        case Rainbow: patternRainbow(); break;
        case Chase  : patternChase(); break;
        case Flame  : patternFlame(); break;
        case Off    : patternOff(); break;
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("setup");
    pinMode(PIN_1, OUTPUT);
    pinMode(PIN_2, OUTPUT);
    pinMode(PIN_3, OUTPUT);
    pinMode(PIN_4, INPUT_PULLUP);
    patternOff();
    initWIFI();
}

void loop() {
  // put your main code here, to run repeatedly:
    if(digitalRead(PIN_4) == LOW){
        if (timer % 100 == 0){
            Serial.println("button pressed");
            pattern = switchPattern(pattern);
            sendData();
        }
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