#include "MqttHandler.h"

MqttHandler::MqttHandler(PubSubClient& client, DallasTemperature& sensor, MD_Parola& display, std::vector<const char*>& topics, std::vector<String>& credentials) 
: mqtt_client(client), temp_sensor(sensor), disp(display), topic_list(topics), cred(credentials) { }

void MqttHandler::device(bool state, uint8_t id){
    digitalWrite(id, state); ///< Toggles the device (relay) on or off based on the state.
}

void MqttHandler::display(String letter){
  disp.setTextAlignment(PA_LEFT);  ///< Set text alignment to left for display.
  disp.print(letter);  ///< Display the provided letter/character.
}

void MqttHandler::callback(char *topic, byte* message, unsigned int length){
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    messageTemp += (char)message[i];  ///< Convert byte message to string.
  }

  // Handle device control based on topics
  if (topic_list.size() > 0 && String(topic) == String(topic_list[1])) {
    if (messageTemp == "on") {
      device(false, 27); ///< Turn on device connected to pin 27.
    } else if (messageTemp == "off") {
      device(true, 27); ///< Turn off device connected to pin 27.
    }
  }

  if (topic_list.size() > 1 && String(topic) == String(topic_list[2])) {
    if (messageTemp == "on") {
      device(false, 26); ///< Turn on device connected to pin 26.
    } else if (messageTemp == "off") {
      device(true, 26); ///< Turn off device connected to pin 26.
    }
  }

  if (topic_list.size() > 2 && String(topic) == String(topic_list[3])) {
    display(messageTemp);  ///< Display received character on the display.
  }
}

void MqttHandler::mqtt_setup(){
  mqtt_client.setServer(cred[0].c_str(), cred[1].toInt());  ///< Set up MQTT broker address and port.
  mqtt_client.setCallback(std::bind(&MqttHandler::callback, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));  ///< Set up callback for incoming messages.

  while (!mqtt_client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("Connecting client %s to the MQTT broker...\n", client_id.c_str());

    if (mqtt_client.connect(client_id.c_str())) {
      Serial.printf("Connected to broker at %s:%s\n", cred[0].c_str(), cred[1].c_str());
      Serial.println("Subscribed to topics:");

      for (const char* topic : topic_list) {
        mqtt_client.subscribe(topic);  ///< Subscribe to each topic in the topic list.
        Serial.println(topic);
      }
    } else {
      Serial.print("Failed to connect with state ");
      Serial.println(mqtt_client.state());
      delay(2000);
    }
  }
}

void MqttHandler::mqtt_loop(){
  if (!mqtt_client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("Attempting to reconnect client %s to MQTT broker...\n", client_id.c_str());

    if (mqtt_client.connect(client_id.c_str())) {
      Serial.println("Reconnected successfully!");
      for (const char* topic : topic_list) {
        mqtt_client.subscribe(topic);  ///< Resubscribe to topics after reconnect.
      }
    } else {
      Serial.print("Failed to reconnect with state ");
      Serial.println(mqtt_client.state());
    }
  }
  mqtt_client.loop();  ///< Process incoming messages.
}

void MqttHandler::mqtt_send_temp(){
  char buffer[10];
  float temp = temp_sensor.getTempCByIndex(0);  ///< Read the temperature from the sensor.
  mqtt_client.publish(topic_list[0], dtostrf(temp, 6, 2, buffer));  ///< Send the temperature data to the MQTT broker.
}

void MqttHandler::mqtt_disconnect(){
  mqtt_client.disconnect();  ///< Disconnect from the MQTT broker.
}
