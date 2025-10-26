#pragma once

#include <Arduino.h>
#include <vector>
#include <NimBLEDevice.h>

extern NimBLEUUID serviceUUID;
extern NimBLEUUID charUUID;

class iPixelDevice : public NimBLEClientCallbacks {
public:
    NimBLEAddress address;
    NimBLEClient *client = nullptr;
    NimBLERemoteService *service = nullptr;
    NimBLERemoteCharacteristic *characteristic = nullptr;
    boolean connected = false;

    iPixelDevice(NimBLEAddress pAddress) : address(pAddress) {}
    void printPrefix();

    //BLEClientCallbacks
    void onConnect(NimBLEClient *pClient);
    void onDisconnect(NimBLEClient *pClient);

    //Methods
    void connectAsync();

    //Queue
    std::vector<std::vector<uint8_t>> queue;
    void queueTick();
    void queuePush(std::vector<uint8_t> command);

    //Commands
    void setTime(int hour, int minute, int second);
    void setFunMode(bool value);
    void setOrientation(int orientation);
    void clear();
    void setBrightness(int brightness);
    void setSpeed(int speed);
    void ledOff();
    void ledOn();
    void deleteScreen(int screen);
    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    void setClockMode(int style, int dayOfWeek, int year, int month, int day, bool showDate, bool format24);
    void setRhythmLevelMode(int style, const int levels[11]);
    void setRhythmAnimationMode(int style, int frame);
    void sendText(const String& text, int animation, int save_slot, int speed, uint8_t colorR, uint8_t colorG, uint8_t colorB, int rainbow_mode, int matrix_height);
    void sendPNG(const std::vector<uint8_t> &pngData);
    void sendGIF(const std::vector<uint8_t> &gifData);

};