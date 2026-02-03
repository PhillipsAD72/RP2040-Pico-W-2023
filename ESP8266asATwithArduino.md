# Arduino WiFi: Použití ESP8266 s AT příkazy
**Aktualizováno:** 14. října 2023 (překlad článku z https://www.teachmemicro.com/arduino-wifi-using-esp8266-commands/)

[ESP8266](https://www.teachmemicro.com/arduino-wifi-using-esp8266-commands/#) se stal požehnáním pro výrobce, kteří potřebují levně připojit své Arduino k internetu. Desky jako NodeMCU a WeMos, které obsahují modul ESP , se staly nesmírně populárními. To není překvapivé, vzhledem k tomu, že tyto desky mohou nahradit Arduino bez náročného učení.

Takže pokud se v programování Arduina vyznáte a chcete odesílat nebo přijímat data přes internet, doporučuji použít NodeMCU. Vlastně jsem již napsal několik článků o tom, jak to udělat:

-   [Úvod do NodeMCU a Arduino IDE](https://www.teachmemicro.com/intro-nodemcu-arduino/)
-   [Vytvoření jednoduchého webového serveru NodeMCU](https://www.teachmemicro.com/simple-nodemcu-web-server/)
-   [Zobrazení dat ze senzorů na webovém serveru NodeMCU](https://www.teachmemicro.com/display-sensor-data-nodemcu-web-server/)
-   [Protokolování dat pomocí NodeMCU a Tabulek Google](https://www.teachmemicro.com/log-data-nodemcu-google-sheets/)
-   [NodeMCU Ajax: Dynamická data ze senzorů na webové stránce](https://www.teachmemicro.com/nodemcu-ajax-dynamic-sensor-data-web-page/)
-   [Výukový program NodeMCU MQTT](https://www.teachmemicro.com/nodemcu-mqtt-tutorial/)
-   [Webový server s NodeMCU WebSocketem](https://www.teachmemicro.com/web-server-nodemcu-websocket/)
-   [Vytváření přístupového bodu WiFi NodeMCU](https://www.teachmemicro.com/nodemcu-wifi-access-point/)
-   [Referenční přípojka NodeMCU](https://www.teachmemicro.com/nodemcu-pinout/)

Pokud si ale nechcete NodeMCU kupovat a chcete se držet klasického modulu ESP8266, pak je tento tutoriál o Arduinu WiFi určen právě vám.

-   [Zapojení Arduina k ESP8266](#zapojen%C3%AD-arduina-k-esp8266)
-   [AT příkazy](#at-p%C5%99%C3%ADkazy)
-   [Příklad náčrtu WiFi pro Arduino](#p%C5%99%C3%ADklad-n%C3%A1%C4%8Drtu-wifi-pro-arduino)
-   [Odeslání dat na webovou stránku](#odesl%C3%A1n%C3%AD-dat-na-webovou-str%C3%A1nku)
    -   [Odesílání dat prostřednictvím POST požadavku](#odes%C3%ADl%C3%A1n%C3%AD-dat-prost%C5%99ednictv%C3%ADm-post-po%C5%BEadavku)
-   [To je ono!](#to-je-ono)

### **Zapojení Arduina k ESP8266**

Jak víte, Arduino UNO, Mega nebo Nano nemá žádné síťové funkce. Pro připojení Arduina k internetu potřebuje modem. Modul ESP8266 bude fungovat jako náš modem a naváže spojení s vaším WiFi routerem za účelem odesílání/přijímání dat na web/z webu.

![Schéma připojení ESP k Arduinu](https://www.teachmemicro.com/wp-content/uploads/2018/12/img_5c244896a0bb4.png)

Modul ESP8266, konkrétně modul ESP-01, běží na 3,3 V. Proto nesmíme výstupní piny Arduina připojovat přímo k pinům ESP8266. Ve výše uvedeném diagramu jsem použil jednoduchý dělič napětí ke snížení logických úrovní 5 V. Všimněte si také, že pro správný chod modulu ESP8266 musí mít vlastní zdroj napájení 3,3 V a 250 mA. To představuje další nevýhodu tohoto nastavení: musíte mít další zdroj, místo abyste používali pouze USB port počítače.

### **AT příkazy**

Nyní, když jsme připojili modul ESP8266 k Arduinu, je čas odeslat speciální příkazy přes sériový port Arduina ([softwarový](https://www.teachmemicro.com/arduino-wifi-using-esp8266-commands/#)). Tyto AT nebo Hayesovy příkazy jsou sadou příkazů používaných nejen ESP8266, ale i jinými modemy, jako jsou [GSM](https://www.teachmemicro.com/arduino-gsm800l-tutorial/), [Bluetooth](https://www.teachmemicro.com/hc-05-bluetooth-command-list/) a [GPRS](https://www.teachmemicro.com/arduino-gprs/).

Celý seznam AT příkazů je ohromující. Pro náš účel, kterým je připojení k internetu, potřebujeme pouze specifickou sadu AT příkazů. Podrobnější vysvětlení každého příkazu naleznete v našem [seznamu AT příkazů ESP8266](https://www.teachmemicro.com/esp8266-wifi-command-list/).

|příkaz|popis funkce či parametrizace|
|:----------------------:| :--------------------------------------------------------------------------------------------------------------------------------------------------------|
| AT+CIPMUX = 1 |Povolte jedno (0) nebo více připojení (1) k webovému serveru. Více připojení je dobrou volbou, pokud opakovaně odesíláte nebo čtete data z internetu. |
| AT+CWMODE = 3 |Nastavení režimu WiFi: 1 je režim stanice (ESP8266 je klient), 2 je režim AP (ESP8266 funguje jako WiFi router, ke kterému se může připojit váš telefon nebo počítač), 3 je režim AP+stanice (ESP8266 dělá obojí)  |
|  AT+CWJAP = „<vaše-ssid>“, „<vaše-heslo>“ | Připojte se k Wi-Fi. Zadejte název SSID a heslo v uvozovkách.   |
|  AT+CIFSR |Toto vrátí IP adresu modulu, což znamená, že se úspěšně připojil k vašemu WiFi routeru.   |
|  AT+CIPSTART=0, „TCP“,„www.teachmemicro.com“, „80“ | Spuštění TCP nebo UDP připojení. První parametr (0) je zde ID připojení, „TCP“ znamená, že používáme TCP místo UDP, následuje adresa (nebo IP) webového serveru a nakonec číslo portu.   |
| AT+CIPSEND=0,16 |Příkaz, který informuje modul o připravenosti dat k odeslání. „0“ je ID připojení a 16 je délka odesílaných dat. Po tomto příkazu ESP8266 odpoví znakem „>“, který nám sděluje, že čeká na odeslání dat. V případě úspěchu modul odpoví „SEND OK“. |
| „Data k odeslání“ | Řetězcová data, která mají být odeslána.   |

Všimněte si, že tyto příkazy musí být odeslány ve správném pořadí z Arduina do modulu ESP8266.

### **Příklad náčrtu WiFi pro Arduino**

Pomocí výše uvedených AT příkazů nyní můžeme vytvořit jednoduchý HTTP požadavek, který načte webovou stránku ze serveru. Zde je kompletní náčrt:

```c
#include <SoftwareSerial.h>

SoftwareSerial ESP8266(10, 11);  //RX,TX

void setup()
{
    Serial.begin(9600);     
 
    // Start the software serial for communication with the ESP8266
    ESP8266.begin(9600);  //this assumes default baud rate is used by the module
 
    Serial.println("");
    Serial.println("Ready"); 
}
 
void loop()
{
    Serial.println(“Establishing connection type…”);
    ESP8266.println(“AT+CIPMUX=1”);
    delay(1000);
    Serial.println(“Setting WiFI mode…”);
    ESP8266.println(“AT+CWMODE=3”);
    delay(1000);
    Serial.println(“Connecting to WiFi”);
    ESP8266.println(“<your-ssid>”,”<your-pw>”); //insert your own SSID and password here
    delay(1000);
    Serial.println(“Establishing TCP connection”);
    ESP8266.println(“AT+CIPSTART=0,"TCP","www.teachmemicro.com","80”);
    delay(1000);
    Serial.println(“Sending out data…”);
    ESP8266.println(“AT+CIPSEND=0,16”);
    ESP8266.println(“GET / HTTP/1.1”);
    ESP8266.println()    //some extra carriage return
    if ( ESP8266.available() )   {  Serial.write( mySerial.read() );  } //wait for the response and 
//print it out

}
```

Všimněte si, že se jedná o velmi jednoduchý náčrt, který postupně odesílá AT příkazy pro navázání WiFi připojení. Tento náčrt můžeme dále vylepšit tím, že počkáme, až modul ESP8266 na každý příkaz odpoví, a ujistíme se tak, že příkaz skutečně přijal.

### **Odeslání dat na webovou stránku**

Pro odeslání dat, která mají být zveřejněna na webové stránce, potřebujeme PHP skript, který bude „zachytávat“ data odeslaná přes modul ESP8266. Existuje sto způsobů, jak to udělat, ale takto jsem to implementoval já:

Na serveru jsem vytvořil soubor „read.php“, který obsahuje toto:

```php
<?php
 
$value = $_POST['value'];
 
$textfile = "values.txt"; // Declares the name and location of the .txt file
$fileLocation = "$textfile";
$fh = fopen($fileLocation, 'w') or die("Something went wrong!"); // Opens up the .txt file for writing and replaces any previous content
fwrite($fh, $value); // Writes it to the .txt file
fclose($fh);
 
?>
```

Tento skript získává hodnotu proměnné $\_POST a zapisuje ji do dynamicky vytvořeného textového souboru. [POST](https://developer.mozilla.org/en-US/docs/Web/HTTP/Methods/POST) je jeden ze způsobů, jak odeslat data na HTTP server. Zde je příklad POST požadavku, který může webový prohlížeč odeslat:

```php
POST /read.php HTTP/1.1
Host: localhost
User-Agent: Mozilla
Content-Type: application/x-www-form-urlencoded
Content-length: 9
 
value=500
```

Dále musíme přimět  [Arduino](https://www.teachmemicro.com/arduino-wifi-using-esp8266-commands/#) , aby odeslalo tento POST požadavek přes modul ESP8266. Zde je návod, jak jsem to udělal:

```c++
#include <SoftwareSerial.h>

SoftwareSerial ESP8266(10, 11);  //RX,TX

void setup()
{
     
    Serial.begin(9600);     
 
    // Start the software serial for communication with the ESP8266
    ESP8266.begin(9600);  //this assumes default baud rate is used by the module
 
    Serial.println("");
    Serial.println("Ready"); 
}
 
void loop()
{
    int adcval = analogRead(A0);
    Serial.println(“Establishing connection type…”);
    ESP8266.println(“AT+CIPMUX=1”);
    delay(1000);
    Serial.println(“Setting WiFI mode…”);
    ESP8266.println(“AT+CWMODE=3”);
    delay(1000);
    Serial.println(“Connecting to WiFi”);
    ESP8266.println(“<your-ssid>”,”<your-pw>”); //insert your own SSID and password here
    delay(1000);
    Serial.println(“Establishing TCP connection”);
    ESP8266.println(“AT+CIPSTART=0,"TCP","www.teachmemicro.com","80”);
    delay(1000);
    Serial.println(“Sending out data…”);
    ESP8266.println(“AT+CIPSEND=0,16”);
    ESP8266.println(“POST /read.php HTTP/1.1”);
    ESP8266.println(“Host: <your site address or IP>”);    //insert your own site address or IP here
    ESP8266.println(“User-Agent: Mozilla”); //We need this so that the server can identify the sender
    ESP8266.println(“Content-Type: application/x-www-form-urlencoded”);
    ESP8266.println(“Content-length: 9”); //make sure this matches the content that follows
    ESP8266.print(“Value=”);
    ESP8266.println(adcval);
    ESP8266.println();
    ESP8266.println();
}
```

##### **Odesílání dat prostřednictvím POST požadavku**

Tato skica čte analogový port A0 a odesílá hodnotu jako POST požadavek. Všimněte si, že první část skicy je podobná předchozí skice, kterou jsem prezentoval. Tato skica také není tak efektivní, protože odeslání hodnoty by trvalo asi 4 sekundy.

Ještě nejsme hotovi! Potřebujeme další PHP soubor, abychom mohli zobrazit hodnoty odeslané Arduinem:

```php
<!doctype html>
 
<html lang="en">
<head>
  <title>Display Data from Arduino to Web</title>
 <meta http-equiv="refresh" content="5" >
</head>
 
<body>
<?php
$textfile = "values.txt"; // Declares the name and location of the .txt file
//$fileLocation = "$textfile";
$fh = fopen($textfile, 'r') or die("Something went wrong!"); // Opens up the .txt file for writing and replaces any previous content
$value = fread($fh,3);
fclose($fh);
printf("Value is: %s",$value);
?>
</body>
</html>
```

Tento soubor můžete pojmenovat \<what-you-like\>.php. Tato stránka by se měla obnovovat každých 5 sekund a aktualizovat hodnotu odeslanou Arduinem.

### **To je ono!**

Doufám, že jste se naučili, jak používat Arduino s modulem ESP8266 pro připojení k internetu. Na druhou stranu vám i tak doporučuji použít NodeMCU, pokud stavíte složitější WiFi projekt s Arduinem. [Jádro ESP8266 pro Arduino](https://github.com/esp8266/Arduino) se velmi snadno používá a obsahuje spoustu ukázkových skic, které vám práci urychlí.

Pokud ale stále trváte na použití generického modulu ESP8266, existuje způsob, jak ho použít s výše zmíněným jádrem Arduina bez použití samotného Arduina. To by ale bylo na samostatný tutoriál, takže doufejme, že se k němu vrátíte.
