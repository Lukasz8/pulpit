/*
SP8XCE & SQ9MDD
PULPIT 

DATA FRAME FORMAT
  // format ramki: XXYYZ
  // XX - adres docelowy odbiornika zakres 10-31
  // YY - adres wyjścia na odbiorniku zakres 10-99 10-wyjscie wirtualne czy żyjesz 11-pierwsze wyjście fizyczne
  // Z  - komenda 1 - ON 0 - OFF

CHANGELOG
2015.11.26 - format ramki danych sprawdzenie poprawności odbioru danych z remote'a
2015.11.24 - poprawki i synchronizacja kodu
*/
//wysyłka danych oraz potwierdzenie odbioru
#include <SPI.h>
#include <RF22.h>
//#include <RF22Datagram.h>
//#include <RF22Mesh.h>
//#include <RF22ReliableDatagram.h>
//#include <RF22Router.h>

//inicjalizacja
RF22 rf22;

//zmienne

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

//infinity loop
void loop(){
  //tutaj wysylka danych przez radio
  send_data(11101); //wysyłam dane patrz format ramki w komentarzu na górze
  Serial.print(" infinity ");
  //ulatwia synchronizowanie odbiornika po zaniku sygnalu
  delay(100); 
}
