/*
SP8XCE & SQ9MDD
PULPIT 

DATA FRAME FORMAT
  // format ramki: XXYYZ
  // XX - adres docelowy odbiornika zakres 10-31
  // YY - adres wyjścia na odbiorniku zakres 10-99 10-wyjscie wirtualne czy żyjesz 11-pierwsze wyjście fizyczne
  // Z  - komenda 1 - ON 0 - OFF

CHANGELOG

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

//zmienne
int przycisk = 0;
int wejscie_pomiarowe_przycisku = A0;    //pomiar na dzielniku napiecia
int TX;
int RX;

//wybor przycisku
void read_przycisk(){
  int przycisk = 0;
  int pomiar = analogRead(wejscie_pomiarowe_przycisku);
  //Serial.println(pomiar); //odkomentuj do kalibracji
  if(pomiar >= 900 && pomiar <= 970){ 
      send_data(10111); //przycisk = 1; 
      delay(100);
  }
  if(pomiar >= 800 && pomiar <= 900){
    send_data(10121); //przycisk = 2;
    delay(100); 
  }
  if(pomiar >= 740 && pomiar <= 800){
    send_data(10131); //przycisk = 3;
    delay(100);
  }
  if(pomiar >= 670 && pomiar <= 740){
    send_data(10141); //przycisk = 4; 
    delay(100);
  }
  if(pomiar >= 620 && pomiar <= 670){
    send_data(10151); //przycisk = 5; 
    delay(100);
  }
  if(pomiar >= 580 && pomiar <= 620){
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
