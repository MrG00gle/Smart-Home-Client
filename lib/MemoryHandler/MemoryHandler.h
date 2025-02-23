#ifndef MEMORYHANDLER_H
#define MEMORYHANDLER_H

/**
 * @class MemoryHandler
 * @brief A class to manage reading and writing of credentials to/from memory (Preferences storage).
 *
 * This class provides functionality to store and retrieve Wi-Fi and MQTT broker credentials,
 * check their availability, and manage errors related to the configuration.
 */
#include <Preferences.h>
#include <vector>

class MemoryHandler {
private:
    String wifi_ssid = "";               ///< Wi-Fi SSID.
    String wifi_pass = "";               ///< Wi-Fi password.
    String broker_addr = "";             ///< Broker address.
    String broker_usr = "";              ///< Broker username.
    String broker_pass = "";             ///< Broker password.
    bool broker_is_anonymous = false;    ///< Flag indicating if broker connection is anonymous.
    String topics = "";                  ///< Broker topics as a colon-separated string.

    Preferences& pref;                   ///< Reference to the Preferences object for non-volatile storage.
public:
    /**
     * @brief Constructor for MemoryHandler class.
     * 
     * Initializes the MemoryHandler with a reference to the Preferences object.
     * @param obj Reference to the Preferences object for memory storage.
     */
    MemoryHandler(Preferences& obj);

    /**
     * @brief Writes Wi-Fi and broker credentials to memory.
     * 
     * This function stores the provided Wi-Fi and broker credentials in the Preferences storage.
     * For clearing storage use MemoryHandler::clearMemory().
     * @param ssid The Wi-Fi SSID.
     * @param wifi_pass The Wi-Fi password.
     * @param broker_addr The broker address (IP or domain).
     * @param broker_usr The broker username.
     * @param broker_pass The broker password.
     * @param anonymous Boolean flag indicating if the broker connection is anonymous.
     * @param topics The list of broker topics to subscribe to, in a colon-separated format.
     */
    void writeCredentials(String ssid, String wifi_pass, String broker_addr, String broker_usr, String broker_pass, bool anonymous, String topics);

    /**
     * @brief Removes credentials for wifi, mqtt brocker and mqtt topics locaded in energy independent memory.
     * 
     * This function should be called for resetting the EIM.
     */
    void clearMemory();

    /**
     * @brief Retrieves the stored Wi-Fi credentials.
     * 
     * This function retrieves the SSID and password of the stored Wi-Fi configuration.
     * @return A vector of strings containing the SSID and password.
     */
    std::vector<String> getWifiCredentials();

    /**
     * @brief Retrieves the stored broker credentials.
     * 
     * This function retrieves the broker address (IP, port), username, and password.
     * @return A vector of strings containing the broker IP, port, username, and password.
     */
    std::vector<String> getBrokerCredentials();

    /**
     * @brief Retrieves the list of topics for the broker.
     * 
     * This function retrieves the list of topics (in a colon-separated string) for subscribing to the broker.
     * @return A vector of const char* representing the topics.
     */
    std::vector<const char*> getBrokerTopics();

    /**
     * @brief Checks if the Wi-Fi credentials are available in memory.
     * 
     * This function checks if the SSID and password keys are available and non-empty in memory.
     * @return True if Wi-Fi credentials are available and valid, false otherwise.
     */
    bool isWiFiConfigAvailable();

    /**
     * @brief Checks if the broker credentials are available in memory.
     * 
     * This function checks if the necessary broker credentials (IP, port) and topics are available in memory.
     * @return True if broker credentials are available and valid, false otherwise.
     */
    bool isBrokerConfigAvailable();
};

#endif // MEMORYHANDLER_H
