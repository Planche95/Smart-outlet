#include "Host.h"

char* Host::ssid;
ESP8266WebServer Host::server;

Host::Host(char* ssid){
  Host::ssid = ssid;
  Host::server = ESP8266WebServer(80);
}

void Host::handleRoot(){
  server.send(200,"text/html","Hello from esp8266! :" + String(ssid));
}

void Host::handleNotFound(){
  server.send(200,"text/html", server.uri());
}
  
void Host::configure(){
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());
  server.on("/",handleRoot);
  server.onNotFound(handleNotFound);
}

void Host::begin(){
  server.begin();
}
