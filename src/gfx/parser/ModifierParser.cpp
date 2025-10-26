#include "ModifierParser.h"

GFXModifier* parseModifier(JsonObject& obj) {
    std::string type = obj["type"].as<std::string>();
    GFXModifier* modifier = nullptr;
    if (type == "GFXBackgroundColorModifier") modifier = new GFXBackgroundColorModifier();
    else if (type == "GFXOpacityModifier") modifier = new GFXOpacityModifier();
    else throw std::invalid_argument("Got an JSON object that is not an modifier at an modifier position?!");
    modifier->fromJSON(obj);

    Serial.print("[GFXParser] Parsed modifier ");
    Serial.println(type.c_str());
    return modifier;
}