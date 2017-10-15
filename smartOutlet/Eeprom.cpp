#include <Arduino.h>
#include <EEPROM.h>
#include "Eeprom.h"

void Eeprom::eepromSave(int from, int maxLength, String value){
  if(value.length() < maxLength){
    Serial.println("EEPROM: Write start " + value);
    
    EEPROM.begin(512);
    EEPROM.write(from, (byte)value.length());
    
    for(int i = from + 1, j = 0; j <= value.length(); i++, j++){
      EEPROM.write(i, value.charAt(j));
    }
    
    EEPROM.end();
    
    Serial.println("EEPROM: Write end");
  }
  else{
    Serial.println("EEPROM ERROR: Value is too long");
  }
}

String Eeprom::eepromRead(int from){
  EEPROM.begin(512);
  
  byte valueLength = EEPROM.read(from);

  if(valueLength != 255 and valueLength != 0){
    Serial.println("EEPROM: Read start");
    char readValue[valueLength + 1];
    Serial.print("Length: ");
    Serial.println(valueLength);
    
    for(int i = from + 1, j = 0; j < valueLength; i++, j++){
      readValue[j] = EEPROM.read(i);
    }

    readValue[valueLength] = '\0';
    EEPROM.end();
    
    String strReadValue = String(readValue);    
    Serial.print("value: ");
    Serial.println(readValue);
    Serial.println("EEPROM: Read end");
    
    return strReadValue;
  }
  else{
    Serial.println("EEPROM ERROR: No value is stored");  
    EEPROM.end();
    return "";
  }
}

void Eeprom::readFirst66Bytes(){
  EEPROM.begin(512);
  for(int i = 0; i < 66; i++){
    char c = EEPROM.read(i);
    Serial.print(i);
    Serial.print(": ");
    Serial.print(c);
    Serial.print(" : ");
    Serial.println((byte)c);
  }
  EEPROM.end();
}
