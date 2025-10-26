#pragma once

#include <ArduinoJson.h>
#include <string>
#include "../elements/GFXElement.h"

GFXElement* parseElement(JsonObject& obj);