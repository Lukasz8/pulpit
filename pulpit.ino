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
#include <RF22Datagram.h>
#include <RF22Mesh.h>
#include <RF22ReliableDatagram.h>
#include <RF22Router.h>

//inicjalizacja
RF22 rf22;

//zmienne
int TX = 73;

//funkcje
<<<<<<< HEAD
<<<<<<< HEAD
void setup(){
  rf22.setFrequency(434.5);
  rf22.setTxPower(RF22_TXPOW_17DBM);   
}

=======
>>>>>>> origin/master
=======
>>>>>>> origin/master
void send_data(){
  //TX
  rf22.send((uint8_t*)&TX, sizeof(TX));
}

//startup sequence
void setup(){
  rf22.setFrequency(434.0);
  rf22.setTxPower(RF22_TXPOW_17DBM);  
  rf22.init(); 
}

infinity loop
void loop(){
  //tutaj wysylka danych przez radio
  send_data();
  //orginalna komenda z programu pilot
  delay(1000); 
}
