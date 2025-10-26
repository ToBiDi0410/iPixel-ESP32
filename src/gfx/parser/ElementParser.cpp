#include "ElementParser.h"
#include "../elements/Elements.h"

GFXElement* parseElement(JsonObject& obj) {
    std::string type = obj["type"].as<std::string>();
    GFXElement* element = nullptr;
    if (type == "GFXElement") element = new GFXElement();
    else if (type == "GFXGroupElement") element = new GFXGroupElement();
    else if (type == "GFXAlternatingGroupElement") element = new GFXAlternatingGroupElement();
    else if (type == "GFXTextElement") element = new GFXTextElement();
    else throw std::invalid_argument("Got an JSON object that is not an element at an element position?!");
    element->fromJSON(obj);

    Serial.print("[GFXParser] Parsed element ");
    Serial.print(type.c_str());
    Serial.print(": x=");
    Serial.print(element->x);
    Serial.print(" y=");
    Serial.print(element->y);
    Serial.print(" w=");
    Serial.print(element->width);
    Serial.print(" h=");
    Serial.print(element->height);
    Serial.print(" z=");
    Serial.print(element->zIndex);
    Serial.print(" modifiers=");
    Serial.println(element->modifiers.size());

    return element;
}