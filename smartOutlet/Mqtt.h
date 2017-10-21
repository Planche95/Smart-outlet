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
  
  unsigned long waitForConnectStartTime = 0;
  unsigned long waitForConnectPeriod = 10000;
  
  static void callback(char* topic, byte* payload, unsigned int length);
  void reconnect();
  
  public:
  char* clientId;
  
  Mqtt(char* mqttServer, int port, char* clientId);
  Mqtt();
  void configure();
  boolean connect();
  void disconnect();
  int getState();
  static void publish(char* topic, char* payload);
  static void publish(char* topic, float payload);
  void update();
};

#endif
