#ifndef Eeprom_h
#define Eeprom_h

class Eeprom{
  
  public:
    static void eepromSave(int from, int maxLength, String value);
    static String eepromRead(int from);
    static void readFirst66Bytes();
};

#endif
