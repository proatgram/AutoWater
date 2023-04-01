#include <Arduino.h>
#include "Relay.hpp"

void setup() {
    Serial.begin(115200);
}

void loop() {
    Relay relay(3);
    relay.toggle();
}
