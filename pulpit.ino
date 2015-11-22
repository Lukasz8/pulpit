/*
 * 
 * 
 * CHANGELOG
 * v.1.0.1 poprawiłem kod nie zapominaj o ";" reformat kodu
 * przetestuj czy działa.
 */

//wysyłka danych do remote
#include <SPI.h>
#include <RF22.h>
//#include <RF22Datagram.h>
//#include <RF22Mesh.h>
//#include <RF22ReliableDatagram.h>
//#include <RF22Router.h>

//inicjalizacja
RF22 rf22;

//zmienne
int TX = 73;

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
   Serial.print("mamy odpowiedz:");
   delay(1000); 
   Serial.println((char*)buf);
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
    Serial.println("RF22 init failed"); //sprawdzamy czy inicjowanie radia zadziałało
  }
  rf22.setFrequency(434.5);
  rf22.setTxPower(RF22_TXPOW_17DBM);  
  
  Serial.print("startup");
}

//infinity loop
void loop(){
  //tutaj wysylka danych przez radio
  send_data();
  Serial.print("infinity");
  //delay(1000); 
}
