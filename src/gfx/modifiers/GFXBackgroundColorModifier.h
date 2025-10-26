#pragma once

#include "../Color.h"
#include "GFXModifier.h"

class GFXBackgroundColorModifier : public GFXModifier {
    public:
        Color color;
        GFXBackgroundColorModifier(Color color = Color(0,0,0,0)) : GFXModifier(Stage::PRE), color(color) {};
        virtual void fromJSON(JsonObject& obj) override;
        void apply(GFXElement *e) override;
};