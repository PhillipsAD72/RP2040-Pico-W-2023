# Webserver jen s příkazy pro AT modem
**Zadání pro AI Gemini:**

Dokážeš mi vygenerovat kód pro RP2040, který bude zobrazovat webovou stránku s nějakým textem a hodnotami z analogových vstupů? Pro přístup k wifi je k UART0 RP2040 připojen AT modem s ESP8285 a FW v2. Chtěl bych, aby kód využíval jen AT příkazy, bez použití externí knihovny.

**Odpověď:**

Toto řešení je poměrně nízkoúrovňové, protože komunikace s AT modemem (ESP8285) bez knihovny vyžaduje ruční obsluhu stavů a odesílání přesných délek dat.
RP2040 (pomocí UART0 na pinech GP0/GP1) bude posílat příkazy typu AT+CIPSEND, aby doručil HTML kód prohlížeči.
Schéma zapojení
RP2040 GP0 (TX) -> ESP RX
RP2040 GP1 (RX) -> ESP TX
GND -> GND (společná zem je nutná!)

**Kód pro Arduino IDE (Philhower jádro):**

```cpp
#include <Arduino.h>
#define ESP_SERIAL Serial1  // UART0 na RP2040 (GP0/GP1)
#define SSID "Tvoje_WiFi"
#define PASS "Tvoje_Heslo...
```

**Klíčové body implementace:**

1. **AT+CIPMUX=1:** Server na ESP vyžaduje režim více spojení, aby mohl identifikovat, kdo se ptá.
2. **+IPD:** Když někdo přistoupí na IP adresu ESP, modem pošle do RP2040 řetězec začínající +IPD,ID,délka:požadavek. Kód tento řetězec zachytí a zjistí ID klienta.
3. **Délka dat:** Příkaz AT+CIPSEND je kritický – musíte mu říct přesný počet bajtů, které následně pošlete. Pokud pošlete víc nebo míň, ESP se "zasekne" nebo ořízne data.
4. **Meta refresh:** Do HTML jsem přidal content='5', aby se stránka každých 5 sekund sama aktualizovala a vy jste viděli změny na analogových pinech.

**Poznámka k napájení:** ESP8285 má v špičkách při vysílání velký odběr (přes 200mA). Pokud se ESP restartuje, ujistěte se, že ho nenapájíte jen z 3.3V pinu RP2040, ale z dostatečně silného zdroje.

### Moje poznámky:
Kód zatím není otestovaný.
