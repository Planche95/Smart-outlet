#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <PubSubClient.h>
#include "Timer.h"


byte status;

const int ssidStartByte = 0;
const int passwordStartByte = 33;

const int ssidMaxLength = 32;
const int passwordMaxLength = 32;

char ssidAccessPoint[15];
const char* passwordAccessPoint = "123456789";

String command = "";

ESP8266WebServer server(80);

WiFiClient client;
PubSubClient mqttClient(client);

Timer timer;

void configureSsid(){
  String ssidAP = String(ESP.getFlashChipId()) + String(ESP.getChipId());
  Serial.println("SSID: " + ssidAP);
  ssidAP.toCharArray(ssidAccessPoint, strlen(ssidAccessPoint));
}

void configureNetwork(){
  
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  configureSsid();
  configureMqtt();
  configureNetwork();
  startTimers();
}

void loop() {
  server.handleClient();
  mqttClient.loop();
  timer.update();
  
  if(status == 0){
    if(!command.equals("")){
      Serial.println(command);
      saveSsidAndPassword();
      command = "";
    }
  } 
}
