## Pískoviště pro tvorbu prvních pokusů

V této části repozitáře jsou uloženy první vytvořené pokusné kódy pro testování funkcí modemu.

1. První příklad **RP2040_testWclone** provádí kopii komunikace s RP2040 na UART0 čipu ESP8285, čímž umožní přeposlat komunikaci přijatou na USB vývojové desky do AT modemu tvořeného tímto ESP.
2. Druhý příklad **RP2040_WebServerESP** je pak první pokus o vytvoření webserveru založeného na této vývojové desce.
3. Plány do budoucna: použitá knihovna WiFiEsp.h je zastaralá, ale s WiFiEspAT.h mi deska nespolupracovala, takže plánuji zkusit změnit firmware AT modemu a novější knihovnu nějak zprovoznit.
