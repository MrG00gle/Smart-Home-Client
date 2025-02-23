#include "main.h"

void DuringLongPress(void *oneButton){
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
  digitalWrite(LED_BUILTIN, HIGH);
}

void LongPressStop(void *oneButton){
  runner.pause();
  runner.disableAll();
  mqttHandler -> mqtt_disconnect();
  memoryHandler.clearMemory();
  WiFi.disconnect();
  esp_restart();
}

void temperature(){mqttHandler -> mqtt_send_temp();}
void status_led(){digitalWrite(LED_BUILTIN, WiFi.status() == WL_CONNECTED);}
void mqtt(){mqttHandler -> mqtt_loop();}
void button_tick(){button.tick();}

// Creating tasks
Task t1(2000, TASK_FOREVER, &temperature);
Task t2(2000, TASK_FOREVER, &status_led);
Task t3(100, TASK_FOREVER, &mqtt);
Task t4(50, TASK_FOREVER, &button_tick);

void setup(){
  // Defining Serial speed
  Serial.begin(SSPEED);
  Serial.printf("\nSerial speed is set to: %ld\n", SSPEED);

  // Defining Pin Modes
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(26, HIGH);
  digitalWrite(27, HIGH);
  pinMode(26, OUTPUT);
  pinMode(27, OUTPUT);

  // 8x8 Matrix setup
  display.begin();
  display.displayClear();
  display.setIntensity(0);
  display.displayClear();
  
  // Button setup
  button.attachDuringLongPress(DuringLongPress, &button);
  button.attachLongPressStop(LongPressStop, &button);
  button.setLongPressIntervalMs(2000);
  
  wifiHandler = new WifiHandler(WiFi, wifi_credentials, LED_BUILTIN, SSID, PASS);   // Initializing Handler, and passing to global pointer.

  // Cheking for errors in Configurations
  if (memoryHandler.isWiFiConfigAvailable() == true && memoryHandler.isBrokerConfigAvailable() == true) {
    // Load Topics to global
    wifi_credentials = memoryHandler.getWifiCredentials();
    topics = memoryHandler.getBrokerTopics();
    brocker_cred = memoryHandler.getBrokerCredentials();

    mqttHandler = new MqttHandler(client, sensors, display, topics, brocker_cred);   // Initializing Handler, and passing to global pointer.
    wifiHandler -> setupWiFi();     // Connecting to WiFi
    mqttHandler -> mqtt_setup();    // Conecting to MQTT broker

    // Adding tasks to Task manager
    runner.init();
    runner.addTask(t1);
    runner.addTask(t2);
    runner.addTask(t3);
    runner.addTask(t4);
    t1.enable();
    t2.enable();
    t3.enable();
    t4.enable();
  } else {
    // Run the configuration web server.
    wifiHandler -> setupAP();
    runHttpServer(memoryHandler);
  }
}

void loop() {
  runner.execute();
}