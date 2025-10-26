#pragma once

#include <ArduinoJson.h>
#include <string>
#include "../modifiers/Modifiers.h"

GFXModifier* parseModifier(JsonObject& obj);