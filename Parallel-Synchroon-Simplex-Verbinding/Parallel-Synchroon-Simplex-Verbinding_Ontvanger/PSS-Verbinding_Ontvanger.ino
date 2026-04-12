
/*
  Parallele synchrone simplex verbinding

  Ontvanger
    Lees de waardes in het midden van de ontvangsttijd uit
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


/*
	Functie: read_pins
	Beschrijving:
		Deze functie leest de pinnen uit en berekent de waarde in decimale getallen.
*/

void read_pins(){
  int calcNumber = 128;
  int decimalNotationCharacter = 0;

  for (int i = 0; i < 8; i++){
    // Kijk of de pin een waarde heeft
    if(digitalRead(pinList[i]) == HIGH){
      // Voeg de waarde toe als de pin een waarde heeft
      decimalNotationCharacter += calcNumber;      
    }
    calcNumber /= 2;
  }

  print_character(decimalNotationCharacter);
}


/*
	Functie: print_character
  Parameters:
	  int decimalCharacter = een character in decimale getallen.
	Beschrijving:
		Deze functie print het character in de serial monitor.
*/

void print_character(int decimalCharacter){
  char character = (char) decimalCharacter;
  String stringForSerial = String(character);  // Voor de zekerheid

  Serial.print("\nCharacter send over: ");
  Serial.print(stringForSerial);
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
  // - ASCII pinnen
  for(int i = 0; i < 8; i++){
    pinMode(pinList[i], INPUT);
  }
  // - CLOCKPIN
  pinMode(pinClock, INPUT);
}


/*
	Functie: loop
	Beschrijving:
		De standaard loop functie van Arduino.
*/

void loop() {
  // Controleer of de clock tijd omhoog gaat
  if (digitalRead(pinClock) == HIGH){
    // Wacht tot een stabiele setting van het signaal
    delay(clockHighTime/2);

    read_pins();

    // Wacht tot de clock weer laag wordt
    while(digitalRead(pinClock) == HIGH){
    }
  }
}
