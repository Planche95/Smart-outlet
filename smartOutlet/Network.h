#ifndef Network_h
#define Network_h

#include "AccessPoint.h"

class Network{
  
  private:
    String password;    
    String ssid;      
    char* passwordAccessPoint;    
    char* ssidAccessPoint;
    //Zamien na klase/interfejs "Service"
    AccessPoint* accessPoint;

    void connect(String ssid, String password);
    void startService();
    void startAccessPoint();
    void startServer();

  public: 
    Network(String ssid, String password, char* ssidAccessPoint, char* passwordAccessPoint);
    Network();
    void configure();
    bool isHost();
    void update();
};

#endif
