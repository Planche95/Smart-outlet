#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include "Timer.h"
#include "Network.h"

WiFiClient client;
PubSubClient mqttClient(client);

Timer timer;

Network network("123456789");

void setup() {
  Serial.begin(115200);
  Serial.println();
  network.configure();
  configureMqtt();
  startTimers();
}

void loop() {
  network.update();
  mqttClient.loop();
  timer.update();
}
