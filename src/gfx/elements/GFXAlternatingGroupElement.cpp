#include "GFXAlternatingGroupElement.h"

void GFXAlternatingGroupElement::fromJSON(JsonObject& obj) {
    if(!obj["switchInterval"].is<unsigned long>()) throw std::invalid_argument("Missing or invalid switchInterval value for GFXAlternatingGroupElement");
    GFXGroupElement::fromJSON(obj);
    switchInterval = obj["switchInterval"].as<unsigned long>();
}

void GFXAlternatingGroupElement::setActiveChild(size_t index) {
    if (index < children.size()) {
        activeIndex = index;
        lastSwitchTime = millis();
    }
}

void GFXAlternatingGroupElement::next() {
    if (!children.empty()) {
        activeIndex = (activeIndex + 1) % children.size();
        lastSwitchTime = millis();
    }
}

void GFXAlternatingGroupElement::render(int depth) {
    unsigned long now = millis();
    if (now - lastSwitchTime >= switchInterval)
        next();

    if (!children.empty()) {
        if(!children[activeIndex]->enabled) return;
        children[activeIndex]->_render(depth+1);
        blendChild(children[activeIndex]);
    }
}

void GFXAlternatingGroupElement::setSwitchInterval(unsigned long intervalMs) {
    switchInterval = intervalMs;
}