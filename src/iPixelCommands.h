#pragma once
#include <Arduino.h>
#include <stdexcept>
#include <string>
#include <vector>

/**
 * Provides functions to generate BLE commands for the
 * iPixel Color Displays
 */
namespace iPixelCommands {

    bool checkRange(const char* name, int value, int minVal, int maxVal);

    std::vector<uint8_t> setTime(int hour, int minute, int second);
    std::vector<uint8_t> setFunMode(bool value);
    std::vector<uint8_t> setOrientation(int orientation);
    std::vector<uint8_t> clear();
    std::vector<uint8_t> setBrightness(int brightness);
    std::vector<uint8_t> setSpeed(int speed);
    std::vector<uint8_t> ledOff();
    std::vector<uint8_t> ledOn();
    std::vector<uint8_t> deleteScreen(int screen);
    std::vector<uint8_t> setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
    std::vector<uint8_t> setClockMode(int style, int dayOfWeek, int year, int month, int day, bool showDate, bool format24);
    std::vector<uint8_t> setRhythmLevelMode(int style, const int levels[11]);
    std::vector<uint8_t> setRhythmAnimationMode(int style, int frameNumber);

}