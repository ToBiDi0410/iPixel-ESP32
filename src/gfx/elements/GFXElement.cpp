#include "GFXElement.h"
#include "../parser/ModifierParser.h"

GFXElement::~GFXElement() {
    for (GFXModifier* modifier : modifiers)
        delete modifier;
    modifiers.clear();
}

void GFXElement::fromJSON(JsonObject& obj) {
    if(!obj["x"].is<uint8_t>()) throw std::invalid_argument("Missing or invalid x value for GFXElement");
    if(!obj["y"].is<uint8_t>()) throw std::invalid_argument("Missing or invalid y value for GFXElement");
    if(!obj["width"].is<uint8_t>()) throw std::invalid_argument("Missing or invalid width value for GFXElement");
    if(!obj["height"].is<uint8_t>()) throw std::invalid_argument("Missing or invalid height value for GFXElement");
    if(!obj["zIndex"].is<uint8_t>()) throw std::invalid_argument("Missing or invalid zIndex value for GFXElement");
    x = obj["x"].as<uint8_t>();
    y = obj["y"].as<uint8_t>();
    width = obj["width"].as<uint8_t>();
    height = obj["height"].as<uint8_t>();
    framebuffer.assign(width * height * 4, 0);
    zIndex = obj["zIndex"].as<uint8_t>();
    if (obj.containsKey("modifiers")) {
        for (auto&& modJSON : obj["modifiers"].as<JsonArray>()) {
            JsonObject modObj = modJSON.as<JsonObject>();
            modifiers.push_back(parseModifier(modObj));
        }
    }
}

void GFXElement::render(int depth) {}

void GFXElement::_render(int depth) {
    //Clear framebuffer
    std::fill(framebuffer.begin(), framebuffer.end(), 0);
    if (!enabled) return;

    // Apply all modifiers for PRE stage
    for (const auto& modifier : modifiers)
        if (modifier->stage == GFXModifier::Stage::PRE)
            modifier->apply(this); 

    // Actually render the element
    render(depth);

    // Apply all modifiers for POST stage
    for (const auto& modifier : modifiers)
        if (modifier->stage == GFXModifier::Stage::POST)
            modifier->apply(this); 
}