#include "iPixelCommands.h"

namespace iPixelCommands {

    bool checkRange(const char* name, int value, int minVal, int maxVal) {
        if (value < minVal || value > maxVal) {
            Serial.print("EXCEPTION: ");
            Serial.print(name);
            Serial.print(" out of range (");
            Serial.print(minVal);
            Serial.print("-");
            Serial.print(maxVal);
            Serial.print("), got ");
            Serial.println(value);
            throw std::invalid_argument(std::string(name) + " out of range (" + std::to_string(minVal) + "-" + std::to_string(maxVal) + ") got " + std::to_string(value));
            return false;
        }
        return true;
    }

    size_t setTime(int hour, int minute, int second, uint8_t* buffer, size_t bufSize) {
        if (!checkRange("Hour", hour, 0, 23)) return 0;
        if (!checkRange("Minute", minute, 0, 59)) return 0;
        if (!checkRange("Second", second, 0, 59)) return 0;
        if (bufSize < 8) return 0;

        buffer[0] = 0x08;
        buffer[1] = 0x00;
        buffer[2] = 0x01;
        buffer[3] = 0x80;
        buffer[4] = (uint8_t)hour;
        buffer[5] = (uint8_t)minute;
        buffer[6] = (uint8_t)second;
        buffer[7] = 0x00;

        return 8;
    }

    size_t setFunMode(bool value, uint8_t* buf, size_t bufSize) {
        if (bufSize < 5) return 0;

        buf[0] = 0x05;
        buf[1] = 0x00;
        buf[2] = 0x04;
        buf[3] = 0x01;
        buf[4] = value ? 1 : 0;

        return 5;
    }
        
    size_t setOrientation(int orientation, uint8_t* buf, size_t bufSize) {
        if (!checkRange("Orientation", orientation, 0, 2)) return 0;
        if (bufSize < 5) return 0;

        buf[0] = 0x05;
        buf[1] = 0x00;
        buf[2] = 0x06;
        buf[3] = 0x80;
        buf[4] = orientation;

        return 5;
    }

    size_t clear(uint8_t* buf, size_t bufSize) {
        if (bufSize < 4) return 0;

        buf[0] = 0x04;
        buf[1] = 0x00;
        buf[2] = 0x03;
        buf[3] = 0x80;

        return 4;
    }

    size_t setBrightness(int brightness, uint8_t* buf, size_t bufSize) {
        if (!checkRange("Brightness", brightness, 0, 100)) return 0;
        if (bufSize < 5) return 0;

        buf[0] = 0x05;
        buf[1] = 0x00;
        buf[2] = 0x04;
        buf[3] = 0x80;
        buf[4] = brightness;

        return 5;
    }

    size_t setSpeed(int speed, uint8_t* buf, size_t bufSize) {
        if (!checkRange("Speed", speed, 0, 100)) return 0;
        if (bufSize < 4) return 0;

        buf[0] = 0x05;
        buf[1] = 0x00;
        buf[2] = 0x03;
        buf[3] = speed;

        return 4;
    }

    size_t ledOff(uint8_t* buf, size_t bufSize) {
        if (bufSize < 5) return 0;

        buf[0] = 0x05;
        buf[1] = 0x00;
        buf[2] = 0x07;
        buf[3] = 0x01;
        buf[4] = 0x00;

        return 5;
    }

    size_t ledOn(uint8_t* buf, size_t bufSize) {
        if (bufSize < 5) return 0;

        buf[0] = 0x05;
        buf[1] = 0x00;
        buf[2] = 0x07;
        buf[3] = 0x01;
        buf[4] = 0x01;

        return 5;
    }

    size_t deleteScreen(int screen, uint8_t* buf, size_t bufSize) {
        if (!checkRange("Screen", screen, 0, 10)) return 0;
        if (bufSize < 6) return 0;

        buf[0] = 0x07;
        buf[1] = 0x00;
        buf[2] = 0x02;
        buf[3] = 0x01;
        buf[4] = 0x00;
        buf[5] = screen;

        return 6;
    }

    size_t setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t* buf, size_t bufSize) {
        if (!checkRange("X", x, 0, 255)) return 0;
        if (!checkRange("Y", y, 0, 255)) return 0;
        if (!checkRange("R", r, 0, 255)) return 0;
        if (!checkRange("G", g, 0, 255)) return 0;
        if (!checkRange("B", b, 0, 255)) return 0;
        if (bufSize < 10) return 0;

        buf[0] = 0x0A;
        buf[1] = 0x00;
        buf[2] = 0x05;
        buf[3] = 0x01;
        buf[4] = 0x00;
        buf[5] = r;
        buf[6] = g;
        buf[7] = b;
        buf[8] = x;
        buf[9] = y;

        return 10;
    }

    size_t setClockMode(int style, int dayOfWeek, int year, int month, int day, bool showDate, bool format24, uint8_t* buf, size_t bufSize) {
        if (!checkRange("Style", style, 1, 8)) return 0;
        if (!checkRange("Day of Week", dayOfWeek, 1, 7)) return 0;
        if (!checkRange("Year", year, 0, 99)) return 0;
        if (!checkRange("Month", month, 1, 12)) return 0;
        if (!checkRange("Day", day, 1, 31)) return 0;
        if (bufSize < 9) return 0;

        buf[0] = 0x0B; buf[1] = 0x00; buf[2] = 0x06; buf[3] = 0x01;
        buf[4] = style;
        buf[5] = format24 ? 1 : 0;
        buf[6] = showDate ? 1 : 0;
        buf[7] = year;
        buf[8] = month;
        buf[9] = day;
        buf[10] = dayOfWeek;

        return 11;
    }

    size_t setRhythmLevelMode(int style, const int levels[11], uint8_t* buf, size_t bufSize) {
        if (!checkRange("Style", style, 0, 4)) return 0;
        for (int i = 0; i < 11; i++)
            if (!checkRange("Level", levels[i], 0, 15)) return 0;
        if (bufSize < 15) return 0;

        buf[0] = 0x10;
        buf[1] = 0x00; 
        buf[2] = 0x01;
        buf[3] = 0x02;
        buf[4] = style;
        for (int i = 0; i < 11; i++)
            buf[5 + i] = levels[i];

        return 16;
    }

    size_t setRhythmAnimationMode(int style, int frame, uint8_t* buf, size_t bufSize) {
        if (!checkRange("Style", style, 0, 1)) return 0;
        if (!checkRange("Frame", frame, 0, 7)) return 0;
        if (bufSize < 6) return 0; 

        buf[0] = 0x06;
        buf[1] = 0x00;
        buf[2] = 0x00;
        buf[3] = 0x02;
        buf[4] = frame;
        buf[5] = style;

        return 6;
    }

}
