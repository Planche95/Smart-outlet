#include "Mqtt.h"

WiFiClient Mqtt::wifiClient;
PubSubClient Mqtt::mqttClient;

Mqtt::Mqtt(char* mqttServer, int port, char* clientId){
  this->clientId = clientId;
  this->mqttServer = mqttServer;
  this->port = port;
  this->mqttClient = PubSubClient(wifiClient);
}

Mqtt::Mqtt(){

}

//TODO slip to: configure, connect and startService
void Mqtt::configure(){
  mqttClient.setServer(mqttServer, port);
  mqttClient.setCallback(callback);
  boolean isConnected = connect();
  boolean isSubscribed = mqttClient.subscribe(clientId);
  Serial.print("Topic: ");
  Serial.print(clientId);
  Serial.print(" Sub: ");
  Serial.println(isSubscribed);
}

void Mqtt::callback(char* topic, byte* payload, unsigned int length){
  char message[length + 1];
  memcpy(message, (char*)payload, length);
  message[length] = '\0';
  
  Serial.print("CallBack!! Topic: ");
  Serial.print(topic);
  Serial.print(" Payload: ");
  Serial.print(message);
  Serial.print(" Length: ");
  Serial.println(length);
  
  if (strcmp(message,"CHECK") == 0) {
     publish("ACK", topic);
  }
  else if (strcmp(message,"ON") == 0) {
    Serial.println("Set low state on pin"); 
  }
  else if (strcmp(message,"OFF") == 0) {
    Serial.println("Set high state on pin");
  }
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

void Mqtt::publish(char* topic, float payload){

  String payloadString = String(payload);

  char payloadArray[payloadString.length() + 1];
  payloadString.toCharArray(payloadArray, payloadString.length() + 1);
  
  mqttClient.publish(topic, payloadArray);
  Serial.println("Public message " + payloadString + " on topic " + String(topic));
}

void Mqtt::reconnect(){
  
  if((unsigned long)(millis() - waitForConnectStartTime) >= waitForConnectPeriod) {
    //configure();    
    waitForConnectStartTime = millis();
    Serial.print("MQTT state test: ");
    Serial.println(mqttClient.state());
  }
}

void Mqtt::update(){
  if(mqttClient.state() == 0){
    mqttClient.loop();  
  } else{
    reconnect();
  }
}

