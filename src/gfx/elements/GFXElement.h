#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include <vector>
#include "../modifiers/GFXModifier.h"

class GFXModifier;
/**
 * Base class for elements
 */
class GFXElement {
    public:
        //Positioning
        uint8_t x;
        uint8_t y;

        //Size
        uint8_t height;
        uint8_t width;

        //ZIndex
        uint8_t zIndex;

        //State
        bool enabled = true;

        //Modifiers
        std::vector<GFXModifier*> modifiers;

        // RGBA Framebuffer (4 bytes per Pixel)
        std::vector<uint8_t> framebuffer;

        /**
         * Creates a new element with position x and y, size width w and height h
         */
        GFXElement(uint8_t x = 0, uint8_t y = 0, uint8_t w = 0, uint8_t h = 0, uint8_t zIndex = 0): x(x), y(y), width(w), height(h), zIndex(zIndex), framebuffer(w*h*4, 0) {}
        virtual ~GFXElement();
        virtual void fromJSON(JsonObject& obj);

        /**
         * Renders the element into it's own framebuffer 
         * (IMPLEMENTED BY SPECIFIC ELEMENTS!)
         */
        virtual void render(int depth);

        /**
         * Actually performs the full render with modifiers
         */
        void _render(int depth);
};