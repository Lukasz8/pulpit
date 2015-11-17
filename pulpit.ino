/* Pulpit test app using github
 * 
 * 
 * 
 */

#include <LiquidCrystal.h>
#include <SPI.h>
#include <RF22.h>

//constants for the number of rows and columns in the LCD
//In my example, I use a 20x4 LCD, thus it has 4 rows and 20 columns
//If you use another LCD, type in the appropriate rows and columns below
const int numRows= 4;
const int numCols= 20;

//initialize the library with the numbers of the interface pins
LiquidCrystal lcd(8, 7, 6, 5, 4, 3);

RF22 rf22;
void setup() {
Serial.begin(9600);
  if (!rf22.init()){}
 pinMode(A0,INPUT);
 digitalWrite(A0,HIGH);
 pinMode(A1,INPUT);
 digitalWrite(A1,HIGH);
  pinMode(A2,INPUT);
 digitalWrite(A2,HIGH);
 pinMode(A3,INPUT);
 digitalWrite(A3,HIGH);
  pinMode(A4,INPUT);
 digitalWrite(A4,HIGH);
 pinMode(A5,INPUT);
 digitalWrite(A5,HIGH);
    rf22.setFrequency(434.0);
  rf22.setTxPower(RF22_TXPOW_5DBM); 
  Serial.print("ok");
 
lcd.begin(numCols, numRows);
lcd.print("ujmulowy FOCH!!!");
}

void loop()
{
if(digitalRead(A0)==LOW){
 int a=0;
             rf22.send((uint8_t*)&a, sizeof(a));
    rf22.waitPacketSent();
}
if(digitalRead(A1)==LOW){
 int a=1;
             rf22.send((uint8_t*)&a, sizeof(a));
    rf22.waitPacketSent();
}
if(digitalRead(A2)==LOW){
  int a=2;
             rf22.send((uint8_t*)&a, sizeof(a));
    rf22.waitPacketSent();
}
if(digitalRead(A3)==LOW){
  int a=3;
             rf22.send((uint8_t*)&a, sizeof(a));
    rf22.waitPacketSent();
}
if(digitalRead(A4)==LOW){
  int a=4;
             rf22.send((uint8_t*)&a, sizeof(a));
    rf22.waitPacketSent();
}
if(digitalRead(A5)==LOW){
  int a=5;
             rf22.send((uint8_t*)&a, sizeof(a));
    rf22.waitPacketSent();
}

//set the cursor to column 0, line 1
//displays the number of seconds which has elapsed since code is uploaded
lcd.setCursor(0,1);
delay(1000);
lcd.print(millis()/1000);
lcd.print(" seconds elapsed");
}

