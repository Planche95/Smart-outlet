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

  String sendTopic = "sockets/" + String(topic);
  char topicArray[sendTopic.length() + 1];
  sendTopic.toCharArray(topicArray, sendTopic.length() + 1);
  
  if (strcmp(message,"Check") == 0) {
     int state = digitalRead(D2);
     if(state == HIGH){
       publish(topicArray, "True");
       Serial.println("Check state: True");
     } else {
       publish(topicArray, "False");
       Serial.println("Check state: False");
     }
  }
  else if (strcmp(message,"True") == 0) {
    Serial.println("Set high state on pin"); 
    digitalWrite(D2, HIGH);
  }
  else if (strcmp(message,"False") == 0) {
    Serial.println("Set low state on pin");
    digitalWrite(D2, LOW);
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
    configure();    
    waitForConnectStartTime = millis();
    Serial.print("MQTT state test: ");
    Serial.println(mqttClient.state());
  }
}

void Mqtt::update(){
  if(mqttClient.state() == 0){
    mqttClient.loop();  
    digitalWrite(BUILTIN_LED, LOW);
  } else{
    digitalWrite(BUILTIN_LED, HIGH);
    reconnect();
  }
}

