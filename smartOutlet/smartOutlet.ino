#include "Network.h"
#include "Mqtt.h"
#include "CurrentMeasure.h"
#include "Eeprom.h"

//TODO const
const int SSID_ACCESS_POINT_SIZE = 15;
char SSID_ACCESS_POINT[SSID_ACCESS_POINT_SIZE];
char* ACCESS_POINT_PASSWORD = "123456789";
char* MQTT_IP = "192.168.0.103";
int MQTT_PORT = 1883;

Network network;
Mqtt mqtt;
CurrentMeasure currentMeasure;

void configureSsid(){
  String ssidAP = String(ESP.getFlashChipId()) + String(ESP.getChipId());
  Serial.println("SSID: " + ssidAP);
  ssidAP.toCharArray(SSID_ACCESS_POINT, SSID_ACCESS_POINT_SIZE);
  Serial.print("SSID: ");
  Serial.println(SSID_ACCESS_POINT);
}

void setup() {
  Serial.begin(57600);
  Serial.println();

  //WiFi.softAPdisconnect(true);
  
  configureSsid();

  String ssidNetwork = Eeprom::readSsid();
  String passwordNetwork = Eeprom::readPassword();

  network = Network(ssidNetwork, passwordNetwork, SSID_ACCESS_POINT, ACCESS_POINT_PASSWORD);
  mqtt = Mqtt(MQTT_IP, MQTT_PORT, SSID_ACCESS_POINT);
  currentMeasure = CurrentMeasure(mqtt);
  
  network.configure();
  mqtt.configure();  
  
  //currentMeasure.startTimers();
}

void loop() {
  network.update();
  mqtt.update();
  currentMeasure.update();
}

