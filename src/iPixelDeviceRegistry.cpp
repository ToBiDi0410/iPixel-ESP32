#include "iPixelDeviceRegistry.h"

std::vector<iPixelDevice*> knownDevices;

iPixelDevice* getOrCreateDevice(const BLEAddress& addr) {
    for (auto* dev : knownDevices) {
        if (dev->address.equals(addr)) {
        return dev;
        }
    }

    iPixelDevice* newDev = new iPixelDevice(addr);
    knownDevices.push_back(newDev);
    return newDev;
}

void loop_deviceregistry() {
    static unsigned long lastAttempt = 0;
    //Connection attempts
    if (millis() - lastAttempt > 1000) {
        lastAttempt = millis();
        for (auto* dev : knownDevices) {
        if (!dev->connected)
            dev->connectAsync();
        }
    }

    //Send from queue
    for (auto* dev : knownDevices)
        if (dev->connected) dev->queueTick();
}