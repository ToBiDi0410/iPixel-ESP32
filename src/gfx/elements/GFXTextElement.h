#pragma once

#include "../Color.h"
#include "GFXElement.h"
#include "../helpers/Adafruit_FramebufferGFX.h"

/**
 * A simple text element that uses Adafruit_GFX to render the text
 */
class GFXTextElement : public GFXElement {
    public:
        String text;
        Color color;
        GFXTextElement(uint8_t x = 0, uint8_t y = 0, uint8_t w = 0, uint8_t h = 0, uint8_t zIndex = 0, const String &text = "", Color color = Color(0,0,0,0)) : GFXElement(x, y, w, h, zIndex), text(text), color(color) {}
        virtual void fromJSON(JsonObject& obj) override;
        virtual void render(int depth) override;
};