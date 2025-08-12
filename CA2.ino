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
#define BUTTONK1 8 
#define KNOB_PIN A0
#define BUTTONK1 8  //BUTTON K1 is connected to digital Pin 8 


TM1637 disp(CLK,DIO);
double humi;
int knobValue=400;
int Aread=400;

DHT dht;

void setup() {
	disp.init();  
	dht.begin();
  Serial.begin(9600);
  pinMode(LED_RED,OUTPUT);
  pinMode(LED_GREEN,OUTPUT);
  pinMode(LED_BLUE,OUTPUT);
  pinMode(LED_YELLOW,OUTPUT);
  pinMode(BUTTONK1,INPUT_PULLUP); 
}

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();

  // check if returns are valid, if they are NaN (not a number) then something went wrong!
  if ( isnan(h)) 
  {
    displayError();
  } 
  else{
    displayHumidity((int8_t)h);//
    Serial.println(h);
    delay(1000);
  }
}
/************************************************* *********************/
/* Function: Display temperature on 4-digit digital tube */
/* Parameter: -int8_t temperature, temperature range is -40 ~ 125 degrees celsius */
/* Return Value: void */


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

  if (humi>70)
{
  Serial.println("LOW HUMIDITY");
  void blink(int led,int msdelay);

  digitalWrite(LED_RED, HIGH);
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_YELLOW,LOW);
  Serial.println("Password hint : What is my gpa?");
	
  for (int i = 0; i < 15; i++)
  {  buz.playTone(392, 100);
     delay(100);
  }
}
else
{ 
  Serial.println("NORMAL HUMIDITY");	
  digitalWrite(LED_RED,LOW );
  digitalWrite(LED_GREEN,HIGH );
  digitalWrite(LED_BLUE,LOW );
  digitalWrite(LED_YELLOW,LOW);
  return 0;
}
if (knobValue=Aread)
{
  knobValue=analogRead(KNOB_PIN);
  Serial.print("Knob value ");
  Serial.print("CORRECT ");
  Serial.println(knobValue);
  delay(500);
  digitalWrite(LED_RED,LOW );
  digitalWrite(LED_GREEN,LOW );
  digitalWrite(LED_BLUE,HIGH );
  digitalWrite(LED_YELLOW,LOW);

  for (int knobValue=4000 ;; )
  {  buz.playTone(392, 100);
     delay(100);
  }

  return 0;
}
else
{
  buz.playTone(392, 100);
  delay(100);
  Serial.print("Knob value ");
  Serial.print("WRONG");
  Serial.println(knobValue);
  delay(500);
  digitalWrite(LED_RED,LOW );
  digitalWrite(LED_GREEN,LOW );
  digitalWrite(LED_BLUE,LOW);
  digitalWrite(LED_YELLOW,HIGH);
  
  return 0;

}
}

void displayError()
{
  disp.display(3,14);//display "E"
}

 
void setup() { 
  
pinMode(BUTTONK1,INPUT_PULLUP); // sets pin8 as input with INPUT_PULLUP mode. 
 
 Serial.begin(9600);   // open serial port, set data rate to 9600 bps        
} 
 
void loop() { 
 
  if (digitalRead(BUTTONK1) == 0) // check if button K1 is pressed (logic 0 when pressed)  
  { 
     delay(300);                 // add a small delay to debounce the button 
         
     Serial.println("Button K1 is pressed");//send the string to Serial monitor 
 
     while (digitalRead(BUTTONK1) == 0);/*Ensure the button is released (i.e. back to 
logic 1) before executing the next statement */ 
  }     
}

