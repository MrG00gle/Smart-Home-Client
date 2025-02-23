#include "WifiHandler.h"

WifiHandler::WifiHandler(WiFiClass &WIFI, std::vector<String> &cred, uint8_t led, const char *ssid, const char *passphrase) 
: wifi(WIFI), credentials(cred), status_led(led), ap_ssid(ssid), ap_passphrase(passphrase) {}

void WifiHandler::WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.printf("Connection to %s is successful!\n", WiFi.SSID().c_str());
}

void WifiHandler::WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.printf("Current IP is: %s\n", WiFi.localIP().toString().c_str());
}

void WifiHandler::WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info) {
    Serial.println("Disconnected from WiFi access point");
    Serial.print("WiFi lost connection. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
    Serial.println("Trying to Reconnect");
    setupWiFi(); ///< Attempt to reconnect to WiFi.
}

void WifiHandler::setupWiFi() {
    // Using lambdas to bind the non-static member functions to WiFi events
    wifi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
        this->WiFiStationConnected(event, info);
    }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);

    wifi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
        this->WiFiGotIP(event, info);
    }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);

    wifi.onEvent([this](WiFiEvent_t event, WiFiEventInfo_t info) {
        this->WiFiStationDisconnected(event, info);
    }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    wifi.mode(WIFI_STA);  ///< Set the WiFi mode to station (client).
    wifi.begin(credentials[0].c_str(), credentials[1].c_str());  ///< Start WiFi connection using credentials.

    // Wait for connection, blink status LED while trying to connect.
    while (wifi.status() != WL_CONNECTED) {
        digitalWrite(status_led, HIGH);
        delay(1000);
        digitalWrite(status_led, LOW);
        delay(1000);
    }
}

void WifiHandler::setupAP() {
    wifi.mode(WIFI_AP);  ///< Set the WiFi mode to access point.
    wifi.softAP(ap_ssid, ap_passphrase);  ///< Set up the access point with the provided SSID and passphrase.
    Serial.print("Access point created!\n");
    Serial.printf("Current IP is: %s\n", wifi.softAPIP().toString().c_str());
}

void WifiHandler::disconect(){
    wifi.disconnect(true, false);  ///< Disconnect from the WiFi network.
}
