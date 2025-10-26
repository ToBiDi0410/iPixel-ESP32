# ESP32-iPixel

**ESP32-iPixel** is a project to control **iPixel color matrices** using an ESP32 microcontroller.  
It exposes the device’s functionality via a **REST API** served by a built-in web server.

---

# ⚠️⚠️⚠️ UNSTABLE
This is currently only work-in-progress with the only supported device beeing the ESP32-S3 (SuperMicro).  
Support for further ESP32 boards is planned.  

---

## 📡 Endpoints
All endpoints are **device-specific**, identified by `:mac` (the device's MAC address).  
You have to determine this BLE-address on your own (e.g. using Android Apps).  

> ⚠️ When an endpoint is called for the first time, the ESP32 connects to the device.  
> This may temporarily return **HTTP 408 (Request Timeout)**.

| Endpoint                              | Description                    | Parameters                                                                                                                        |
| ------------------------------------- | ------------------------------ | --------------------------------------------------------------------------------------------------------------------------------- |
| `/device/:mac/setTime`                | Set device time                | `hour=0-23&minute=0-59&second=0-59`                                                                                               |
| `/device/:mac/setFunMode`             | Enable/disable fun mode        | `funMode=true/false`                                                                                                              |
| `/device/:mac/setOrientation`         | Set display orientation        | `orientation=0-2`                                                                                                                 |
| `/device/:mac/clear`                  | Clear the display              | —                                                                                                                                 |
| `/device/:mac/setBrightness`          | Adjust brightness              | `brightness=0-100`                                                                                                                |
| `/device/:mac/setSpeed`               | Adjust animation speed         | `speed=0-100`                                                                                                                     |
| `/device/:mac/ledOff`                 | Turn LEDs off                  | —                                                                                                                                 |
| `/device/:mac/ledOn`                  | Turn LEDs on                   | —                                                                                                                                 |
| `/device/:mac/deleteScreen`           | Delete a screen                | `screen=0-10`                                                                                                                     |
| `/device/:mac/setPixel`               | Set individual pixel color     | `x=0-255&y=0-255&r=0-255&g=0-255&b=0-255`                                                                                         |
| `/device/:mac/setClockMode`           | Configure clock mode           | `style=1-8&dayOfWeek=1-7&year=0-99&month=1-12&day=1-31`                                                                           |
| `/device/:mac/setRhythmLevelMode`     | Set rhythm-level visualization | `style=0-4&l0=0-15&...&l14=0-15`                                                                                                  |
| `/device/:mac/setRhythmAnimationMode` | Set rhythm animation           | `style=0-1&frame=0-7`                                                                                                             |
| `/device/:mac/sendText`               | Send text                      | `text=Hello&animation=0-7&save_slot=1-10&speed=0-100&colorR=0-255&colorG=0-255&colorB=0-255&rainbow_mode=0-9&matrix_height=0-255` |
---

## ⚙️ Installation
1. Install **[PlatformIO](https://platformio.org/)**.
2. Build and upload the firmware to the ESP32 using PlatformIO.
3. Open a **Chromium-based browser** and visit [https://install.wled.me/](https://install.wled.me/).
4. Click **Install** and select your device.
5. Connect to WiFi:

   * Click **Connect to WiFi** and wait for the scan to finish.
   * Select your WiFi network and enter credentials.
   * Wait for the connection to establish.
6. Click **Visit Device** to get the IP address.

---
## 📝 Todo
* Arbitrary `setPixels` to draw a custom framebuffer
* Device pairings (save devices instead of always using the mac)
* Password or key-based protection
* Connection improvements (faster connect? better disconnect handling?)
* Clock Mode via NTP (set Display as clock)
* Web Flasher Support
---

## 🙏 Credits
Reverse engineering of the protocol by **[lucagoc](https://github.com/lucagoc)** via his **[iPixel-CLI](https://github.com/lucagoc/iPixel-CLI)**.

## 🤖 AI Disclaimer
Some descriptions and code-snippets in this repository are generated by artificial intelligence.  