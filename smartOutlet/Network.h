#ifndef Network_h
#define Network_h

#include "AccessPoint.h"

class Network{
  
  private:    
    const char* passwordAccessPoint = "123456789";
    AccessPoint* accessPoint;

    void configureSsid();
    void connect(String ssid, String password);
    void startService();
    void startAccessPoint();
    void startServer();

  public: 
    //Jak sie tego pozbyc!!!
    static const int ssidEepromStartByte = 0;
    static const int passwordEepromStartByte = 33;
    static const int ssidEepromMaxLength = 32;
    static const int passwordEepromMaxLength = 32;

    static char ssidAccessPoint[15];
  
    void configure();
    void update();
};

#endif
