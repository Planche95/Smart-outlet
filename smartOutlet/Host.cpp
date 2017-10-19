#include "Host.h"

char* Host::ssid;
ESP8266WebServer Host::server;

Host::Host(char* ssid){
  Host::ssid = ssid;
  Host::server = ESP8266WebServer(80);
}

void Host::handleStartTimer(){
  //startTimers();
  server.send(200,"text/html","Timer start!");
}

void Host::handleRoot(){
  server.send(200,"text/html","Hello from esp8266! :" + String(ssid));
}

void Host::handleStopTimer(){
  //stopTimer();
  server.send(200,"text/html","Timer stop!");
}

void Host::handleNotFound(){
  server.send(200,"text/html", server.uri());
}
  
void Host::configure(){
  Serial.print("Connected! IP: ");
  Serial.println(WiFi.localIP());
  server.on("/",handleRoot);
  server.on("/stop",handleStopTimer);
  server.on("/start",handleStartTimer);
  server.onNotFound(handleNotFound);
}

void Host::begin(){
  server.begin();
}
