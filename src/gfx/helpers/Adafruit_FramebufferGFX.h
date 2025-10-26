#pragma once

#include <Arduino.h>
#include <Adafruit_GFX.h>
#include <vector>

/**
 * Class to use Adafruit_GFX for a memory framebuffer
 */
class Adafruit_FramebufferGFX : public Adafruit_GFX {
public:
    std::vector<uint8_t> framebuffer; // RGBA framebuffer
    uint16_t width, height;

    Adafruit_FramebufferGFX(uint16_t w, uint16_t h) : Adafruit_GFX(w, h), width(w), height(h), framebuffer(w * h * 4, 0) {}

    void drawPixel(int16_t x, int16_t y, uint16_t color) override {
        if (x < 0 || y < 0 || x >= width || y >= height) return;

        int i = (y * width + x) * 4;

        uint8_t r = ((color >> 11) & 0x1F) << 3;
        uint8_t g = ((color >> 5) & 0x3F) << 2;
        uint8_t b = (color & 0x1F) << 3;

        framebuffer[i + 0] = r;
        framebuffer[i + 1] = g;
        framebuffer[i + 2] = b;
        framebuffer[i + 3] = 255; // alpha
    }
};