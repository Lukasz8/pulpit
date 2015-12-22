/*
SP8XCE & SQ9MDD
PULPIT 

DATA FRAME FORMAT
  // format ramki: XXYYZ
  // XX - adres docelowy odbiornika zakres 10-31
  // YY - adres wyjścia na odbiorniku zakres 10-99 10-wyjscie wirtualne czy żyjesz 11-pierwsze wyjście fizyczne
  // Z  - komenda 1 - ON 0 - OFF

CHANGELOG
2015.12.22 - poprawiono odwolanie do tabliczy sekwencji recznej, dodano opisy do pojedynczych klawiszy
2015.12.10 - drugi przycisk steruje sekwencją ręczną
2015.12.06 - mozliwosc wpisania sekwencji 16x3, linia 44 konczy pokaz totez wazne aby sie stan zgadzal z tablica !! PAMIETAJ Że TABLICE MUSZA MIEC TE SAME ILOŚCI ZMIENNYCH!!!
2015.12.05 - wyzwalanie sekwencji odpalenia
2015.12.05 - obsługa wysyłki ramki sterującej po przycisnieciu przycisku
2015.11.29 - dolozenie obslugi przyciskow i funkcji ich kalibracji
2015.11.26 - format ramki danych sprawdzenie poprawności odbioru danych z remote'a
2015.11.24 - poprawki i synchronizacja kodu
*/

//wysyłka danych oraz potwierdzenie odbioru
#include <SPI.h>
#include <RF22.h>

//inicjalizacja
RF22 rf22;

//tablice kolejności i zwłok czasowych
//XXYYZ XX - adres sterownika YY - adres wyjscia Z - komenda odpal
int tablica[48] = {10131,11111,12111,13111,10111,10161,10171,10181,10221,10201,10211,11221,10231,10241,10251,10261,13151};
int interwa[48] = {600,0,0,900,500,100,100,100,100,100,300,100,500,100,100,10};

int tablica_reczna[48] = {10131,11111,12111,13111,10111,10161,10171,10181,10221,10201,10211,11221,10231,10241,10251,10261,10131};

//zmienne
int przycisk = 0;
int wejscie_pomiarowe_przycisku = A0;    //pomiar na dzielniku napiecia
int TX;
int RX;
int flaga  = 0;
int licznik_reczny = 0;

//funkcja odtwarzania pokazu
void play_show(){
  if(flaga == 0){
    Serial.println("Start Pokazu");
    for(int licznik = 0; licznik < 17; licznik++){
       Serial.print(tablica[licznik]);
       Serial.print(";");
       Serial.println(interwa[licznik]); 
       
       //wywolanie procedury odpalenia odpowiedniego wyjscia
       send_data(tablica[licznik]);
       delay(interwa[licznik]);
    } 
   flaga = 1; 
   Serial.println("Koniec Pokazu");
  } 
}

//funkcja odtwarzania pokazu ręcznego
void execute_step(){
  Serial.print("Reczna Sekwencja "); 
 
  //wywolanie procedury odpalenia odpowiedniego wyjscia
  Serial.println(tablica_reczna[licznik_reczny]);
  send_data(tablica_reczna[licznik_reczny]);
  delay(100);

  licznik_reczny++;
}

//wybor przycisku
void read_przycisk(){
  int przycisk = 0;
  int pomiar = analogRead(wejscie_pomiarowe_przycisku);
  //Serial.println(pomiar); //odkomentuj do kalibracji
  if(pomiar >= 900 && pomiar <= 970){ 
      play_show(); // przycisk = 1 / zamiast send data odpalam automatyczna sekwencje;
      delay(100);
  }
  if(pomiar >= 800 && pomiar <= 900){
    execute_step(); //przycisk = 2 / recznie odpalam kolejne elementy sekwencji;
    delay(100); 
  }
  if(pomiar >= 740 && pomiar <= 800){
    Serial.print("przycisk 3 ");
    send_data(10131); //przycisk = 3;
    delay(100);
  }
  if(pomiar >= 670 && pomiar <= 740){
    Serial.print("przycisk 4 ");
    send_data(10141); //przycisk = 4; 
    delay(100);
  }
  if(pomiar >= 620 && pomiar <= 670){
    Serial.print("przycisk 5 ");
    send_data(10151); //przycisk = 5; 
    delay(100);
  }
  if(pomiar >= 580 && pomiar <= 620){
    Serial.print("przycisk 6 ");
    send_data(10161); //przycisk = 6; 
    delay(100);
  }
 }
  
//funkcje
void send_data(int TX){
  int RX;
  //TX
  rf22.send((uint8_t*)&TX, sizeof(TX));
  rf22.waitPacketSent();
  //RX
  uint8_t buf[RF22_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf22.waitAvailableTimeout(500)){ 
    if (rf22.recv(buf, &len)){
      RX = (int&)buf;
      if(RX == TX){
        Serial.println("ACK");
      }else{
        Serial.println("REJ");
      }
      Serial.println(RX);      
    }
    else{
        Serial.println("blad obierania");
    }
   }
   else{
     Serial.println("brak sygnalow");
   }
}

//startup sequence
void setup(){
  Serial.begin(9600);
  if (!rf22.init()){
    Serial.println("RF22 init failed");
  }
  rf22.setFrequency(434.50);
  rf22.setTxPower(RF22_TXPOW_17DBM);  
  Serial.print("startup");
}

//pę0ltka glowna
void loop(){
  read_przycisk();
}
