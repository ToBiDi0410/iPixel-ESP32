#pragma once

#include <Arduino.h>
#include "../elements/GFXElement.h"

class GFXElement;
class GFXModifier {
public:
    enum class Stage { PRE, POST };
    Stage stage;
    GFXModifier(Stage stage) : stage(stage) {};
    virtual void fromJSON(JsonObject& obj);
    virtual void apply(GFXElement* e);
};