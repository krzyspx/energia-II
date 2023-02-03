
#include <WebSocketsServer.h>
WebSocketsServer webSocket = WebSocketsServer(8000); // Default Virtuino Server port = 8000

void sendPinsStatus(); 
void onValueReceived(String tag, String value); 

//===================================================== webSocketEvent
//This is the server handler. It receives all the messages

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;
    case WStype_CONNECTED: {
        IPAddress ip = webSocket.remoteIP(num);
        Serial.printf("[%u] New client connected - IP: %d.%d.%d.%d \n", num, ip[0], ip[1], ip[2], ip[3]);
        sendPinsStatus();         // send the initial pin and variable values to the connected clients
        break;
      }
    case WStype_TEXT:  // a new message received
      Serial.printf("[%u] Received: %s\n", num, payload);
      //-- The incoming payload is a json message. The following code extracts the value from json without extra library
      String str = (char*)payload;
      int p1 = str.indexOf("{\"");
      if (p1 == 0) {
        int p2 = str.indexOf("\":");
        if (p2 > p1) {
          String tag = str.substring(p1 + 2, p2);
          p1 = str.indexOf(":\"", p2);
          if (p1 > 0) {
            p2 = str.lastIndexOf("\"}");
            if (p2 > 0) {
              String value = str.substring(p1 + 2, p2);
              onValueReceived(tag, value);
            }
          }
        }
      }
      break;
  }
}

//===================================================== sendValue
// This function sends a value to a Tag.
// The tag and the value are converted to a json message before sending

bool sendValue(const char* tag, String value) {
  String json = "{\"";
  json += tag;
  json += "\":\"";
  json += value;
  json += "\"}";
  Serial.println("Send: " + json);
  return webSocket.broadcastTXT(json);     // This function sends the message to all connected clients.
}

//============================================================== vDelay
void vDelay(int delayInMillis) {
  long t = millis() + delayInMillis;
  while (millis() < t) webSocket.loop();
}
