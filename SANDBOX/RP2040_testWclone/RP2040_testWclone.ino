// Test komunikace RP2040 s WiFi modulem ESP8285
// Návrh pro klon Raspbery Pico W 2023
// ========================================================


void setup() {
  Serial.begin(9600);       // UART pro komunikaci USB
  Serial1.begin(115200);    // UART pro komunikaci s ESP8285 (WiFi modem)
  delay(3000);
  Serial.println("Inicializace spojení...");
}

void loop() {
  if(Serial.available()>0) {
    char c = Serial.read();
    Serial1.print(c);
  }
  if(Serial1.available()>0) {
    char c = Serial1.read();
    Serial.print(c);
  }
 
}

/*
---------------------------------------------------------------
Pár příkladů zadaných příkazů v sériovém monitoru vč. odpovědí
---------------------------------------------------------------
AT+GMR
AT version:1.6.2.0(Apr 13 2018 11:10:59)
SDK version:2.2.1(6ab97e9)
compile time:Sep 10 2019 17:31:08
OK

(pozn: před připojením k WiFi je třeba nejdřív nastavit mód modemu)
AT+CWMODE_CUR=3
OK

AT+CWJAP_CUR="moje_ssid","moje_pwd"
WIFI CONNECTED
WIFI GOT IP
OK

AT+CIFSR
+CIFSR:APIP,"192.168.x.x"
+CIFSR:APMAC,"cc:ff:33:dd:66:bb"
+CIFSR:STAIP,"192.168.x.xx"
+CIFSR:STAMAC,"cc:ff:33:dd:66:bb"
OK

AT+UART?
+UART:115273,8,1,0,1
OK
*/
