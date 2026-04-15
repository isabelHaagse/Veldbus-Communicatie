# Veldbus-Communicatie
In deze repository vind je implementaties van verschillende veldbusprotocollen. Alle voorbeelden zijn ontworpen om te werken op twee Arduino Uno’s.

## Instalatie
Benodigdheden:
- 2 Arduino Uno’s of een ander soort Arduino
- 10 jumperkabels male‑male
- 2 datakabels om de code op de Arduino te zetten en te kunnen communiceren
- Arduino IDE

Download het project lokaal door het te clonen via Git. Open de terminal en voer in:

```
git clone https://github.com/isabelHaagse/Veldbus-Communicatie.git
```

Nu staat de repo lokaal op je pc.
Open daarna je bestandsbrowser en zoek de map waar je de repo hebt geplaatst. Open de .ino‑file in de Arduino IDE.
Verbind je Arduino‑board met je pc en run de code op de Arduino.
Open nu nogmaals het andere bestand in een ander tabblad van de Arduino IDE.
Verbind het andere Arduino‑board ook met de pc en run hier eveneens de code.
Open de serial monitor in beide tabbladen en verzend een karakter. Als alles goed is aangesloten, verschijnt het karakter in de andere serial monitor.

De twee verschillende protocollen vereisen verschillende soorten setups. Hieronder wordt het protocol uitgelegd en welke setup daarbij hoort.

## Parallel Synchroon Simplex Verbinding
### Regels van het protocol
Dit protocol heeft de volgende eisen:
- Er kan data verstuurd worden van de ene Arduino naar de andere Arduino, niet andersom.
- De klok wordt als signaal meegegeven.
- De data wordt met meerdere datalijnen op dezelfde klokstand verzonden.

Hieruit zijn de volgende regels voor het protocol opgesteld:
- Er zijn 8 datalijnen die elk een bit in een byte voorstellen.
- Een aparte kloklijn geeft aan wanneer er data wordt verzonden.
- Een hoog signaal geeft een logische 1 aan.
- Data wordt verzonden op een hoog kloksignaal.
- Het kloksignaal blijft dan 100 ms hoog.
- Als er geen data wordt verzonden, is het kloksignaal laag.
- Eerst wordt de data op de lijn geplaatst, vervolgens wordt het kloksignaal hoog gezet.

### Opstelling
Bij deze verbinding worden 8 datakabels gebruikt, daarnaast wordt de klok apart meegegeven.
Gebruik pinnen 2 t/m 9 voor het verzenden van de databits. Deze pinnen moeten op dezelfde pinnen van het andere Arduino‑board worden aangesloten om goede communicatie te krijgen.
Zet de kloklijn op pin 10 van elke Arduino.
Voeg als laatste ook nog een gemeenschappelijke grond toe tussen de twee Arduino’s.

### Code
Voor dit protocol zijn twee verschillende .ino‑bestanden gebruikt.
Aangezien het een simplex‑verbinding is, wordt één .ino‑bestand op de Arduino gezet die data kan verzenden. Het andere .ino‑bestand wordt op de ontvangende Arduino gezet en kan alleen data ontvangen.


## Serieel Asynchroon Half-duplex Verbinding
### Regels van het protocol
Dit protocol heeft de volgende eisen:
- Er kan data verstuurd worden van de ene naar de andere Arduino en andersom, maar niet tegelijk.
- Elke Arduino houdt zelf de kloktijd bij.
- De data wordt achter elkaar verstuurd over één datalijn.

Hieruit zijn de volgende regels voor het protocol opgesteld:
- Een laag signaal geeft een logische 1 aan.
- De data wordt verzonden na het startbit.
- Na het startbit volgt het Meest Significante Bit, gevolgd door de rest van de bits.
- De klok van elke Arduino wordt gestart bij het startbit.
- De data wordt 10 ms op de lijn gezet.
- De baudrate is 100 bits per seconde.
- De Arduino leest de data uit in het midden van de klokperiode.
- Er volgt een stopbit van 10 ms na het verzenden van een byte.
- Elke byte moet apart worden verzonden, telkens startend met een startbit en eindigend met een stopbit.

### Opstelling
Voor deze opstelling is één datakabel nodig, verbonden met pin 2 op beide Arduino’s. Daarnaast moet ook hier een gemeenschappelijke grond verbonden worden tussen de Arduino’s.

### Code
Maak een kopie van het SAHd-Verbinding.ino-bestand en open deze apart in een extra tabblad van de Arduino IDE. Run op beide Arduino’s de code en probeer data van de ene naar de andere Arduino te verzenden.

