#pragma once
#include "GFXGroupElement.h"

class GFXAlternatingGroupElement : public GFXGroupElement {
public:
    size_t activeIndex = 0;
    unsigned long switchInterval = 1000;
    unsigned long lastSwitchTime = 0;
    GFXAlternatingGroupElement(uint8_t x = 0, uint8_t y = 0, uint8_t w = 0, uint8_t h = 0, uint8_t zIndex = 0, unsigned long intervalMs = 1000): GFXGroupElement(x, y, w, h, zIndex), switchInterval(intervalMs) {}
    virtual void fromJSON(JsonObject& obj) override;
    void setActiveChild(size_t index);
    void next();
    void render(int depth) override;
    void setSwitchInterval(unsigned long intervalMs);
};