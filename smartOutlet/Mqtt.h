#ifndef Mqtt_h
#define Mqtt_h

#include <WiFiClient.h>
#include <PubSubClient.h>

class Mqtt{

  private:
  WiFiClient wifiClient;
  PubSubClient mqttClient;
  
  char* mqttServer;
  int port;

  public:
  char* clientId;
  
  Mqtt(char* mqttServer, int port, char* clientId);
  Mqtt();
  void configure();
  boolean connect();
  void disconnect();
  void isConnected();
  void publish(char* topic, char* payload);
  void update();
};

#endif
