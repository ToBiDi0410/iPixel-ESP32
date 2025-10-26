#pragma once
#include <Arduino.h>

struct Color {
    uint8_t r, g, b, a;
    Color(uint8_t _r=0, uint8_t _g=0, uint8_t _b=0, uint8_t _a=255)
        : r(_r), g(_g), b(_b), a(_a) {}
};