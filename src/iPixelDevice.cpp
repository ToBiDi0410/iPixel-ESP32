#include "iPixelDevice.h"
#include "iPixelCommands.h"

NimBLEUUID serviceUUID("000000fa-0000-1000-8000-00805f9b34fb");
NimBLEUUID charUUID("0000fa02-0000-1000-8000-00805f9b34fb");

void iPixelDevice::printPrefix() {
    Serial.print("[iPixelDevice] [");
    Serial.print(address.toString().c_str());
    Serial.print("] ");
}

void iPixelDevice::onConnect(NimBLEClient *pClient) {
    printPrefix();
    Serial.println("Connected!");
    connected = true;
}

void iPixelDevice::onDisconnect(NimBLEClient *pClient) {
    printPrefix();
    Serial.println("Disconnected!");
    connected = false;
}

void iPixelDevice::connectAsync() {
    if (connected) {
        printPrefix();
        Serial.println("WARN: Already connected!");
        return;
    }

    printPrefix();
    Serial.println("Connecting...");

    if (!client) {
        client = NimBLEDevice::createClient();
        client->setClientCallbacks(this, false);
    }

    // Async connect (non-blocking)
    if (!client->connect(address)) {
        printPrefix();
        Serial.println("ERROR: Failed to connect!");
        return;
    }

    // Discover services & characteristic
    service = client->getService(serviceUUID);
    if (!service) {
        printPrefix();
        Serial.println("ERROR: Service not found!");
        client->disconnect();
        return;
    }

    characteristic = service->getCharacteristic(charUUID);
    if (!characteristic) {
        printPrefix();
        Serial.println("ERROR: Characteristic not found!");
        client->disconnect();
        return;
    }

    connected = true;
    printPrefix();
    Serial.println("Connected successfully!");
}

void iPixelDevice::sendCommand(std::vector<uint8_t> command) {
    if (!connected || !characteristic) {
        printPrefix();
        Serial.println("ERROR: Not connected or characteristic not ready!");
        return;
    }

    if (command.size() <= 4) {
        printPrefix();
        Serial.println("ERROR: Command too short!");
        return;
    }

    characteristic->writeValue(command.data(), command.size(), false);

    // Debug output
    printPrefix();
    Serial.print("Sent command (");
    Serial.print(command.size());
    Serial.println(" bytes)");
}

void iPixelDevice::setTime(int hour, int minute, int second) {
    std::vector<uint8_t> command = iPixelCommands::setTime(hour, minute, second);
    printPrefix();
    Serial.print("Time ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.print(second);
    Serial.println();
    sendCommand(command);
}

void iPixelDevice::setFunMode(bool value) {
    std::vector<uint8_t> command = iPixelCommands::setFunMode(value);
    printPrefix();
    Serial.print("Fun Mode ");
    Serial.print(value);
    Serial.println();
    sendCommand(command);
}

void iPixelDevice::setOrientation(int orientation) {
    std::vector<uint8_t> command = iPixelCommands::setOrientation(orientation);
    printPrefix();
    Serial.print("Orientation ");
    Serial.print(orientation);
    Serial.println();
    sendCommand(command);
}

void iPixelDevice::clear() {
    std::vector<uint8_t> command = iPixelCommands::clear();
    printPrefix();
    Serial.print("Clear");
    Serial.println();
    sendCommand(command);
}

void iPixelDevice::setBrightness(int brightness) {
    std::vector<uint8_t> command = iPixelCommands::setBrightness(brightness);
    printPrefix();
    Serial.print("Brightness ");
    Serial.print(brightness);
    Serial.println();
    sendCommand(command);
}

void iPixelDevice::setSpeed(int speed) {
    std::vector<uint8_t> command = iPixelCommands::setSpeed(speed);
    printPrefix();
    Serial.print("Speed ");
    Serial.print(speed);
    Serial.println();
    sendCommand(command);
}

void iPixelDevice::ledOff() {
    std::vector<uint8_t> command = iPixelCommands::ledOff();
    printPrefix();
    Serial.print("LED: Off");
    Serial.println();
    sendCommand(command);
}

void iPixelDevice::ledOn() {
    std::vector<uint8_t> command = iPixelCommands::ledOn();
    printPrefix();
    Serial.print("LED: On");
    Serial.println();
    sendCommand(command);
}

void iPixelDevice::deleteScreen(int screen) {
    std::vector<uint8_t> command = iPixelCommands::deleteScreen(screen);
    printPrefix();
    Serial.print("Delete Screen ");
    Serial.print(screen);
    Serial.println();
    sendCommand(command);
};

void iPixelDevice::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    std::vector<uint8_t> command = iPixelCommands::setPixel(x, y, r, g, b);
    printPrefix();
    Serial.print("Pixel at (");
    Serial.print(x);
    Serial.print(",");
    Serial.print(y);
    Serial.print(") to rgb(");
    Serial.print(r);
    Serial.print(",");
    Serial.print(g);
    Serial.print(",");
    Serial.print(b);
    Serial.print(")");
    Serial.println();
    sendCommand(command);
};

void iPixelDevice::setClockMode(int style, int dayOfWeek, int year, int month, int day, bool showDate, bool format24) {
    std::vector<uint8_t> command = iPixelCommands::setClockMode(style, dayOfWeek, year, month, day, showDate, format24);
    printPrefix();
    Serial.print("Clock Mode: style=");
    Serial.print(style);
    Serial.print(", showDate=");
    Serial.print(showDate);
    Serial.print(", format24=");
    Serial.print(format24);
    Serial.print(", date=");
    Serial.print(day);
    Serial.print("-");
    Serial.print(month);
    Serial.print("-");
    Serial.print(year);
    Serial.print(", dayOfWeek=");
    Serial.print(dayOfWeek);
    Serial.println();
    sendCommand(command);
};

void iPixelDevice::setRhythmLevelMode(int style, const int levels[11]) {
    std::vector<uint8_t> command = iPixelCommands::setRhythmLevelMode(style, levels);
    printPrefix();
    Serial.print("Rythm Level Mode: Style ");
    Serial.print(style);
    Serial.print(" with Levels ");
    for(int i=0; i<11; i++) {
        if(i!=0) Serial.print(",");
        Serial.print(levels[i]);
    }
    Serial.println();
    sendCommand(command);
}

void iPixelDevice::setRhythmAnimationMode(int style, int frame) {
    std::vector<uint8_t> command = iPixelCommands::setRhythmAnimationMode(style, frame);
    printPrefix();
    Serial.print("Rythm Animation Mode: Style ");
    Serial.print(style);
    Serial.print(" at Frame ");
    Serial.print(frame);
    Serial.println();
    sendCommand(command);
};