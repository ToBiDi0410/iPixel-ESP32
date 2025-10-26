#pragma once

#include "../Color.h"
#include "GFXModifier.h"

class GFXOpacityModifier : public GFXModifier {
    public:
        uint8_t opacity;
        GFXOpacityModifier(uint8_t opacity = 0): GFXModifier(Stage::POST), opacity(opacity) {}
        virtual void fromJSON(JsonObject& obj) override;
        void apply(GFXElement* e) override;
};