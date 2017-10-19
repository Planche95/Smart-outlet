#include "Mqtt.h"

Mqtt::Mqtt(char* mqttServer, int port, char* clientId){
  this->clientId = clientId;
  this->mqttServer = mqttServer;
  this->port = port;
  this->mqttClient = PubSubClient(wifiClient);
}

Mqtt::Mqtt(){

}

void Mqtt::configure(){
  mqttClient.setServer(mqttServer, port);
}

boolean Mqtt::connect(){
  boolean isConnected = mqttClient.connect(clientId);
  Serial.print("MQTT connected: ");
  Serial.println(isConnected);
  return isConnected;
}

void Mqtt::disconnect(){
  mqttClient.disconnect();
}

void Mqtt::isConnected(){
  Serial.print("MQTT connected: ");
  Serial.println(mqttClient.connected());
}

void Mqtt::publish(char* topic, char* payload){
  mqttClient.publish(topic, payload);
  Serial.println("Public message " + String(payload) + " on topic " + String(topic));
}

void Mqtt::update(){
  mqttClient.loop();
}

