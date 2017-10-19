#ifndef Eeprom_h
#define Eeprom_h

class Eeprom{

  private:
    static const int ssidEepromStartByte;
    static const int passwordEepromStartByte;
    static const int ssidEepromMaxLength;
    static const int passwordEepromMaxLength;

    static void save(int from, int maxLength, String value);
    static String read(int from);
  
  public:
    static void saveSsid(String value);
    static void savePassword(String value);
    static String readSsid();
    static String readPassword();
    static void readFirst66Bytes();
};

#endif
