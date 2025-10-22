#pragma once

#include <vector>
#include "iPixelDevice.h"

extern std::vector<iPixelDevice*> knownDevices;
iPixelDevice* getOrCreateDevice(const BLEAddress& addr);
void loop_deviceregistry();