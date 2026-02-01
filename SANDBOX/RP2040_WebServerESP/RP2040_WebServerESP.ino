/*
 Výchozí kód:
 WiFiEsp example: WebServer
 Upraveno pro RP2040 Pico W-2023

 A simple web server that shows the value of the analog input 
 pins via a web page using an ESP8266 module.
 This sketch will print the IP address of your ESP8266 module (once connected)
 to the Serial monitor. From there, you can open that address in a web browser
 to display the web page.
 The web page will be automatically refreshed each 20 seconds.

 For more details see: http://yaab-arduino.blogspot.com/p/wifiesp.html
*/

#include "WiFiEsp.h"

char ssid[] = "####";            // your network SSID (name)
char pass[] = "****";            // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int reqCount = 0;                // number of requests received

WiFiEspServer server(80);

void setup()
{
  // initialize serial for debugging
  Serial.begin(115200);
  // initialize serial for ESP module
  Serial1.begin(115200);
  // initialize ESP module
  WiFi.init(&Serial1);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield nenalezen");
    // don't continue
    while (true);
  }

  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) {
    Serial.print("Připojuji se k síti pomocí WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network
    status = WiFi.begin(ssid, pass);
  }

  Serial.println("Modul je připojen k síti");
  //delay(3000);
  printWifiStatus();
  
  // start the web server on port 80
  server.begin();
}

+region
void loop()
{
  // listen for incoming clients
  WiFiEspClient client = server.available();
  if (client) {
    Serial.println("New client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          Serial.println("Sending response");
          
          // send a standard http response header
          // use \r\n instead of many println statements to speedup data send
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"  // the connection will be closed after completion of the response
            "Refresh: 20\r\n"        // refresh the page automatically every 20 sec
            "\r\n");
          client.print("<!DOCTYPE HTML>\r\n");
          client.print("<html>\r\n");

          client.print("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\r\n");
          client.print("<title>ESP32 Weather Report</title>\r\n");
          client.print("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\r\n");
          client.print("body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;}\r\n");
          client.print("p {font-size: 24px;color: #444444;margin-bottom: 10px;}\r\n");
          client.print("</style>\r\n");
          client.print("</head>\r\n");
          client.print("<body>\r\n");
          client.print("<div id=\"webpage\">\r\n");
          client.print("<h1>ESP32 Weather Report</h1>\r\n");
  
          client.print("<p>Temperature: ");
          client.print(analogReadTemp());
          client.print("°C</p>");
          client.print("<p>Humidity: ");
          client.print(analogRead(0));
          client.print("%</p>");
  
          client.print("</div>\r\n");
          client.print("</body>\r\n");
          client.print("</html>\r\n");

          client.print("<h1>Hello World!</h1>\r\n");
          client.print("Requests received: ");
          client.print(++reqCount);
          client.print("<br>\r\n");
          // output the value of analog input pins
          for (int analogChannel = 0; analogChannel < 4; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            client.print("analog input ");
            client.print(analogChannel);
            client.print(" is ");
            client.print(sensorReading);
            client.println("<br />");
          }
          client.print("Teplota MCU: ");
          client.print(analogReadTemp());
          client.print("°C<br>\r\n");

          String ptr = "======================================<br>";
          ptr +="=1====================================<br>";
          ptr +="===2==================================<br>";
          ptr +="=====3================================<br>";
          ptr +="=======4==============================<br>";
          ptr +="=========5============================<br>";
          ptr +="===========6==========================<br>";
          ptr +="=============7========================<br>";
          ptr +="===============8======================<br>";
          client.print(ptr);
          client.print("</html>\r\n");

          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(10);

    // close the connection:
    client.stop();
    Serial.println("Client disconnected");
  }
}


void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
/*  IPAddress ip = WiFi.localIP();  Příkaz nefunguje, pokud je ponechán, program se zde kousne
  Serial.print("IP Address: ");
  Serial.println("ip"); // proměnná vložena do uvozovek
  
  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println("ip"); // proměnná vložena do uvozovek
  Serial.println();*/
  Serial.println("");
  Serial.println("IP adresa webserveru:");
  Serial.println("=====================");
  printIPadr();
}
void printIPadr() {
  // tisk IP adresy serveru - náhrada za nefunkční příkaz WiFi.localIP
  Serial1.println("AT+CIFSR");
  delay(100);
  while(Serial1.available()>0) {
    char c = Serial1.read();
    Serial.print(c);
  }
}
