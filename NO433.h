
#include <RCSwitch.h> // biblioteka obsługi nadawania i odbioru 433 MHz
RCSwitch mySwitch = RCSwitch();
#define ESP01 1
#define D1MINI 2
int procesor = D1MINI;
unsigned long o_kod = 0;


int printon = 0; // flaga wysyłania danych na monitor
int terminalon = 0; // flaga wysyłania danych na terminal BLYNK

void pin16_rec(String value) {//button
  terminalon = value.toInt();
}

void no433setup()
{
  if (procesor == D1MINI) {
    mySwitch.enableReceive(12);  // port odbiornika
    mySwitch.enableTransmit(14); // nadajnika
  }
  if (procesor == ESP01) {
    mySwitch.enableReceive(3);  //
    mySwitch.enableTransmit(1); //
  }
}

void no433akcja()
{
  
  if (mySwitch.available()) {

     String str = String(("Kod " + String(mySwitch.getReceivedValue()) + " / " + String(mySwitch.getReceivedBitlength()) + " IMP " + String(mySwitch.getReceivedDelay())
                   + " Prot  "  + " / " + String(mySwitch.getReceivedProtocol()) ));
  
    //String L1 = "1 line";
    //String L2 = "2 line";
    //String str = String(L1 + '\n'+'\r' + L2);
    //sendValue(pin3, str);
    //Serial.print(str);

    if (terminalon) {// przełącznik wysyłania info na terminal
      sendValue(pin3, str);
    }

    sendValue(pin3, str);
    Serial.println(str);

    o_kod = mySwitch.getReceivedValue();

    mySwitch.disableReceive();
// tu sprawdzić odebrany kod
    mySwitch.resetAvailable();
    no433setup(); 
  } 
}

void sendkod(unsigned long code, int impuls, byte bity, byte prot) { // wysłanie kodu 
  mySwitch.disableReceive();

  mySwitch.setPulseLength(impuls);
  mySwitch.setRepeatTransmit(10);
  mySwitch.send(code, bity);

  no433setup();
}

void sendkodpilot (unsigned long code) {
  sendkod(code, 340, 24, 1);
}
