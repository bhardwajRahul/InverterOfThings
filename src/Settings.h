#include <arduino.h>
#include <EEPROM.h>

class Settings
{
  public:

    bool _valid = false;
    String _wifiSsid = "";
    String _wifiPass = "";
  
    String _batteryApiKey = "";
    String _chargerApiKey = "";
    String _loadApiKey = "";
    String _readConfigApiKey = "";
    String _writeConfigApiKey = "";
  
    void readString(String& s, int maxLen, int offset)
    {
      int i;
      s = "";
      for (i=0; i<maxLen; ++i)
      {
        char c = EEPROM.read(offset+i);
        if (c == 0)
          break;
        s += c;
      }
    }
  
    void writeString(String& s, int maxLen, int offset)
    {
      int i;
      //leave space for null termination
      maxLen--;
      if (s.length() < maxLen-1)
        maxLen = s.length();

Serial.print("Writing ");
Serial.print(maxLen);
Serial.print(" ");
Serial.print(offset);
Serial.print(" ");
Serial.println(s);

      for (i=0; i<maxLen; ++i)
      {
        EEPROM.write(offset+i, s[i]);
        Serial.print(" ");
        Serial.print(offset+i);
        Serial.print("=");
        Serial.print(s[i]);
        Serial.print(",");
      }
      //null terminate the string
      EEPROM.write(offset+i, 0);
      Serial.print(offset+i);
    }
    
    void load()
    {
      EEPROM.begin(512);
  
      _valid = true;
      _valid &= EEPROM.read(0) == 0xDB;
      _valid &= EEPROM.read(1) == 0xEE;
      _valid &= EEPROM.read(2) == 0xAE;
      _valid &= EEPROM.read(3) == 0xDF;

      if (_valid)
      {
        readString(_wifiSsid, 0x20, 0x20);
        readString(_wifiPass, 0x20, 0x40);
        readString(_batteryApiKey, 0x20, 0x60);
        readString(_chargerApiKey, 0x20, 0x80);
        readString(_loadApiKey, 0x20, 0xA0);
        readString(_readConfigApiKey, 0x20, 0xC0);
        readString(_writeConfigApiKey, 0x20, 0xE0);
      }
      
      EEPROM.end();
    }
  
    void save()
    {
      EEPROM.begin(512);
  
      EEPROM.write(0, 0xDB);
      EEPROM.write(1, 0xEE);
      EEPROM.write(2, 0xAE);
      EEPROM.write(3, 0xDF);
  
      writeString(_wifiSsid, 0x20, 0x20);
      writeString(_wifiPass, 0x20, 0x40);
      writeString(_batteryApiKey, 0x20, 0x60);
      writeString(_chargerApiKey, 0x20, 0x80);
      writeString(_loadApiKey, 0x20, 0xA0);
      writeString(_readConfigApiKey, 0x20, 0xC0);
      writeString(_writeConfigApiKey, 0x20, 0xE0);

      EEPROM.commit();

      _valid = true;

      EEPROM.end();    
    }

    Settings()
    {
      load(); 
    }
};

