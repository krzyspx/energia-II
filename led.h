#define led_ziel 16      //program OK miganie led
#define sw   13
#define led_wew   2
#define led_czerw   0
int Tmoc = 10;

void ledsetup() {
  pinMode(led_czerw, OUTPUT); //LED na module
  digitalWrite(led_czerw, HIGH);

  pinMode(led_wew, OUTPUT); //LED na module
  digitalWrite(led_wew, LOW);

  pinMode(led_ziel, OUTPUT); //LED na module
  digitalWrite(led_ziel, HIGH);

  pinMode(sw, INPUT_PULLUP);
}

int flaga_led = 0;
void led_timer()
{
  flaga_led = !flaga_led;
  if (flaga_led)  akcja.updateInterval(2, (Tmoc * 100)); else akcja.updateInterval(2, 50);
  digitalWrite(led_czerw, flaga_led);
}

int  stanswOnOff = 1;
void swOnOff() {
  if (stanswOnOff != digitalRead(sw))sendValue(pin1, String(stanswOnOff)); // sw esp ----> app
  stanswOnOff = digitalRead(sw);
}

void connetion_test() {
  if (WiFi.status() != WL_CONNECTED)   digitalWrite(led_wew, LOW);  else digitalWrite(led_wew, HIGH); //wł LED gdy nie ma WiFi
}

int wsk_led = 0;
void led_app_timer()  // esp -----> app every 3000 ms
{
  wsk_led = !wsk_led;
  sendValue(pin0, String(wsk_led)); //esp ----> app
  get_time_ntp();
  swOnOff();
  connetion_test();
}

void pin6_rec(String value) //control LED in ESP for test
{
  int v = value.toInt();
  if (v == 1) digitalWrite(led_wew, HIGH); else digitalWrite(led_wew, LOW);
}
