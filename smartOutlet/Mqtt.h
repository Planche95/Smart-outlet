#ifndef Mqtt_h
#define Mqtt_h

#include <WiFiClient.h>
#include <PubSubClient.h>

class Mqtt{

  private:
  WiFiClient _client;
  PubSubClient _mqttClient;
  char* _mqttServer;
  int _port;

  public:
  Mqtt(char* mqttServer, int port);
  Mqtt();
  
  void configure();
  boolean connect();
  void disconnect();
  void isConnected();
  void publish(char* topic, char* payload);
  void update();
};

#endif
