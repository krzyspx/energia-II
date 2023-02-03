bool sendValue(const char* tag, String value);
bool sendValue(const char* tag, String value);

//====== predefinicje funkcji z innych bloków programu

void pin6_rec(String value); //
//void pin15_rec(String value);
void pin16_rec(String value) ; //reset licznika

//======= deklaracje wartości początkowych

int pin11_lastValue = 0;

// deklaracje pinów łączących ESP i APP
// ESP -------> app
const char* pin0 = "V0"; //led
const char* pin1 = "V1";  //przelacznik
const char* pin2 = "V2";
const char* pin3 = "V3";   //wys kod
const char* pin4 = "V4";   //temp
const char* pin5 = "V5";
// app ------> ESP
const char* pin6 = "V6";
//------------------------------zmienne programu
const char* pin10 = "V10";  //power >app
const char* pin11 = "V11";  //elapsedkWh  esp>app
const char* pin12 = "V12";  //pulseCount  esp>app
const char* pin13 = "V13";  //elapsedkWhperiod >> app
const char* pin14 = "V14";  //mocmax >> app
const char* pin15 = "V15";  // zał pompy esp<app
const char* pin16 = "V16";  // reset licznika



//===================================================== sendPinStatus
// It is called every time a new client is connected.
// The void informs Virtuino app about the current pin states and variable values.

void sendPinsStatus() {
  sendValue(pin11, String(pin11_lastValue));    // send pin11 value

}

//===================================================== onValueReceived
// It is called every time a new value received  - add ---> esp

void onValueReceived(String tag, String value) {
  Serial.println("Received: tag=" + tag + "  value=" + value);

  if (tag == "V6") pin6_rec(value); // led in esp

  //  if (tag == "V15") pin15_rec(value);  // 
  if (tag == "V16") pin16_rec(value);  // reset licznika
}
