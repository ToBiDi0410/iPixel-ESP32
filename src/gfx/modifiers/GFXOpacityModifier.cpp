#include "GFXOpacityModifier.h"

void GFXOpacityModifier::fromJSON(JsonObject& obj) {
    if(!obj["opacity"].is<uint8_t>()) throw std::invalid_argument("Missing or invalid opacity value for GFXOpacityModifier");
    opacity = obj["opacity"].as<uint8_t>();
};

void GFXOpacityModifier::apply(GFXElement* e) {
    for (size_t i = 0; i < e->framebuffer.size(); i += 4)
        e->framebuffer[i + 3] = (e->framebuffer[i + 3] * opacity) / 255;
}