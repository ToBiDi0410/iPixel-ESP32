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

void iPixelDevice::sendCommand(uint8_t* buffer, size_t length) {
    if (!connected || !characteristic) {
        printPrefix();
        Serial.println("ERROR: Not connected or characteristic not ready!");
        return;
    }

    if (length <= 4) {
        printPrefix();
        Serial.println("ERROR: Command too short!");
        return;
    }

    characteristic->writeValue(buffer, length, false);

    // Debug output
    printPrefix();
    Serial.print("Sent command (");
    Serial.print(length);
    Serial.println(" bytes)");
}

void iPixelDevice::setTime(int hour, int minute, int second) {
    uint8_t buf[8];
    size_t len = iPixelCommands::setTime(hour, minute, second, buf, sizeof(buf));
    if(len == 0) return;
    printPrefix();
    Serial.print("Time ");
    Serial.print(hour);
    Serial.print(":");
    Serial.print(minute);
    Serial.print(":");
    Serial.print(second);
    Serial.println();
    sendCommand(buf, len);
}

void iPixelDevice::setFunMode(bool value) {
    uint8_t buf[5];
    size_t len = iPixelCommands::setFunMode(value, buf, sizeof(buf));
    if(len == 0) return;
    printPrefix();
    Serial.print("Fun Mode ");
    Serial.print(value);
    Serial.println();
    sendCommand(buf, len);
}

void iPixelDevice::setOrientation(int orientation) {
    uint8_t buf[5];
    size_t len = iPixelCommands::setOrientation(orientation, buf, sizeof(buf));
    if(len == 0) return;
    printPrefix();
    Serial.print("Orientation ");
    Serial.print(orientation);
    Serial.println();
    sendCommand(buf, len);
}

void iPixelDevice::clear() {
    uint8_t buf[4];
    size_t len = iPixelCommands::clear(buf, sizeof(buf));
    if(len == 0) return;
    printPrefix();
    Serial.print("Clear");
    Serial.println();
    sendCommand(buf, len);
}

void iPixelDevice::setBrightness(int brightness) {
    uint8_t buf[5];
    size_t len = iPixelCommands::setBrightness(brightness, buf, sizeof(buf));
    if(len == 0) return;
    printPrefix();
    Serial.print("Brightness ");
    Serial.print(brightness);
    Serial.println();
    sendCommand(buf, len);
}

void iPixelDevice::setSpeed(int speed) {
    uint8_t buf[4];
    size_t len = iPixelCommands::setSpeed(speed, buf, sizeof(buf));
    if(len == 0) return;
    printPrefix();
    Serial.print("Speed ");
    Serial.print(speed);
    Serial.println();
    sendCommand(buf, len);
}

void iPixelDevice::ledOff() {
    uint8_t buf[5];
    size_t len = iPixelCommands::ledOff(buf, sizeof(buf));
    if(len == 0) return;
    printPrefix();
    Serial.print("LED Off");
    Serial.println();
    sendCommand(buf, len);
}

void iPixelDevice::ledOn() {
    uint8_t buf[5];
    size_t len = iPixelCommands::ledOn(buf, sizeof(buf));
    if(len == 0) return;
    printPrefix();
    Serial.print("LED On");
    Serial.println();
    sendCommand(buf, len);
}

void iPixelDevice::deleteScreen(int screen) {
    uint8_t buf[6];
    size_t len = iPixelCommands::deleteScreen(screen, buf, sizeof(buf));
    if(len == 0) return;
    printPrefix();
    Serial.print("Delete Screen ");
    Serial.print(screen);
    Serial.println();
    sendCommand(buf, len);
};

void iPixelDevice::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    uint8_t buf[10];
    size_t len = iPixelCommands::setPixel(x, y, r, g, b, buf, sizeof(buf));
    if(len == 0) return;
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
    sendCommand(buf, len);
};

void iPixelDevice::setClockMode(int style, int dayOfWeek, int year, int month, int day, bool showDate, bool format24) {
    uint8_t buf[11];
    size_t len = iPixelCommands::setClockMode(style, dayOfWeek, year, month, day, showDate, format24, buf, sizeof(buf));
    if(len == 0) return;
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
    sendCommand(buf, len);
};

void iPixelDevice::setRhythmLevelMode(int style, const int levels[11]) {
    uint8_t buf[16];
    size_t len = iPixelCommands::setRhythmLevelMode(style, levels, buf, sizeof(buf));
    if(len == 0) return;
    printPrefix();
    Serial.print("Rythm Level Mode: Style ");
    Serial.print(style);
    Serial.print(" with Levels ");
    for(int i=0; i<11; i++) {
        if(i!=0) Serial.print(",");
        Serial.print(levels[i]);
    }
    Serial.println();
    sendCommand(buf, len);
}

void iPixelDevice::setRhythmAnimationMode(int style, int frame) {
    uint8_t buf[6];
    size_t len = iPixelCommands::setRhythmAnimationMode(style, frame, buf, sizeof(buf));
    if(len == 0) return;
    printPrefix();
    Serial.print("Rythm Animation Mode: Style ");
    Serial.print(style);
    Serial.print(" at Frame ");
    Serial.print(frame);
    Serial.println();
    sendCommand(buf, len);
};