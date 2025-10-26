#pragma once

#include "GFXElement.h"
#include <vector>

/**
 * A simple group element that renders multiple elements while respecting zIndex and alpha values
 */
class GFXGroupElement : public GFXElement {
    public:
        std::vector<GFXElement*> children;
        GFXGroupElement(uint8_t x = 0, uint8_t y = 0, uint8_t w = 0, uint8_t h = 0, uint8_t zIndex = 0) : GFXElement(x, y, w, h, zIndex) {}
        ~GFXGroupElement() override;
        virtual void fromJSON(JsonObject& obj) override;
        virtual void render(int depth) override;
        void blendChild(const GFXElement* child);
};