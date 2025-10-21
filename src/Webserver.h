#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "iPixelDeviceRegistry.h"

AsyncWebServer server(80);

void init_webserver() {
    Serial.println("Initializing webserver...");

    server.onNotFound([](AsyncWebServerRequest* request) {
        String url = request->url();
        Serial.println("Incoming: " + url);

        if (!url.startsWith("/device/")) {
            request->send(404, "text/plain", "Unknown route");
            return;
        }

        int firstSlash = url.indexOf('/', 8); // position after "/device/"
        if (firstSlash == -1) {
            request->send(400, "text/plain", "Invalid device address format");
            return;
        }

        String macStr = url.substring(8, firstSlash);
        String action = url.substring(firstSlash + 1);

        Serial.printf("MAC=%s, action=%s\n", macStr.c_str(), action.c_str());

        NimBLEAddress addr(macStr.c_str(), 0);
        iPixelDevice* dev = getOrCreateDevice(addr);

        if (!dev->connected) {
            request->send(408, "text/plain", "Device is connecting");
            return;
        }

        auto getParamInt = [&](const char* name, long def = 0L) -> long {
            if (request->hasParam(name)) return request->getParam(name)->value().toInt();
            return def;
        };
        auto getParamBool = [&](const char* name, bool def = false) {
            if (request->hasParam(name)) return request->getParam(name)->value() == "true";
            return def;
        };

        try {
            if (action == "setTime") {
                dev->setTime(getParamInt("hour"), getParamInt("minute"), getParamInt("second"));
            } else if (action == "setFunMode") {
                dev->setFunMode(getParamBool("funMode"));
            } else if (action == "setOrientation") {
                dev->setOrientation(getParamInt("orientation"));
            } else if (action == "clear") {
                dev->clear();
            } else if (action == "setBrightness") {
                dev->setBrightness(getParamInt("brightness"));
            } else if (action == "setSpeed") {
                dev->setSpeed(getParamInt("speed"));
            } else if (action == "ledOff") {
                dev->ledOff();
            } else if (action == "ledOn") {
                dev->ledOn();
            } else if (action == "deleteScreen") {
                dev->deleteScreen(getParamInt("screen"));
            } else if (action == "setPixel") {
                dev->setPixel(getParamInt("x"), getParamInt("y"), getParamInt("r"), getParamInt("g"), getParamInt("b"));
            } else if (action == "setClockMode") {
                dev->setClockMode(getParamInt("style"), getParamInt("dayOfWeek"), getParamInt("year"), getParamInt("month"), getParamInt("day"), getParamBool("showDate"), getParamBool("format24"));
            } else if (action == "setRhythmLevelMode") {
                int levels[11];
                for (int i = 0; i < 11; ++i) {
                    levels[i] = getParamInt("l" + char(char('0') + i));
                }
                dev->setRhythmLevelMode(getParamInt("style"), levels);
            } else if (action == "setRhythmAnimationMode") {
                dev->setRhythmAnimationMode(getParamInt("style"), getParamInt("frame"));
            } else {
                request->send(400, "text/plain", "Invalid action");
                return;
            }
            request->send(200, "text/plain", "OK");
        } catch(const std::invalid_argument &ex) {
            request->send(404, "text/plain", ex.what());
        }
  });

  server.begin();
  Serial.println("Webserver initialized!");
}