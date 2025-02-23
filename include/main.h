#ifndef MAIN_H
#define MAIN_H

#include "MD_MAX72xx.h"
#include "MD_Parola.h"
#include "SPI.h"
#include <Arduino.h>
#include <Preferences.h>

#include <TaskScheduler.h>

#include <DallasTemperature.h>
#include <OneWire.h>
#include "OneButton.h"

#include <WiFi.h>
#include <PubSubClient.h>

#include <WifiHandler.h>
#include <HttpServer.h>
#include <MemoryHandler.h>
#include <MqttHandler.h>

#define SSID "Esp32"
#define PASS "esp32esp32"
#define SSPEED 921600
#define LED_BUILTIN 2
#define BUTTON_PIN 14
#define MAX_DEVICES 1
#define CS_PIN 5
#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

std::vector<String> wifi_credentials;
std::vector<const char *> topics;
std::vector<String> brocker_cred;

OneWire oneWire(15);
DallasTemperature sensors(&oneWire);
OneButton button(BUTTON_PIN, false);
MD_Parola display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

Preferences preferences;
MemoryHandler memoryHandler(preferences);

WifiHandler* wifiHandler;

WiFiClient espClient;
PubSubClient client(espClient);
MqttHandler* mqttHandler;

Scheduler runner;

#endif // MAIN_H