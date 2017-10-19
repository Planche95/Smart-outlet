#ifndef AccessPoint_h
#define AccessPoint_h

#include <ESP8266WebServer.h>

class AccessPoint{
  
  private:
  static char* ssid;
  static char* password;
  static ESP8266WebServer server;

  static void handleRoot();
  static void handleNotFound();
  static void handleConfigure();

  public:
  AccessPoint(char* ssid, char* password);  
  void configure();
  void begin();
  static void stop();
  void handleClient();
};

#endif
