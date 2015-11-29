/*
SP8XCE & SQ9MDD
PULPIT 

CHANGELOG
2015.11.29 - dolozenie obslugi przyciskow i funkcji ich kalibracji
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
int TX = 10131;
int RX;

//wybor przycisku
int set_przycisk(){
  int przycisk = 0;
  int pomiar = analogRead(wejscie_pomiarowe_przycisku);
  //Serial.println(pomiar); //odkomentuj do kalibracji
  if(pomiar <= 0) przycisk = 0;
  if(pomiar >= 900 && pomiar <= 970) przycisk = 1; 
  if(pomiar >= 800 && pomiar <= 900) przycisk = 2; 
  if(pomiar >= 740 && pomiar <= 800) przycisk = 3;
  if(pomiar >= 670 && pomiar <= 740) przycisk = 4; 
  if(pomiar >= 620 && pomiar <= 670) przycisk = 5; 
  if(pomiar >= 580 && pomiar <= 620) przycisk = 6; 
  //if(pomiar >= 270 && pomiar <= 300) przycisk = 7; 
  //if(pomiar >= 300 && pomiar <= 330) przycisk = 8; 
  return(przycisk+10); 
  }
  
//funkcje
void send_data(){
  //TX
  rf22.send((uint8_t*)&TX, sizeof(TX));
  rf22.waitPacketSent();
  //RX
  uint8_t buf[RF22_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);
  if (rf22.waitAvailableTimeout(500))
  { 
  if (rf22.recv(buf, &len))
      {
      RX = (int&)buf;
   Serial.print("mamy odpowiedz: ");
   Serial.print(RX);
      }
      else
      {
        Serial.println("blad obierania");
      }
   }
    else
    {
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

//infinity loop
void loop(){
  //tutaj wysylka danych przez radio
  send_data();
  //aby skalibrować drabinke z adresem odkomentuj i czytaj wartości
  //przycisk = set_przycisk();
  //delay(1000);
  //tylko do kalibracji
  
  delay(100); 
}
