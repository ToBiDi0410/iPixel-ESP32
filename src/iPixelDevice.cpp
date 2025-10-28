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

void iPixelDevice::queueTick() {
    if (queue.empty()) return;
    //Get command from queue
    std::vector<uint8_t> &command = queue.front();

    //Take bytes from command
    size_t chunkSize = min(500, (int)command.size());

    //Write bytes from command
    bool writeOk = false;
    writeOk = characteristic->writeValue(command.data(), chunkSize, false);

    //Check that device is still connected
    if (!writeOk) {
        printPrefix();
        Serial.println("ERROR: write failed or no ACK received, disconnecting...");

        connected = false;
        client->disconnect();

        delay(100);
        return;
    }

    //Debug
    printPrefix();
    Serial.print("Sent chunk of ");
    Serial.print(chunkSize);
    Serial.print(" bytes (remaining: ");
    Serial.print(command.size());
    Serial.print(") (queue size: ");
    Serial.print(queue.size());
    Serial.println(")");

    //Print bytes as HEX
    Serial.print("Data: ");
    for (size_t i = 0; i < chunkSize; i++) {
        if (command[i] < 0x10) Serial.print('0'); // leading zero for single-digit hex
        Serial.print(command[i], HEX);
        Serial.print(' ');
    }
    Serial.println();

    //Remove bytes from command
    command.erase(command.begin(), command.begin() + chunkSize);

    //Remove command if empty
    if (command.empty()) queue.erase(queue.begin());

    //Do not overload BLE
    delay(100);
}

void iPixelDevice::queuePush(std::vector<uint8_t> command) {
    queue.push_back(command);
    printPrefix();
    Serial.print("Added command with ");
    Serial.print(command.size());
    Serial.print(" bytes to queue at ");
    Serial.println(queue.size() - 1);
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
    queuePush(command);
}

void iPixelDevice::setFunMode(bool value) {
    std::vector<uint8_t> command = iPixelCommands::setFunMode(value);
    printPrefix();
    Serial.print("Fun Mode ");
    Serial.print(value);
    Serial.println();
    queuePush(command);
}

void iPixelDevice::setOrientation(int orientation) {
    std::vector<uint8_t> command = iPixelCommands::setOrientation(orientation);
    printPrefix();
    Serial.print("Orientation ");
    Serial.print(orientation);
    Serial.println();
    queuePush(command);
}

void iPixelDevice::clear() {
    std::vector<uint8_t> command = iPixelCommands::clear();
    printPrefix();
    Serial.print("Clear");
    Serial.println();
    queuePush(command);
}

void iPixelDevice::setBrightness(int brightness) {
    std::vector<uint8_t> command = iPixelCommands::setBrightness(brightness);
    printPrefix();
    Serial.print("Brightness ");
    Serial.print(brightness);
    Serial.println();
    queuePush(command);
}

void iPixelDevice::setSpeed(int speed) {
    std::vector<uint8_t> command = iPixelCommands::setSpeed(speed);
    printPrefix();
    Serial.print("Speed ");
    Serial.print(speed);
    Serial.println();
    queuePush(command);
}

void iPixelDevice::ledOff() {
    std::vector<uint8_t> command = iPixelCommands::ledOff();
    printPrefix();
    Serial.print("LED: Off");
    Serial.println();
    queuePush(command);
}

void iPixelDevice::ledOn() {
    std::vector<uint8_t> command = iPixelCommands::ledOn();
    printPrefix();
    Serial.print("LED: On");
    Serial.println();
    queuePush(command);
}

void iPixelDevice::deleteScreen(int screen) {
    std::vector<uint8_t> command = iPixelCommands::deleteScreen(screen);
    printPrefix();
    Serial.print("Delete Screen ");
    Serial.print(screen);
    Serial.println();
    queuePush(command);
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
    queuePush(command);
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
    queuePush(command);
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
    queuePush(command);
}

void iPixelDevice::setRhythmAnimationMode(int style, int frame) {
    std::vector<uint8_t> command = iPixelCommands::setRhythmAnimationMode(style, frame);
    printPrefix();
    Serial.print("Rythm Animation Mode: Style ");
    Serial.print(style);
    Serial.print(" at Frame ");
    Serial.print(frame);
    Serial.println();
    queuePush(command);
};

void iPixelDevice::sendText(const String& text, int animation, int save_slot, int speed, uint8_t colorR, uint8_t colorG, uint8_t colorB, int rainbow_mode, int matrix_height) {
    std::vector<uint8_t> command = iPixelCommands::sendText(text, animation, save_slot, speed, colorR, colorG, colorB, rainbow_mode, matrix_height);
    printPrefix();
    Serial.print("Text: text=");
    Serial.print(text);
    Serial.print(", animation=");
    Serial.print(animation);
    Serial.print(", save_slot=");
    Serial.print(save_slot);
    Serial.print(", speed=");
    Serial.print(speed);
    Serial.print(", color=(");
    Serial.print(colorR);
    Serial.print(",");
    Serial.print(colorG);
    Serial.print(",");
    Serial.print(colorB);
    Serial.print("), rainbow_mode=");
    Serial.print(rainbow_mode);
    Serial.print(", matrix_height=");
    Serial.println(matrix_height);
    queuePush(command);
};

void iPixelDevice::sendPNG(const std::vector<uint8_t> &pngData) {
    std::vector<uint8_t> command = iPixelCommands::sendPNG(pngData);
    printPrefix();
    Serial.print("PNG with ");
    Serial.print(pngData.size());
    Serial.println(" bytes");
    queuePush(command);
}

void iPixelDevice::sendGIF(const std::vector<uint8_t> &gifData) {
    std::vector<uint8_t> command = iPixelCommands::sendPNG(gifData);
    printPrefix();
    Serial.print("GIF with ");
    Serial.print(gifData.size());
    Serial.println(" bytes");
    queuePush(command);
}