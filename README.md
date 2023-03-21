# IPK_projekt_1

## Použití

#### Spuštění programu v konzoli:

```
./ipkcpc -h <HOSTNAME> -p <PORT> -m <MODE>
```

```
	&emsp;HOSTNAME - validní doménové jméno, nebo adresa souhlasná s adresou souběžně běžícího programu ipkpd
	&emsp;PORT - číslo portu, které musí souhlasit s číslem portu ipkpd
	&emsp;MODE - je na výběr ze dvou možností: udp, tcp
```

Program se připojí na server zvolený parametrem HOSTNAME přes port v paramentru PORT. Program pošle sadu dotazů a v závislosti na zvoleném módu parametrem MODE vrátí na serveru běžící ipkpd odpovědi, které ipkcpc přijme a vytiskne. Dotazy musí být korektně ukončeny pomocí EOF.

### Módy
```
	UDP
	- přijmá dotazy v podobě ([+-*/] [0-9]+ [0-9]+)
	- výsledek výpočtu tiskne v podobě OK:[-]*[0-9]+
	- v případě zadání příkazu v jiné podobě tiskne chybovou hlášku
	TCP
	- spojení je zahájeno zprávou HELLO
	- během spojení lze poslat libovolný počet zpráv v podobě SOLVE ([+-*/] [0-9]+ [0-9]+)
	- na zprávy SOLVE odpovídá formou RESULT [-]*[0-9]+
	- pro ukončení spojení je třeba poslat zprávu BYE, na kterou reaguje server odpovědí BYE a ukončením spojení a běh programu
	- při zadání nevalidní podoby zprávy je program ukončen chybovou hláškou
```

## Popis

Program načítá argumenty zadané přes konzoli při spuštění programu.
Dále získá pomocí funkce gethostbyname() z knihovny `<netdb.h>` pomocí DNS IP adresu zadaného serveru. Pokud není jméno domény známé, funkce vrací hodnotu NULL a program je ukončen.
Po získání a nastavení adresy serveru je podle zadaného argumentu MODE spuštěna jedna z funkcí udp/tcp.
V jednotlivých funkcích je vytvořen soket, skrz který jsou odesílány a příjmány data. Soket je vytvořen pomocí fuknkce socket() z knihony `<sys/soket.h>`. V závislosti na módu je nastaven typ přenosu dat prostřednictvím promněnné sock_type na SOCK_DGRAM nebo SOCK_STREAM. Po úspěšném vytvoření soketu jsou posílána a tisknuta data.

### Reference

Základní struktura funkcí pro udp a tcp jsou převzaty z následujících zdrojů:

Ryšavý, Ondřej, Ph.D "IPK-DemoUdp." *FIT - VUT Brno - Git*, git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoUdp/client.c
Ryšavý, Ondřej, Ph.D "IPK-DemoTcp." *FIT - VUT Brno - Git*, git.fit.vutbr.cz/NESFIT/IPK-Projekty/src/branch/master/Stubs/cpp/DemoTcp/client.c

