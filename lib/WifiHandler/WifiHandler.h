#ifndef WIFIHANDLER_H
#define WIFIHANDLER_H

#include <Arduino.h>
#include <WiFi.h>
#include <memory.h>
#include <vector>

/**
 * @class WifiHandler
 * @brief A class that handles WiFi connection and access point (AP) functionality.
 *
 * This class manages the connection to a WiFi network, monitors the WiFi connection status,
 * and provides functionality for setting up an access point (AP) mode.
 * It also handles events related to WiFi connection, IP assignment, and disconnection.
 */
class WifiHandler{
private:
    WiFiClass& wifi; ///< WiFi instance used for connection and events.
    std::vector<String>& credentials; ///< Vector of WiFi credentials (SSID, password).
    uint8_t status_led; ///< GPIO pin for the status LED.
    const char* ap_ssid; ///< SSID for the access point.
    const char* ap_passphrase; ///< Passphrase for the access point.

    /**
     * @brief Event handler for WiFi station connection.
     * 
     * This function is called when the WiFi station successfully connects to a network.
     * @param event The WiFi event.
     * @param info Event-specific information.
     */
    void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info);

    /**
     * @brief Event handler for receiving an IP address.
     * 
     * This function is called when the WiFi station receives an IP address.
     * @param event The WiFi event.
     * @param info Event-specific information.
     */
    void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);

    /**
     * @brief Event handler for WiFi station disconnection.
     * 
     * This function is called when the WiFi station is disconnected from a network.
     * @param event The WiFi event.
     * @param info Event-specific information.
     */
    void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info);

public:
    /**
     * @brief Constructor for WifiHandler class.
     * 
     * Initializes the WifiHandler object with WiFi instance, credentials, LED pin, and AP settings.
     * @param WIFI Reference to the WiFiClass instance.
     * @param cred Vector of credentials (SSID and password).
     * @param led GPIO pin number for the status LED.
     * @param ssid SSID for the access point.
     * @param passphrase Passphrase for the access point.
     */
    WifiHandler(WiFiClass &WIFI, std::vector<String> &cred, uint8_t led, const char *ssid, const char *passphrase);

    /**
     * @brief Sets up the WiFi station mode and attempts to connect to the provided WiFi network.
     *
     * This method sets up the WiFi in station mode, attempts to connect using provided credentials, 
     * and monitors the WiFi connection status.
     */
    void setupWiFi();

    /**
     * @brief Sets up the WiFi in access point (AP) mode.
     *
     * This method sets up the device as a WiFi access point, broadcasting the provided SSID and passphrase.
     */
    void setupAP();

    /**
     * @brief Disconnects from the WiFi network.
     *
     * This method disconnects the WiFi station from the current network.
     */
    void disconect();
};

#endif // WIFIHANDLER_H
