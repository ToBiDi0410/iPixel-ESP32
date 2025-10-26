#include "GFXTextElement.h"

void GFXTextElement::fromJSON(JsonObject& obj) {
    if(!obj["r"].is<uint8_t>()) throw std::invalid_argument("Missing or invalid r value for GFXTextElement");
    if(!obj["g"].is<uint8_t>()) throw std::invalid_argument("Missing or invalid g value for GFXTextElement");
    if(!obj["b"].is<uint8_t>()) throw std::invalid_argument("Missing or invalid b value for GFXTextElement");
    if(!obj["text"].is<String>()) throw std::invalid_argument("Missing or invalid text value for GFXTextElement");
    GFXElement::fromJSON(obj);
    color.r = obj["r"].as<uint8_t>();
    color.g = obj["g"].as<uint8_t>();
    color.b = obj["b"].as<uint8_t>();
    text = obj["text"].as<String>();
}

#define RGB565(r, g, b) (((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3))
void GFXTextElement::render(int depth) {
    Adafruit_FramebufferGFX canvas(width, height);
    canvas.setCursor(0,0);
    uint16_t rgb565_color = RGB565(color.r, color.g, color.b);
    canvas.setTextColor(rgb565_color);
    canvas.print(text);
    std::copy(canvas.framebuffer.begin(), canvas.framebuffer.end(), framebuffer.begin());
}