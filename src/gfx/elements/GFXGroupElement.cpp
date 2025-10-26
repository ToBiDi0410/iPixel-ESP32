#include "GFXGroupElement.h"
#include "../parser/ElementParser.h"

GFXGroupElement::~GFXGroupElement() {
    for (GFXElement* child : children) delete child;
    children.clear();
}

void GFXGroupElement::fromJSON(JsonObject& obj) {
    if(!obj["children"].is<JsonArray>()) throw std::invalid_argument("Missing or invalid children value for GFXGroupElement");
    GFXElement::fromJSON(obj);
    if (obj.containsKey("children")) {
        for (auto&& childJSON : obj["children"].as<JsonArray>()) {
            JsonObject childObj = childJSON.as<JsonObject>();
            children.push_back(parseElement(childObj));
        }
    }
}

void GFXGroupElement::render(int depth) {
    // Sort children by zIndex
    std::sort(children.begin(), children.end(), [](const GFXElement* a, const GFXElement* b) {
        return a->zIndex < b->zIndex;
    });

    // Render children into their framebuffers, then blend
    for (auto& child : children) {
        if (!child->enabled) continue;
        child->_render(depth+1);
        blendChild(child);
    }
}

void GFXGroupElement::blendChild(const GFXElement* child) {
    for (uint8_t cy = 0; cy < child->height; ++cy) {
        int py = cy + child->y;
        if (py < 0 || py >= height) continue; // skip out-of-bounds

        for (uint8_t cx = 0; cx < child->width; ++cx) {
            int px = cx + child->x;
            if (px < 0 || px >= width) continue; // skip out-of-bounds

            size_t parentIndex = (py * width + px) * 4;
            size_t childIndex  = (cy * child->width + cx) * 4;

            float alpha = child->framebuffer[childIndex + 3] / 255.0f;

            framebuffer[parentIndex + 0] = (uint8_t)(
                child->framebuffer[childIndex + 0] * alpha +
                framebuffer[parentIndex + 0] * (1 - alpha)
            );
            framebuffer[parentIndex + 1] = (uint8_t)(
                child->framebuffer[childIndex + 1] * alpha +
                framebuffer[parentIndex + 1] * (1 - alpha)
            );
            framebuffer[parentIndex + 2] = (uint8_t)(
                child->framebuffer[childIndex + 2] * alpha +
                framebuffer[parentIndex + 2] * (1 - alpha)
            );
            framebuffer[parentIndex + 3] = std::max(
                framebuffer[parentIndex + 3], child->framebuffer[childIndex + 3]
            );
        }
    }
}