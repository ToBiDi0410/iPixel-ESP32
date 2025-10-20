#pragma once

#include <Arduino.h>
#include <NimBLEDevice.h>

void init_bluetooth() {
    Serial.println("Initializing bluetooth...");
    NimBLEDevice::init("ESP32");
    Serial.println("Bluetooth initialized!");
}