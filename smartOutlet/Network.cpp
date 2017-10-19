#include "Network.h"
#include "Host.h"

Network::Network(String ssid, String password, char* ssidAccessPoint, char* passwordAccessPoint){
  this->ssid = ssid;
  this->password = password;
  this->passwordAccessPoint = passwordAccessPoint;
  this->ssidAccessPoint = ssidAccessPoint;
}

Network::Network(){
  
}

void Network::connect(String ssid, String password){
    
  char ssidArray[ssid.length()]; 
  char passwordArray[password.length()];

  ssid.toCharArray(ssidArray, ssid.length() + 1);
  password.toCharArray(passwordArray, password.length() + 1);
    
  WiFi.begin(ssidArray, passwordArray);

  int waitForConnect = 0;
  while (WiFi.status() != WL_CONNECTED and waitForConnect++ < 20) {
    delay(500);
    Serial.print(".");
    Serial.println(WiFi.status());
  }
}

void Network::startService(){
  if(WiFi.status() == WL_CONNECTED){
    startServer();
  }
  else{
    startAccessPoint();
  }
}

void Network::startAccessPoint(){
  *accessPoint = AccessPoint(ssidAccessPoint, passwordAccessPoint); 
  (*accessPoint).configure();
  (*accessPoint).begin();   
}

void Network::startServer(){
  Host host(ssidAccessPoint);
  host.configure();
  host.begin();
}
 
void Network::configure(){
  if(!ssid.equals("") and !password.equals("")){
    connect(ssid, password);
    startService();
  }
  else{
    Serial.println("Brak hasla i SSID");
    startAccessPoint();  
  }
}

bool Network::isHost(){
  return accessPoint == NULL;
}

void Network::update(){
  if(accessPoint != NULL){
    (*accessPoint).handleClient();  
  }
}
