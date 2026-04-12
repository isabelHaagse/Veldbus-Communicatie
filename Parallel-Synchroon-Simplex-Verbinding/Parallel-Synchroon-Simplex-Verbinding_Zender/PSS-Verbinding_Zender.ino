
/*
  Parallele synchrone simplex verbinding
  
  Zender
    Verzend de waardes als de klok HIGH is.
    Als er geen waardes worden verzonden ook geen klok / HIGH en de waardes van de ASCII pins op LOW.
  Algemeen
    Een verbinding met 8 fysieke datadraden en een klokdraad. 
    De databits worden uitgelezen als de klok op HIGH staat.
    De databit representeerd een 1 als het bit op HIGH staat.
    De klok blijft per keer 100 ms op HIGH staan.
    Na 100 ms blijft de klok minimaal 100 ms op LOW staan.
    Als er geen data wordt verzonden, staat de klok op LOW.
*/


int clockHighTime = 100;
int pinList[] = {2,3,4,5,6,7,8,9};
int pinClock = 10;
char characterToSend = '\0';


/*
	Functie: print_character
  Parameters:
	  char characterToSend = het karakter dat verzonden moet worden.
	Beschrijving:
		Deze functie zet alle pins op de goede stand zodat in ASCII het goede getal op de draden staat.
*/

void send_character(char characterToSend){
  // Zet het character om in een decimaal cijfer
  int decimalNotationCharacter = characterToSend;
  int calcNumber = 128;

  // Haal elke keer de grootst mogelijke hoeveelheid eraf en zet op die plaats de pin omhoog
  for (int i = 0; i < 8; i++){
    int rest = decimalNotationCharacter - calcNumber;
    if((rest) >= 0){
      digitalWrite(pinList[i], HIGH);
      decimalNotationCharacter -= calcNumber;
    }
    calcNumber /= 2;
  }
}


/*
	Functie: reset_pins
	Beschrijving:
		Deze functie zet alle pins terug naar beneden.
*/

void reset_pins(){
  for(int i = 0; i < 8; i++){
    digitalWrite(pinList[i], LOW);
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
  
  // Zet de pinnen klaar voor gebruik
  // ASCII pinnen
  for(int i = 0; i < 8; i++){
    pinMode(pinList[i], OUTPUT);
  }
  // CLOCKPIN
  pinMode(pinClock, OUTPUT);
}


/*
	Functie: loop
	Beschrijving:
		De standaard loop functie van Arduino.
*/

void loop() {

  // Kijk of er iets in de serial monitor zit
  if (Serial.available() > 0){
    // Pak het teken uit de serial monitor
    String serialInput = Serial.readStringUntil('\n');
    characterToSend = serialInput.charAt(0);
    Serial.print("\nSend character: ");
    Serial.print(characterToSend);    
  }


  // Kijk of er een teken is die verzonden moet worden, als het zo is verzend deze dan ook en wacht tot een nieuw character verzonden kan worden
  if (characterToSend != '\0'){
    // Zo ja, zet de pinnen op het juiste teken + zet de klok voor het zenden omhoog

    // Zet de klok omhoog
    digitalWrite(pinClock, HIGH);

    // Zet de juiste waardes op de pinnen
    send_character(characterToSend);

    // Wacht tot het volgende signaal gelezen kan worden
    delay(clockHighTime);

    // Zet de clock weer laag
    digitalWrite(pinClock, LOW);
    
    // Haal de waardes weer van de pinnen naar de 0 setting
    reset_pins();

    // Zet character weer op ASCII NULL, zodat het vorige teken niet de hele tijd verzonden wordt.
    characterToSend = '\0';

    // Wacht
    delay(clockHighTime);
  }
}
