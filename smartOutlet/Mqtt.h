#ifndef Mqtt_h
#define Mqtt_h

#include <WiFiClient.h>
#include <PubSubClient.h>

class Mqtt{

  private:
  static WiFiClient wifiClient;
  static PubSubClient mqttClient;
  
  char* mqttServer;
  int port;
  
  static void callback(char* topic, byte* payload, unsigned int length);
  
  public:
  char* clientId;
  
  Mqtt(char* mqttServer, int port, char* clientId);
  Mqtt();
  void configure();
  boolean connect();
  void disconnect();
  int getState();
  static void publish(char* topic, char* payload);
  void update();
};

#endif
