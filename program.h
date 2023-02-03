// działa OK dodane zapamiętanie do 10 odczytów


//int els_pin = 12;// czujnik
int els_pin = 13;// symulacja przełącznikiem
bool pool_int = 0; // wybor metody pomiaru impulsu 0 pooling; 1 interrupt

volatile long pulseCount;   //Used to measure global power.
volatile long pulseCountperiod;   //Used to measure power period
volatile unsigned long pulseTime, lastTime;
volatile double power, elapsedkWh, elapsedkWhperiod; //power and energy

int ppwh = 1; //1000 pulses/kwh = 1 pulse per wh
double mocmin = 20000;
double mocmax = 0;


void onPulse() // The interrupt routine pomiar impulsu przerwaniem
{
  if ((micros() - pulseTime) > 200000) {//filtr krótkich impulsów moc > 20 kW
    lastTime = pulseTime;
    pulseTime = micros();
    pulseCount++;
    pulseCountperiod++;
    power = (3600000000.0 / (pulseTime - lastTime)) / ppwh; //Calculate power
    elapsedkWh = (1.0 * pulseCount / (ppwh * 1000)); //count energy in kWh
    elapsedkWhperiod = (1.0 * pulseCountperiod / (ppwh * 1000)); //count energy in kWh
    Serial.print("In ");   Serial.print((pulseTime - lastTime)); Serial.print("  moc "); Serial.print(power, 2);  Serial.print(" W / energia ");  Serial.print(elapsedkWh, 3); Serial.println(" kWh");
  }
}

void set_p_i() { //wył/zał przerwania
  if (!pool_int) {
    detachInterrupt(els_pin);
  } else {
    attachInterrupt(els_pin, onPulse, FALLING);//RISING
  }
}

void programsetup() { //przerwanie czy pooling
  pinMode(els_pin, INPUT_PULLUP);
  set_p_i();
}
void minmax() { //ustawianie wskazań min i max odczytanej energii
  if ( power > mocmax)mocmax = power;
  if ( power < mocmin)mocmin = power;
}

void program_timer() {}

void send_to_virtuino()
{
  minmax();
  sendValue(pin10, String(power));
  sendValue(pin11, String(elapsedkWh));
  sendValue(pin12, String(pulseCount));
  sendValue(pin13, String(elapsedkWhperiod));
  sendValue(pin14, String(mocmax));
  sendValue(pin15, String(mocmin));
}

int liczba_danych = 0;
void Zapisz_dane_GET (double v_moc) { // zapisuje do serwera 10 pomiarów mocy w momencie wywołania tej pocedury
  liczba_danych = liczba_danych + 1;
  if (liczba_danych > 10) {
    GET_REST_reset();                 // kasuje dane na serwerze po 10 zapisach
    liczba_danych = 0;
  } else GET_REST(v_moc);
}

byte nowy_get = 0;
void get_timer() {                // zapis pomiaru mocy co 60 sek
  if(nowy_get)Zapisz_dane_GET(power);
  nowy_get=0;
}
byte stan_els = 0;
unsigned long time_els = 0;
void programakcja() {  //pomiar impulsu w poolingu
  unsigned  long duration;
  if (!pool_int) {
    if (stan_els == digitalRead(els_pin)); else { //czy stan na pinie zmienil sie
      stan_els = digitalRead(els_pin);
      if (stan_els) {                             //czy stan wysoki
        if ((micros() - time_els) > 200000)       //eliminacja fałszywych impulsów
        {
          unsigned long   temp_time_els = micros(); //millis();
          duration = temp_time_els - time_els;   //dlugosc impulsu
          time_els = temp_time_els;  // zapamietanie czasu
          power = (3600000000.0 / ((duration)));  //przeliczenie na kWh
          pulseCount++;  //licznik impulsow
          elapsedkWh = (1.0 * pulseCount / (ppwh * 1000)); //count energy in kWh
          pulseCountperiod++;
          elapsedkWhperiod = (1.0 * pulseCountperiod / (ppwh * 1000)); //count energy in kWh
          nowy_get = 1;
          send_to_virtuino();
          Serial.print("P ");    Serial.print((duration)); Serial.print("  moc "); Serial.print(power);  Serial.print(" W / energia ");  Serial.print(elapsedkWh, 3); Serial.println(" kWh");
        }
      }
    }
  }
}


/*
// ustawienie pooling/ przerwanie z  virtuino - test aktywuje się po resecie
  {
  pool_int =  value.toInt();
  set_p_i();
  }
*/

void pin16_rec(String value) //reset licznika impulsów dla  pomiaru energii w okresie
{
  int v = value.toInt();
  if (v == 1)   {
    pulseCountperiod = 0;
    mocmin = 20000;
    mocmax = 0;
    
  }
}



//----------------------------------nie używana
int hour = 10;
int minute = 10;
void time_compare() {
  if (hour == timeClient.getHours() && minute == timeClient.getMinutes()) {
    //    digitalWrite(led, 0);
  }
}
