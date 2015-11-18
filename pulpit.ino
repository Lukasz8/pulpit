//wysyłka danych do remote

#include <RF22.h>
#include <RF22Datagram.h>
#include <RF22Mesh.h>
#include <RF22ReliableDatagram.h>
#include <RF22Router.h>

RF22 rf22
//zmienne
Int TX = 73

//funkcje
void setup(){
rf22.setFrequency(434.0);
rf22.setTxPower(RF22_TXPOW_17DBM);   
}

void send_data(){
TX
} 

void loop(){
  
//tutaj wysylka danych przez radio
  send_data()
//orginalna komenda z programu pilot
  rf22.send((uint8_t*)&a, sizeof(a));
  delay(1000); 
}
