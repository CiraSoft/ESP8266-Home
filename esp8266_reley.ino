#include <ESP8266WiFi.h>

// Replace with your network details

const char* ssid = "CiraSoft";
const char* password = "11111111";

// Web Server on port 80
int value = LOW;
int ledPin = BUILTIN_LED;
WiFiServer server(80);

byte relON[] = { 0xA0, 0x01, 0x01, 0xA2 };
byte relOFF[] = { 0xA0, 0x01, 0x00, 0xA1 };

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(9600);
  delay(10);


   pinMode(ledPin, OUTPUT);
   digitalWrite(ledPin, LOW);
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.hostname("DnevnaSoba-Grejanje");
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000);
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
}

// runs over and over again
void loop() {
  // Listenning for new clients
  WiFiClient client = server.available();
  
  if (client) {
    Serial.println("New client");
    // bolean to locate when the http request ends
    boolean blank_line = true;
    while (client.connected()) {
      if (client.available()) {
        // Read the first line of the request
          String request = client.readStringUntil('\r');
          client.flush();
         
          // Match the request
         
          if (request.indexOf("/rel=on") != -1) {
            digitalWrite(ledPin, HIGH);
            Serial.write(relON, sizeof(relON));
          } 
          if (request.indexOf("/rel=off") != -1){
            digitalWrite(ledPin, LOW);
            Serial.write(relOFF, sizeof(relOFF));
          }
          value=digitalRead(ledPin);
           String s = "HTTP/1.1 200 OK\r\n";
                    s += "Content-Type: application/json\r\n\r\n";
                    s += "{\"data\":{\"message\":\"success\",\"type\":\"reley\",\"state\":\"" +  String(value) + "\"}}";
                    s += "\r\n";
                    s += "\n";

            client.println(s);
            break;

      }
    }  
    delay(1);

    Serial.println("Client disconnected.");
  }
}   
