#include "MemoryHandler.h"

MemoryHandler::MemoryHandler(Preferences& obj): pref(obj) {}

void MemoryHandler::writeCredentials(String ssid, String wifi_pass, String broker_addr, String broker_usr, String broker_pass, bool anonymous, String topics) {
  pref.begin("wifi");  ///< Start writing Wi-Fi credentials to the "wifi" namespace.
  pref.putString("ssid", ssid);
  pref.putString("pass", wifi_pass);
  pref.end();

  int8_t colonIndex = broker_addr.indexOf(':'); ///< Find the colon separator in the broker address (IP:port).
  pref.begin("broker");  ///< Start writing broker credentials to the "broker" namespace.
  pref.putString("ip", broker_addr.substring(0, colonIndex));  ///< Store the broker IP address.
  pref.putString("port", broker_addr.substring(colonIndex + 1));  ///< Store the broker port.
  pref.putString("usr", broker_usr);  ///< Store the broker username.
  pref.putString("pass", broker_pass);  ///< Store the broker password.
  pref.putBool("anonymous", anonymous);  ///< Store the anonymous connection flag.
  pref.putString("topics", topics);  ///< Store the topics.
  pref.end();
}

std::vector<String> MemoryHandler::getWifiCredentials() {
  pref.begin("wifi", true);  ///< Open the "wifi" namespace in read-only mode.
  String ssid = pref.getString("ssid");  ///< Retrieve the Wi-Fi SSID.
  String pass = pref.getString("pass");  ///< Retrieve the Wi-Fi password.
  pref.end();
  return {ssid, pass};  ///< Return the credentials as a vector.
}

std::vector<String> MemoryHandler::getBrokerCredentials() {
  pref.begin("broker", true);  ///< Open the "broker" namespace in read-only mode.
  String ip = pref.getString("ip");  ///< Retrieve the broker IP address.
  String port = pref.getString("port");  ///< Retrieve the broker port.
  String usr = pref.getString("usr");  ///< Retrieve the broker username.
  String pass = pref.getString("pass");  ///< Retrieve the broker password.
  pref.end();
  return {ip, port, usr, pass};  ///< Return the credentials as a vector.
}

std::vector<const char*> MemoryHandler::getBrokerTopics() {
  pref.begin("broker", true);  ///< Open the "broker" namespace in read-only mode.
  const String tp = pref.getString("topics");  ///< Retrieve the topics string.
  pref.end();

  std::vector<const char*> topicVector;
  int startIdx = 0;

  // Iterate over the input string and split by ':'
  for (int i = 0; i <= tp.length(); i++) {
    if (i == tp.length() || tp[i] == ':') {
      String topic = tp.substring(startIdx, i);  ///< Extract the topic string.
      char* topicCStr = new char[topic.length() + 1];  ///< Create a buffer for the topic.
      strcpy(topicCStr, topic.c_str());  ///< Copy the topic string into the buffer.
      topicVector.push_back(topicCStr);  ///< Add the topic to the vector.
      startIdx = i + 1;  ///< Update the start index for the next topic.
    }
  }

  return topicVector;  ///< Return the list of topics.
}

void MemoryHandler::clearMemory() {
  writeCredentials("", "", "", "", "", false, "");  ///< Write empty values for cleaning.
}

bool MemoryHandler::isWiFiConfigAvailable() {
  pref.begin("wifi", true);  ///< Open the "wifi" namespace in read-only mode.

  bool ssidExists = pref.isKey("ssid");  ///< Check if the "ssid" key exists.
  bool passwordExists = pref.isKey("pass");  ///< Check if the "pass" key exists.

  if (ssidExists && passwordExists) {
    String ssid = pref.getString("ssid", "");
    String password = pref.getString("pass", "");

    // If the SSID or password is empty, the configuration is invalid
    if (ssid.isEmpty() || password.isEmpty()) {
      Serial.println("Wi-Fi configuration keys exist, but values are empty.");
      pref.end();
      return false;
    } else {
      Serial.println("Wi-Fi configuration found.");
      pref.end();
      return true;
    }
  } else {
    Serial.println("Wi-Fi configuration keys do not exist.");
    pref.end();
    return false;
  }
}

bool MemoryHandler::isBrokerConfigAvailable() {
  pref.begin("broker", true);  ///< Open the "broker" namespace in read-only mode.

  if (pref.isKey("ip") && pref.isKey("port")) {
    String ip = pref.getString("ip", "");
    String port = pref.getString("port", "");
    String usr = pref.getString("usr", "");
    String pass = pref.getString("pass", "");
    bool anonymous = pref.getBool("anonymous", true);
    String topics = pref.getString("topics", "");

    if (ip.isEmpty() || port.isEmpty()) {
      pref.end();
      Serial.println("Broker configuration exists, but IP and Port are empty.");
      return false;
    }

    Serial.println("Broker configuration found:");
    if (anonymous) {
      Serial.println("Connection type: Anonymous");
      pref.end();
      return true;
    }

    if (!usr.isEmpty() && !pass.isEmpty()) {
      Serial.println("Connection type: Private");
      if (!topics.isEmpty()) {
        Serial.println("Topics: " + topics);
      } else {
        Serial.println("Topics field is empty!");
        pref.end();
        return false;
      }

      pref.end();
      return true;
    } else {
      Serial.println("Broker Username or Password not found!");
      pref.end();
      return false;
    }
  } else {
    pref.end();
    Serial.println("Broker configuration keys do not exist!");
    return false;
  }
}
