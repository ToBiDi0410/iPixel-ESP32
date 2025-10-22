#include <Arduino.h>
#include <ImprovWiFiLibrary.h>
#include <Preferences.h>
#include "Bluetooth.h"
#include "Webserver.h"

Preferences preferences;
ImprovWiFi improvSerial(&Serial);

void loop_connected();
void setup_connected();
void setup_wifi_post();

void onImprovWiFiErrorCb(ImprovTypes::Error err) {
  Serial.println("[Improv] WiFi failed! Reconnecting to saved one...");
  setup_wifi_post();
}

void onImprovWiFiConnectedCb(const char *ssid, const char *password) {
  Serial.println("[Improv] Got WiFi credentials! (no, we won't leak them here :/)");
  preferences.begin("wifi", false); // namespace "wifi"
  preferences.putString("ssid", ssid);
  preferences.putString("pass", password);
  preferences.end();
  setup_wifi_post();
}

bool connectWifi(const char *ssid, const char *password) {
  WiFi.begin(ssid, password);
  while (!improvSerial.isConnected()) {}
  return true;
}

void setup_wifi_pre() {
  WiFi.mode(WIFI_STA);
  Serial.println("[WiFi] Mode is now 'STATION'!");

  WiFi.disconnect();
  Serial.println("[WiFi] Disconnected after startup!");
}

void setup_wifi_post() {
  preferences.begin("wifi", true); // read-only
  String ssid = preferences.getString("ssid", "");
  String pass = preferences.getString("pass", "");
  preferences.end();
  if(ssid.equals("")) {
    Serial.println("[WiFi] No credentials set! Not connecting!");
    return;
  }

  Serial.print("[WiFi] Connecting to SSID: " + ssid);
  WiFi.begin(ssid, pass);
  while(!WiFi.isConnected()) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("OK");
  Serial.println("[WiFi] Connected!");

  Serial.print("[WiFi] IP: ");
  Serial.println(WiFi.localIP());
  setup_connected();
}

void setup_improv() {
  Serial.println("[Improv] Setting up...");
  improvSerial.setDeviceInfo(ImprovTypes::ChipFamily::CF_ESP32_S3, "iPixel-Server", "1.0.0", "ESP32", "http://{LOCAL_IPV4}?name=Guest");
  improvSerial.onImprovError(onImprovWiFiErrorCb);
  improvSerial.onImprovConnected(onImprovWiFiConnectedCb);
  improvSerial.setCustomConnectWiFi(connectWifi);
  Serial.println("[Improv] Ready!");
}

void setup() {
  delay(2000);
  Serial.begin(115200);
  Serial.println("[Setup] Hello World! Let's hope we can pixel together!");
  setup_wifi_pre();
  setup_improv();
  setup_wifi_post();
}

void setup_connected() {
  init_bluetooth();
  init_webserver();
}

void loop() {
  improvSerial.handleSerial();
  if (improvSerial.isConnected()) loop_connected();
}

void loop_connected() {
  loop_deviceregistry();
}

//  iPixelDevice test(BLEAddress("3d:50:0c:1f:6d:ec"));