#include "Host.h"

char* Host::_ssid;
ESP8266WebServer Host::_server;

void Host::handleStartTimer(){
  //startTimers();
  _server.send(200,"text/html","Timer start!");
}

void Host::handleRoot(){
  _server.send(200,"text/html","Hello from esp8266! :" + String(_ssid));
}

void Host::handleStopTimer(){
  //stopTimer();
  _server.send(200,"text/html","Timer stop!");
}

void Host::handleNotFound(){
  _server.send(200,"text/html", _server.uri());
}

Host::Host(char* ssid){
  _ssid = ssid;
  _server = ESP8266WebServer(80);
}
  
void Host::configure(){
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());
  _server.on("/",handleRoot);
  _server.on("/stop",handleStopTimer);
  _server.on("/start",handleStartTimer);
  _server.onNotFound(handleNotFound);
}

void Host::begin(){
  _server.begin();
}
