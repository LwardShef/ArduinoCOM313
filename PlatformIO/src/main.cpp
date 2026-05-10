#include <Arduino.h>

const char *ssid = "TNCAP1E424B";          // Change this to your WiFi SSID
const char *password = "CmtkkYCnf42rJqMc";  // Change this to your WiFi password
const char *wsAddress = "ws://localhost:5000" // Address of the WebSocket to connect


// put function declarations here:
int myFunction(int, int);

void setup() {
  // put your setup code here, to run once:
  int result = myFunction(2, 3);    

  WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    } 

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    webSocketConnect
}


void loop() {
  // put your main code here, to run repeatedly:
}

// put function definitions here:
int myFunction(int x, int y) {
  return x + y;
}

void webSocketConnect(){
    ws = new WebSocket(wsAddress)
    ws.addEventListener("open"){
        ws.send("Arduino Connected")
  }
}