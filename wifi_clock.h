// blok wifi + NTP + REST API
#include <ESP8266WiFi.h>
#include <NTPClient.h>      // for clock NTP
#include <WiFiUdp.h>      // for clock NTP

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

const char* ssid = "xxxxxx";                  // WIFI network SSID
const char* password = "xxxxxxxxxx";        // WIFI network PASSWORD
String nazwa_ESP = String("ESP_pompa");     // nazwa ESP w DHCP

IPAddress ip(192, 168, 2, 170);            // where 150 is the desired IP Address. The first three numbers must be the same as the router IP
IPAddress gateway(192, 168, 2, 1);
IPAddress subnet(255, 255, 255, 0);        // set subnet mask to match your network
IPAddress ip4 = ipaddr_addr( "8.8.8.8" );  //dns 1
IPAddress ip5 = ipaddr_addr( "8.8.4.4" );  //dns 2

#define led_wew   2
//===================================================== connectToWiFiNetwork

void connectToWiFiNetwork() {
  Serial.println("Connecting to " + String(ssid));
  WiFi.mode(WIFI_STA);
  WiFi.hostname(nazwa_ESP);
  WiFi.config(ip, gateway, subnet, ip4, ip5);          // If you don't want to config IP manually disable this line
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  digitalWrite(led_wew, HIGH); //wył LED wew jako potwierdzenie połączenia z WiFi

  Serial.print("\nWiFi connected   ");
  Serial.println(WiFi.localIP());              // Insert this IP on Virtuino IoT server settings
  Serial.print("\nHost name   ");
  Serial.println(WiFi.hostname());

}

void ntpsetup() {
  timeClient.begin();
  // dopasowanie strefy czasowej
  // GMT +1 = 3600
  // GMT -1 = -3600
  timeClient.setTimeOffset(3600);   //przesunięcie na czas PL
}

void get_time_ntp() {               //pobranie aktualnych danych zegara NTP
 /*
  timeClient.update();

  String formattedTime = timeClient.getFormattedTime();
  Serial.print("Formatted Time: ");
  Serial.println(formattedTime);

  int currentHour = timeClient.getHours();
  Serial.print("Hour: ");
  Serial.println(currentHour);

  int currentMinute = timeClient.getMinutes();
  Serial.print("Minutes: ");
  Serial.println(currentMinute);

  int currentSecond = timeClient.getSeconds();
  Serial.print("Seconds: ");
  Serial.println(currentSecond);
  */
}


const char* host = "esp8266restapi.000webhostapp.com"; // only esp8266restapi.000webhostapp.com not https://esp8266restapi.000webhostapp.com

void GET_REST(double v_moc) {

  Serial.print("connecting to ");
  Serial.println(host);

  WiFiClientSecure client; // Use WiFiClient class to create TCP connections
  const int httpPort = 443; // 80 is for HTTP / 443 is for HTTPS!
  
  client.setInsecure(); // this is the magical line that makes everything work
  
  if (!client.connect(host, httpPort)) { //works!
    Serial.println("connection failed");
    return;
  }

  String url = "/dwa.php?czujnik=moc_%5BW%5D&wartosc=" + String(v_moc);    // We now create a URI for the request

  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +          // This will send the request to the server
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  Serial.println();
  Serial.println("closing connection");
}

void GET_REST_reset() {

  Serial.print("connecting to ");
  Serial.println(host);

  WiFiClientSecure client; // Use WiFiClient class to create TCP connections
  const int httpPort = 443; // 80 is for HTTP / 443 is for HTTPS!
  
  client.setInsecure(); // this is the magical line that makes everything work
  
  if (!client.connect(host, httpPort)) { //works!
    Serial.println("connection failed");
    return;
  }

  String url = "/cztery.php";    // We now create a URI for the request

  Serial.print("Requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +          // This will send the request to the server
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");

  Serial.println();
  Serial.println("closing connection");
}
