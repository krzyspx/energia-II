#include <EEPROM.h>

#define memory 100
int timeon1 = 0;
int timeon2 = 0;
int timeoff = 0;
void save_data () { //zapisz dane do flash

  EEPROM.begin(1024);
  EEPROM.put(memory, timeon1);
  //  EEPROM.put((memory + 10), timeoff);
  //  EEPROM.put((memory + 20), timeon2);
  //    EEPROM.put(memory, dane2);
  EEPROM.end(); //  EEPROM.commit();
}

void read_data() { // odczyt z flash

  EEPROM.begin(1024);
  EEPROM.get(memory, timeon1);
  //  EEPROM.get((memory + 10), timeoff);
  // EEPROM.get((memory + 20), timeon2);
  //  EEPROM.get(memory, dane2);
  EEPROM.end(); //  EEPROM.commit();
}

//-----------------
//eeprom adr
#define adrauth 200
#define adrssid 250
#define adrpass 300
#define adrflagawifi 1
const byte defwifi = 3;
const byte  setwifi = 6;
int xxx = 0;
void savewifi()
{
  EEPROM.begin(512);
  EEPROM.put(adrauth, xxx);
  EEPROM.put(adrssid, xxx);
  EEPROM.put(adrpass, xxx);
  //  EEPROM.commit();
  EEPROM.end();
}

void settypewifi(byte set) { // save mode login flag
  EEPROM.begin(512);
  int addr = adrflagawifi;
  EEPROM.put(addr, set);
  //  EEPROM.commit();
  EEPROM.end();
}

void printread() { // print login data from EEPROM
  EEPROM.begin(512);
  EEPROM.get(adrauth, xxx);
  EEPROM.get(adrssid, xxx);
  EEPROM.get(adrpass, xxx);
  byte setp;
  EEPROM.get(adrflagawifi, setp);
  //EEPROM.commit();
  EEPROM.end();

}
