#include <Arduino.h>

#define ESP_SERIAL Serial1  // UART0 na RP2040 (GP0/GP1)
#define SSID "Tvoje_WiFi"
#define PASS "Tvoje_Heslo"

void posliAT(String prikaz, int timeout = 2000) {
  ESP_SERIAL.println(prikaz);
  long int cas = millis();
  while ((cas + timeout) > millis()) {
    while (ESP_SERIAL.available()) {
      Serial.write(ESP_SERIAL.read()); // Debug do PC
    }
  }
}

void setup() {
  Serial.begin(115200);     // Debug do PC přes USB
  ESP_SERIAL.begin(115200); // Komunikace s ESP8285

  delay(2000);
  Serial.println("Inicializace ESP...");

  posliAT("AT+RST");              // Restart
  delay(2000);
  posliAT("AT+CWMODE=1");         // Režim klient
  posliAT("AT+CWJAP=\"" SSID "\",\"" PASS "\""); // Připojení k WiFi
  delay(5000);
  posliAT("AT+CIPMUX=1");         // Povolení více spojení
  posliAT("AT+CIPSERVER=1,80");   // Spuštění serveru na portu 80

  Serial.println("Server běží. Zjisti IP příkazem AT+CIFSR v konzoli.");
}

void obsluzWeb() {
  if (ESP_SERIAL.available()) {
    if (ESP_SERIAL.find("+IPD,")) { // Detekce příchozího HTTP požadavku
      delay(300);
      int connectionId = ESP_SERIAL.read() - 48; // Získání ID spojení

      // Čtení hodnot z analogových vstupů
      int a0 = analogRead(26);
      int a1 = analogRead(27);

      // Příprava HTML obsahu
      String html = "<html><head><meta charset='UTF-8'><meta http-equiv='refresh' content='5'></head>";
      html += "<body><h1>RP2040 Web Server</h1>";
      html += "<p>Analog A0 (GP26): <b>" + String(a0) + "</b></p>";
      html += "<p>Analog A1 (GP27): <b>" + String(a1) + "</b></p>";
      html += "</body></html>";

      // AT příkaz pro odeslání dat: AT+CIPSEND=<id>,<delka>
      String cipSend = "AT+CIPSEND=";
      cipSend += connectionId;
      cipSend += ",";
      cipSend += html.length();

      posliAT(cipSend, 500);
      ESP_SERIAL.print(html); // Odeslání samotného HTML
      delay(100);
      
      // Uzavření spojení
      String closeCommand = "AT+CIPCLOSE=";
      closeCommand += connectionId;
      posliAT(closeCommand, 500);
    }
  }
}

void loop() {
  obsluzWeb();
}
