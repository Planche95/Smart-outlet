#ifndef Host_h
#define Host_h

#include <ESP8266WebServer.h>

class Host{

  private:
  static ESP8266WebServer _server;
  static char* _ssid;

  static void handleStartTimer();
  static void handleRoot();
  static void handleStopTimer();
  static void handleNotFound();

  public:
  Host(char* ssidAccessPoint);
  void configure();
  void begin();
};

#endif
