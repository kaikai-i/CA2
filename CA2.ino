// LAI SAM THUNG & ZHUANG KAILI 
// 2517720         2517689
// DCEP/1A/04

#include <Wire.h>

#include "RichShieldDHT.h"
#include "RichShieldTM1637.h"
#include "RichShieldPassiveBuzzer.h"

#define PassiveBuzzerPin 3//the SIG pin of the module is connected with D3 of OPEN-SMART UNO R3
                     //There is no white waterproof sticker on the passive buzzer
PassiveBuzzer buz(PassiveBuzzerPin);

#define CLK 10//CLK of the TM1637 IC connect to D10 of OPEN-SMART UNO R3
#define DIO 11//DIO of the TM1637 IC connect to D11 of OPEN-SMART UNO R3
#define LED_RED 4
#define LED_GREEN 5
#define LED_BLUE 6
#define LED_YELLOW 7


TM1637 disp(CLK,DIO);
double humi,t;

DHT dht;

void setup() {
	disp.init();  
	dht.begin();
  Serial.begin(9600);
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if (isnan(t) || isnan(h)) 
  {
    displayError();
  } 
  else{
    displayTemperature((int8_t)t);//
    delay(1000);
    displayHumidity((int8_t)h);//
    Serial.print(t);
    Serial.print(",");
    Serial.println(h);
    delay(2000);
  }
}
/************************************************* *********************/
/* Function: Display temperature on 4-digit digital tube */
/* Parameter: -int8_t temperature, temperature range is -40 ~ 125 degrees celsius */
/* Return Value: void */

void displayTemperature(int8_t temperature)
{
  int8_t temp[4];
  if(temperature < 0)
	{
		temp[0] = INDEX_NEGATIVE_SIGN;
		temperature = abs(temperature);
	}
	else if(temperature < 100)temp[0] = INDEX_BLANK;
	else temp[0] = temperature/100;
	temperature %= 100;
	temp[1] = temperature / 10;
	temp[2] = temperature % 10;
	temp[3] = 12;	          //index of 'C' for celsius degree symbol.
	disp.display(temp);
}

void displayHumidity(int8_t humi)
{
  int8_t temp[4];
  if(humi < 100)temp[0] = INDEX_BLANK;
  else temp[0] = humi/100;
  humi %= 100;
  temp[1] = humi / 10;
  temp[2] = humi % 10;
  temp[3] = 18;	          //index of 'H' for celsius degree symbol.
  disp.display(temp);

  if (humi>40)
{
  Serial.println("LOW HUMIDITY value");
  void blink(int led,int msdelay);

  digitalWrite(LED_RED, LOW);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE,HIGH);
  digitalWrite(LED_YELLOW,LOW);
	
  for (int i = 0; i < 10; i++)
  {  buz.playTone(392, 100);
     delay(100);
  }
}
else
{ 
  Serial.println("HIGH HUMIDITY");	
  digitalWrite(LED_RED,HIGH );
  digitalWrite(LED_GREEN,LOW );
  digitalWrite(LED_BLUE,LOW );
  digitalWrite(LED_YELLOW,LOW);
  return 0;
}
}

void displayError()
{
  disp.display(3,14);//display "E"
}

void setUp()
{
  pinMode(LED_RED,OUTPUT);
  pinMode(LED_GREEN,OUTPUT);
  pinMode(LED_BLUE,OUTPUT);
  pinMode(LED_YELLOW,OUTPUT);
}
