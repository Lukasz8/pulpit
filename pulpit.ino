/*
SP8XCE & SQ9MDD
PULPIT 

DATA FRAME FORMAT
  // format ramki: XXYYZ
  // XX - adres docelowy odbiornika zakres 10-31
  // YY - adres wyjścia na odbiorniku zakres 10-99 10-wyjscie wirtualne czy żyjesz 11-pierwsze wyjście fizyczne
  // Z  - komenda 1 - ON 0 - OFF

CHANGELOG
2016.02.13 - dopisanie drugiej recznej sekwencji oraz dodano sekwencje testu lacznosci
2015.12.22 - scalenie kodu 
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

//tablica odpalania automatycznego
int tablica[48] = {12261,12111,13111,13261,13201,12121,13121,12261,13261,12201,13201,12111,10131,10111,12201,10121,13261};
int interwa[48] = {100,200,500,200,100,50,100,80,50,90,100,50,0,0,0,0};

//XXYYZ XX - adres sterownika YY - adres wyjscia Z - komenda odpal
//tablica odpalania recznego przycisk nr 2
int tablica_reczna_1[48] = {12111,12121,12131,12141,12151,12161,12171,12181,12191,12201,12211,12221,12231,12241,12251,12261};

//tablica odpalania recznego przycisk nr 3
int tablica_reczna_2[48] = {13111,13121,13131,13141,13151,13161,13171,13181,13191,13201,13211,13221,13231,13241,13251,13261};

//tablica sprawdzania lacznosci przycisk nr 6
int tablica_test[48] = {10100,11100,12100,13100,14100,15100,16100,17100,18100,19100,20100,21100,22100,23100,24100,25100,26100};

//zmienne
int przycisk = 0;
int wejscie_pomiarowe_przycisku = A0;    //pomiar na dzielniku napiecia
int TX;
int RX;
int flaga  = 0;
int licznik_reczny_1 = 0;
int licznik_reczny_2 = 0;

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

//funkcja odtwarzania pokazu ręcznego 1
void execute_step_1(){
  Serial.print("Reczna Sekwencja 1 ");  
  //wywolanie procedury odpalenia odpowiedniego wyjscia
  Serial.println(tablica_reczna_1[licznik_reczny_1]);
  send_data(tablica_reczna_1[licznik_reczny_1]);
  delay(100);
  licznik_reczny_1++;
}

//funkcja odtwarzania pokazu ręcznego 2
void execute_step_2(){
  Serial.print("Reczna Sekwencja 2 ");  
  //wywolanie procedury odpalenia odpowiedniego wyjscia
  Serial.println(tablica_reczna_2[licznik_reczny_2]);
  send_data(tablica_reczna_2[licznik_reczny_2]);
  delay(100);
  licznik_reczny_2++;
}

//funkcja testowania lacznosci
void play_test(){
    Serial.println("Start testu lacznosci");
    for(int licznik = 0; licznik < 17; licznik++){
       Serial.print(tablica_test[licznik]);
       Serial.print(";");
       //wywolanie procedury sprawdzenia lacznosci bez odpalania wyjsc
       send_data(tablica_test[licznik]);
       delay(100);
  } 
}

//wybor przycisku
void read_przycisk(){
  int przycisk = 0;
  int pomiar = analogRead(wejscie_pomiarowe_przycisku);
  //Serial.println(pomiar); //odkomentuj do kalibracji
  if(pomiar >= 900 && pomiar <= 970){ 
      play_show(); // przycisk = 1 /  odpalam automatyczna sekwencje;
      delay(100);
  }
  if(pomiar >= 800 && pomiar <= 900){
    execute_step_1(); //przycisk = 2 / recznie odpalam kolejne elementy sekwencji;
    delay(100); 
  }
  if(pomiar >= 740 && pomiar <= 800){
    execute_step_2(); //przycisk = 3 / recznie odpalam kolejne elementy sekwencji;
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
    play_test(); //przycisk = 6 / sprawdzam lacznosc z wyrzutniami sekwencja test;
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
        Serial.print("ACK ");
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

//pętel glowna
void loop(){
  read_przycisk();
}
