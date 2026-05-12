#include <Arduino.h>

void setup() {
    Serial.begin(115200);
    Serial.println("setup");
}

void loop() {
    Serial.println("loop");
    delay(5000);
}
