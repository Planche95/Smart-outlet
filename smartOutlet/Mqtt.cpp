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

int Mqtt::getState(){
  int temp = mqttClient.state();
  //Serial.print("MQTT state: ");
  //Serial.println(temp);
  return temp;
}

void Mqtt::publish(char* topic, char* payload){
  mqttClient.publish(topic, payload);
  Serial.println("Public message " + String(payload) + " on topic " + String(topic));
}

void Mqtt::update(){
  if(mqttClient.state() == 0){
    mqttClient.loop();  
  }
}

