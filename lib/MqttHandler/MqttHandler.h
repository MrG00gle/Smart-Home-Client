#ifndef MQTTHANDLER_H
#define MQTTHANDLER_H

/**
 * @class MqttHandler
 * @brief A class to manage MQTT connection, communication, and device control.
 *
 * This class handles the setup, connection, and communication with an MQTT broker.
 * It also handles device control (e.g., relays) based on MQTT messages, as well as sending
 * temperature data from a DallasTemperature sensor.
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include <vector>
#include <list>
#include <functional> 
#include <Arduino.h>
#include <DallasTemperature.h>
#include "MD_Parola.h"

class MqttHandler{
private:
    std::vector<const char*> topic_list;        ///< List of topics to subscribe to.
    std::vector<String> cred;                   ///< Broker credentials: cred[0] is address, cred[1] is port.
    std::vector<uint8_t> devices;               ///< Pins of connected devices (relays), e.g., devices[0] - device1, devices[1] - device2.
    PubSubClient& mqtt_client;                  ///< MQTT client instance.
    DallasTemperature& temp_sensor;             ///< DallasTemperature sensor instance for reading temperature.
    MD_Parola& disp;                            ///< Display instance for showing characters.

    /**
     * @brief Toggles a device on/off based on its state.
     * 
     * This function controls a relay or device by setting the pin high or low.
     * @param state The desired state of the device (true for on, false for off).
     * @param pin The pin number of the device to control.
     */
    void device(bool state, uint8_t pin);

    /**
     * @brief Displays a character on the display.
     * 
     * This function sends a string to the display for visualization.
     * @param letter The character or string to display.
     */
    void display(String letter);

    /**
     * @brief MQTT callback function for message handling.
     * 
     * This function is called whenever a message is received on a subscribed topic.
     * It processes the message and controls devices or updates the display accordingly.
     * @param topic The topic of the received message.
     * @param message The message payload.
     * @param length The length of the message.
     */
    void callback(char *topic, byte* message, unsigned int length);

public:
    /**
     * @brief Constructor for the MqttHandler class.
     * 
     * Initializes the MqttHandler with the necessary parameters for MQTT communication,
     * temperature sensor, and display setup.
     * @param client Reference to the PubSubClient instance.
     * @param sensor Reference to the DallasTemperature instance.
     * @param display Reference to the MD_Parola display instance.
     * @param topics A list of topics to subscribe to.
     * @param credentials A vector containing the MQTT broker address and port.
     */
    MqttHandler(PubSubClient& client, DallasTemperature& sensor, MD_Parola& display, std::vector<const char*>& topics, std::vector<String>& credentials);

    /**
     * @brief Initializes the MQTT connection.
     * 
     * This function sets up the MQTT broker address, port, and callback function.
     * It also connects to the broker and subscribes to the specified topics.
     */
    void mqtt_setup();

    /**
     * @brief Maintains the MQTT connection.
     * 
     * This function keeps the MQTT client connected to the broker, attempting to reconnect
     * if the connection is lost. It also processes incoming messages.
     */
    void mqtt_loop();

    /**
     * @brief Sends temperature data to the MQTT broker.
     * 
     * This function reads the temperature from the DallasTemperature sensor and publishes
     * it to the specified MQTT topic.
     */
    void mqtt_send_temp();

    /**
     * @brief Disconnects from the MQTT broker.
     * 
     * This function disconnects the MQTT client from the broker.
     */
    void mqtt_disconnect();
};

#endif // MQTTHANDLER_H
