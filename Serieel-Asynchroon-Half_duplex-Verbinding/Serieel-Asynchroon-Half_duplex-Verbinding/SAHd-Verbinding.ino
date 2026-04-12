
/*
  Seriele asynchrone half-duplex verbinding

  Ontvanger en Zender bevatten dezelfde code.
  Een verbinding met 1 fysieke datadraad, de verschillende bits om een byte te vormen voor het karakter worden acher elkaar op de lijn gezet.

  Het dominante signaal geeft een logische 1 aan. 
  In dit geval is ervoor gekozen om het dominante signaal LOW te maken, zodat er een pull-up weerstand gebruikt kan worden en er minder ruis op de lijn ontstaat.

  De data wordt verstuurd met de most significant bit eerst.
  De klok wordt bijgehouden door elke Arduino apart. 1 klokperiode duurt 10 ms, en elke arduino houd die tijd zelf bij nadat het startbit is verzonden.
  
  Ontvangen:
  Als de Arduino een startbit heeft ontvangen zal deze 1.5 keer de klokperiode afwachten, zodat er vanuit gegaan kan worden dat de goede data wordt uitgelezen midden in de klokperiode.
*/


#define DOMINANT LOW
#define SUB HIGH
#define CLOCKTIME 10

int pinDataLine = 2; // Lijn waar de data op komt te staan/op wordt verzonden.


/*
	Functie: read_character
	Beschrijving:
		Deze functie leest de pinnen uit en berekent de waarde in decimale getallen
*/

void read_data(){
  int calcNumber = 128;

  int decimalNotationCharacter = 0;

  for (int i = 0; i < 8; i++){
    // Kijk of de pin een waarde heeft
    if(digitalRead(pinDataLine) == DOMINANT){
      // Voeg de waarde toe als de pin een waarde heeft
      decimalNotationCharacter += calcNumber;      
    }
    calcNumber /= 2;

    // Wacht voor het volgende signaal - Wachttijd = CLOCKTIME
    delay(CLOCKTIME);
  }
}


/*
	Functie: send_character
  Parameters:
	  int decimalCharacter = een karakter in decimale getallen.
	Beschrijving:
		Deze functie print een karakter uit in ASCII.
*/

void print_character(int decimalCharacter){
  char character = (char) decimalCharacter;
  String stringForSerial = String(character);  // Voor de zekerheid

  Serial.print("Het ontvangen character: ");
  Serial.println(stringForSerial);
}


/*
	Functie: send_character
  Parameters:
	  int decimalCharacter = een character in decimale getallen.
	Beschrijving:
		Deze functie zet alle pins op de goede stand zodat in ASCII het goede getal op de draden staat.
*/

void send_character(char characterToSend){
  // Zet het character om in een cijfer
  Serial.print("Het verzonden character: ");
  Serial.println(characterToSend);
  int decimalNotationCharacter = characterToSend;
  int calcNumber = 128;

  // Haal elke keer de grootst mogelijke hoeveelheid eraf en zet op die plaats de pin omhoog
  for (int i = 0; i < 8; i++){
    int rest = decimalNotationCharacter - calcNumber;
    if((rest) >= 0){
      digitalWrite(pinDataLine, DOMINANT);
      decimalNotationCharacter -= calcNumber;
    }else{
      digitalWrite(pinDataLine, SUB);
    }
    calcNumber /= 2;

    // Wacht op de volgende klokperiode
    delay(CLOCKTIME);
  }
}


/*
	Functie: setup
	Beschrijving:
		De standaard setup functie van Arduino.
*/

void setup() {
  // Maak alles klaar voor de serial monitor
  Serial.begin(9600);

  // Zet pin in luister modus
  pinMode(pinDataLine, INPUT_PULLUP);
}


/*
	Functie: loop
	Beschrijving:
		De standaard loop functie van Arduino.
    Check of er data binnenkomt, via de serial verbinding of de serial monitor
*/

void loop() {

  if (digitalRead(pinDataLine) == DOMINANT){
    // Ga lezen

    // Wacht tot het volledige eerste bit van de zender op de lijn staat
    delay((CLOCKTIME * 1.5));

    // Lees de databits uit
    read_data();

  } else if(Serial.available() > 0){
    // Ga zenden
    
    // Zend het startbit op de dataline
    pinMode(pinDataLine, OUTPUT);
    digitalWrite(pinDataLine, DOMINANT);
    delay(CLOCKTIME);

    // Data uit serial
    char characterSerial = Serial.read();

    // Zend de data
    send_character(characterSerial);

    // Zend stopbit
    digitalWrite(pinDataLine, SUB);
    delay(CLOCKTIME);
    pinMode(pinDataLine, INPUT_PULLUP);
  }
}
