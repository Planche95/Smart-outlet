#include "AccessPoint.h"
#include "Eeprom.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

char* AccessPoint::ssid;
char* AccessPoint::password;
ESP8266WebServer AccessPoint::server;

AccessPoint::AccessPoint(char* ssid, char* password){
  AccessPoint::ssid = ssid;
  AccessPoint::password = password; 
  AccessPoint::server = ESP8266WebServer(80);
}

void AccessPoint::handleRoot(){
  server.send(200,"text/html", "<html><body style=\"background-color:#f2f2f2\"><table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"10px\" style=\"background-color:#f2f2f2; text-align: center;\"> <tr> <td><font face=\"'Source Sans Pro', sans-serif\" color=\"#222222\" style=\"font-size: 40px; line-height: 28px;\"><b>SMARTI APP</b></font> <td> </tr><tr> <td><font face=\"'Source Sans Pro', sans-serif\" color=\"#6b7381\" style=\"font-size: 20px; line-height: 28px;\"><br/>Configure connection for Device Id: " + String(ssid) + "</font> <td> </tr><tr><td><form action=\"/configure\"><font face=\"'Source Sans Pro', sans-serif\" color=\"#6b7381\" style=\"font-size: 20px; line-height: 28px;\"><br/>SSID:</font> <br><input type=\"text\" name=\"ssid\"> <br><font face=\"'Source Sans Pro', sans-serif\" color=\"#6b7381\" style=\"font-size: 20px; line-height: 28px;\"><br/>Password:</font> <br><input type=\"password\" name=\"pass\"> <br><br><input type=\"Submit\" value=\"Submit\" style=\"background-color:#337ab7;border:1px solid #337ab7;border-radius:3px;color:#ffffff;display:inline-block;font-family:sans-serif;font-size:16px;line-height:44px;text-align:center;text-decoration:none;width:150px;cursor: pointer;\"></form> </td></tr></table></body></html>");
}

void AccessPoint::handleNotFound(){
  server.send(200,"text/html", server.uri());
}

void AccessPoint::handleConfigure(){
  //Serial.println("I'm in configure!");
  if(server.args() == 2){
    Eeprom::saveSsid(server.arg(0));
    Eeprom::savePassword(server.arg(1));
    server.send(200,"text/html", "<html><body style=\"background-color:#f2f2f2\"><table width=\"100%\" border=\"0\" cellspacing=\"0\" cellpadding=\"10px\" style=\"text-align: center;\"> <tr> <td><font face=\"'Source Sans Pro', sans-serif\" color=\"#222222\" style=\"font-size: 40px; line-height: 28px;\"><b>SMARTI APP</b></font> <td> </tr><tr> <td><font face=\"'Source Sans Pro', sans-serif\" color=\"#6b7381\" style=\"font-size: 20px; line-height: 28px;\"><br/>Reset smart Socket!</font> <td></table></body></html>");
    Serial.println("Reset Socket!");
    //TODO while(true) will reset?
    stop(); 
  }
  else{
    Serial.println("False number of arguments!");
  }
}

void AccessPoint::configure(){
  WiFi.softAP(ssid, password);
  Serial.print("Ssid: ");
  Serial.println(ssid);
  Serial.print("Password: ");
  Serial.println(password);
  server.on("/",handleRoot);
  server.on("/configure", handleConfigure);
  server.onNotFound(handleNotFound);
}
  
void AccessPoint::begin(){
  server.begin();
  Serial.println("Access point!");  
}

void AccessPoint::stop(){
  WiFi.softAPdisconnect(true);
}

void AccessPoint::handleClient(){
  server.handleClient();
}
