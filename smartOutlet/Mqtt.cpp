#include "Mqtt.h"
#include "Network.h"

Mqtt::Mqtt(char* mqttServer, int port){
  _mqttServer = mqttServer;
  _port = port;
  _mqttClient = PubSubClient(_client);
}

Mqtt::Mqtt(){

}

void Mqtt::configure(){
  _mqttClient.setServer(_mqttServer, _port);
}

boolean Mqtt::connect(){
  boolean isConnected = _mqttClient.connect(Network::_ssidAccessPoint);
  Serial.print("MQTT connected: ");
  Serial.println(isConnected);
  return isConnected;
}

void Mqtt::disconnect(){
  _mqttClient.disconnect();
}

void Mqtt::isConnected(){
  Serial.print("MQTT connected: ");
  Serial.println(_mqttClient.connected());
}

void Mqtt::publish(char* topic, char* payload){
  _mqttClient.publish(topic, payload);
  Serial.println("Public message " + String(payload) + " on topic " + String(topic));
}

void Mqtt::update(){
  _mqttClient.loop();
}

