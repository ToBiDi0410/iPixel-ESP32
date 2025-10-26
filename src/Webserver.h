#pragma once

#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "iPixelDeviceRegistry.h"
#include "Helpers.h"
#include "gfx/GFX.h"

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
        auto getParamString = [&](const char* name, const String def = "Unknown") {
            if (request->hasParam(name)) return request->getParam(name)->value();
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
                dev->setPixel(
                    getParamInt("x"),
                    getParamInt("y"),
                    getParamInt("r"),
                    getParamInt("g"),
                    getParamInt("b")
                );
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
            } else if (action == "fill") {
                for(int x=0; x<32; x++) {
                    for(int y=0; y<32; y++) {
                        dev->setPixel(x, y, 255, 255, 255);
                    }
                }
            } else if (action == "sendText") {
                dev->sendText(
                    getParamString("text"),
                    getParamInt("animation"),
                    getParamInt("save_slot"),
                    getParamInt("speed"),
                    getParamInt("colorR"),
                    getParamInt("colorG"),
                    getParamInt("colorB"),
                    getParamInt("rainbow_mode"),
                    getParamInt("matrix_height")
                );
            } else if(action == "sendPNG") {
                dev->sendPNG(Helpers::hexStringToVector(getParamString("hex")));
            } else if(action == "sendGIF") {
                dev->sendPNG(Helpers::hexStringToVector(getParamString("hex")));
            } else if(action == "sendArbitrary") {
                int WIDTH = 32;
                int HEIGHT = 32;
                std::vector<uint8_t> framebuffer(WIDTH * HEIGHT * 4); // 3 bytes per pixel (RGBA)
                // Fill framebuffer with rainbow
                for (int y = 0; y < HEIGHT; y++) {
                    for (int x = 0; x < WIDTH; x++) {
                        int i = (y * WIDTH + x) * 4; // 4 bytes per pixel

                        // Hue-based rainbow
                        float h = float(x) / WIDTH; 
                        float r = fabs(sin(h * 3.14159f * 2.0f));
                        float g = fabs(sin((h + 0.33f) * 3.14159f * 2.0f));
                        float b = fabs(sin((h + 0.66f) * 3.14159f * 2.0f));

                        framebuffer[i + 0] = (uint8_t)(r * 255); // Red
                        framebuffer[i + 1] = (uint8_t)(g * 255); // Green
                        framebuffer[i + 2] = (uint8_t)(b * 255); // Blue
                        framebuffer[i + 3] = 255;                // Alpha = fully opaque
                    }
                }
                dev->sendPNG(Helpers::encodeRGBAPixelsToPng(framebuffer, WIDTH, HEIGHT));
            } else if(action == "gfxTest") {
                GFXGroupElement elem(0,0,32,32,0);

                GFXElement* redBox = new GFXElement(0,0,16,16,0);
                redBox->modifiers.push_back(new GFXBackgroundColorModifier(Color(255, 0, 0, 255)));
                elem.children.push_back(redBox);

                GFXElement* greenBox = new GFXElement(16,0,16,16,0);
                greenBox->modifiers.push_back(new GFXBackgroundColorModifier(Color(0, 255, 0, 255)));
                elem.children.push_back(greenBox);

                GFXElement* blueBox = new GFXElement(0,16,16,16,0);
                blueBox->modifiers.push_back(new GFXBackgroundColorModifier(Color(0, 0, 255, 255)));
                elem.children.push_back(blueBox);

                GFXElement* whiteBox = new GFXElement(16,16,16,16,0);
                whiteBox->modifiers.push_back(new GFXBackgroundColorModifier(Color(255, 255, 255, 255)));
                elem.children.push_back(whiteBox);

                //Render and send
                elem._render(0);
                dev->sendPNG(Helpers::encodeRGBAPixelsToPng(elem.framebuffer, elem.width, elem.height));
            } else if(action == "gfxJSON") {
                if (!request->hasParam("root")) {
                    request->send(400, "text/plain", "Missing 'root' parameter");
                    return;
                }

                JsonDocument doc;
                DeserializationError error = deserializeJson(doc, getParamString("root", "{}"));
                if (error) {
                    request->send(400, "text/plain", "Invalid JSON");
                    return;
                }

                JsonObject rootObj = doc.as<JsonObject>();
                GFXElement* rootElement = parseElement(rootObj);

                // Render element
                rootElement->_render(0);
                dev->sendPNG(Helpers::encodeRGBAPixelsToPng(rootElement->framebuffer, rootElement->width, rootElement->height));

                // Cleanup
                delete rootElement;
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