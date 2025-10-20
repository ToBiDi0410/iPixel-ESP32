#pragma once
#include <Arduino.h>

namespace iPixelCommands {

    bool checkRange(const char* name, int value, int minVal, int maxVal);

    size_t setTime(int hour, int minute, int second, uint8_t* buffer, size_t bufSize);
    size_t setFunMode(bool value, uint8_t* buf, size_t bufSize);
    size_t setOrientation(int orientation, uint8_t* buf, size_t bufSize);
    size_t clear(uint8_t* buf, size_t bufSize);
    size_t setBrightness(int brightness, uint8_t* buf, size_t bufSize);
    size_t setSpeed(int speed, uint8_t* buf, size_t bufSize);
    size_t ledOff(uint8_t* buf, size_t bufSize);
    size_t ledOn(uint8_t* buf, size_t bufSize);
    size_t deleteScreen(int screen, uint8_t* buf, size_t bufSize);
    size_t setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t* buf, size_t bufSize);
    size_t setClockMode(int style, int dayOfWeek, int year, int month, int day, bool showDate, bool format24, uint8_t* buf, size_t bufSize);
    size_t setRhythmLevelMode(int style, const int levels[11], uint8_t* buf, size_t bufSize);
    size_t setRhythmAnimationMode(int style, int frame, uint8_t* buf, size_t bufSize);

}