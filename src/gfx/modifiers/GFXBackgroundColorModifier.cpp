#include "GFXBackgroundColorModifier.h"

void GFXBackgroundColorModifier::fromJSON(JsonObject& obj) {
    if(!obj["r"].is<uint8_t>()) throw std::invalid_argument("Missing or invalid r value for GFXBackgroundColorModifier");
    if(!obj["g"].is<uint8_t>()) throw std::invalid_argument("Missing or invalid g value for GFXBackgroundColorModifier");
    if(!obj["b"].is<uint8_t>()) throw std::invalid_argument("Missing or invalid b value for GFXBackgroundColorModifier");
    if(!obj["a"].is<uint8_t>()) throw std::invalid_argument("Missing or invalid a value for GFXBackgroundColorModifier");
    color.r = obj["r"].as<uint8_t>();
    color.g = obj["g"].as<uint8_t>();
    color.b = obj["b"].as<uint8_t>();
    color.a = obj["a"].as<uint8_t>();
};

void GFXBackgroundColorModifier::apply(GFXElement *e)  {
    for (size_t i = 0; i < e->width * e->height; ++i) {
        e->framebuffer[i * 4 + 0] = color.r;
        e->framebuffer[i * 4 + 1] = color.g;
        e->framebuffer[i * 4 + 2] = color.b;
        e->framebuffer[i * 4 + 3] = color.a;
    }
}