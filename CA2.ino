// LAI SAM THUNG & ZHUANG KAILI 
// 2517720         2517689
// DCEP/1A/04

#include <Wire.h>
#include "RichShieldDHT.h"
#include "RichShieldTM1637.h"
#include "RichShieldPassiveBuzzer.h"

#define PassiveBuzzerPin 3
#define CLK 10   // CLK of the TM1637 IC connect to D10
#define DIO 11   // DIO of the TM1637 IC connect to D11
#define LED_RED 4
#define LED_GREEN 5
#define LED_BLUE 6
#define LED_YELLOW 7
#define BUTTONK1 8
#define KNOB_PIN A0

// Music notes
#define NOTE_B4  494
#define NOTE_A4  440
#define NOTE_G4  392
#define NOTE_E4  330
#define NOTE_D4  294  

PassiveBuzzer buz(PassiveBuzzerPin);
TM1637 disp(CLK, DIO);
DHT dht;

// Global variables
double humi;
double temperature;
int knobValue;
int tolerance = 30;
int targetValue = 400;

// Function prototypes
void displayTemperature(int8_t temperature);
void displayHumidity(int8_t humi);
void knobLedBuzzer();
void displayError();
void playMusic();
void ledLightsUp_RED();
void ledLightsUp_BLUE();

void setup()
{
  disp.init();  
  dht.begin();
  Serial.begin(9600);

  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(BUTTONK1, INPUT_PULLUP);
}

void loop() 
{
  int knobValue = analogRead(KNOB_PIN);

  // Case 1: knob value 0–314 shows and detect Temperature  (LAI SAM THUNG)
  if (knobValue <= 314)
  {
    disp.display(1);   // shows case 1

    if (digitalRead(BUTTONK1) == 0) {
      delay(200);
      disp.clearDisplay();  // clear mode 1 diplay shown
      digitalWrite(LED_RED, HIGH);
      digitalWrite(LED_GREEN, HIGH);
      digitalWrite(LED_YELLOW, LOW);
      digitalWrite(LED_BLUE, LOW);

      float t = dht.readTemperature();

      if (isnan(t))
      {
        displayError();
      } else
      {
        displayTemperature((int8_t)t);
        ledLightsUp_RED();
        playMusic();
      }
    }
  }

  // Case 2: knob value 341–682 shows and detect Humidity    (ZHUANG KAILI)
  else
  if (knobValue >= 341 && knobValue <= 682)
  {
    disp.display(2);   // shows mode 2

    if (digitalRead(BUTTONK1) == 0) //PRESS BUTTON
    {  
      delay(200);
      disp.clearDisplay();
      digitalWrite(LED_BLUE, HIGH);
      digitalWrite(LED_YELLOW, HIGH);
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GREEN, LOW);

      float h = dht.readHumidity();

      if (isnan(h))
      {
        displayError();
      } else
      {
        displayHumidity((int8_t)h);
        ledLightsUp_BLUE();
        playMusic();
      }
    }
  }

  // Case 3: knob value > 682 → LED
  else
  {
    disp.display(3); // SHOWS MODE 3
    if (digitalRead(BUTTONK1) == 0)
    {
      delay(200);
      disp.clearDisplay();

      digitalWrite(LED_RED, HIGH);  
      delay(200);
      digitalWrite(LED_RED, LOW);    
      delay(200);
      digitalWrite(LED_GREEN, HIGH);
      delay(200);
      digitalWrite(LED_GREEN, LOW);  
      delay(200);
      digitalWrite(LED_BLUE, HIGH);  
      delay(200);
      digitalWrite(LED_BLUE, LOW);  
      delay(200);
      digitalWrite(LED_YELLOW, HIGH);
      delay(200);
      digitalWrite(LED_YELLOW, LOW);
      while(1){
        knobLedBuzzer();
      }
    }
  }
}

// -------- Functions --------

void displayError()
{
  disp.display(3, 14); // display "E"
}

void playMusic() 
{  // CHATGPT:MARY HAD A LITTLE LAMB  
  int melody[] = {  // SECOND ARRAY
    NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_B4, NOTE_B4,
    NOTE_A4, NOTE_A4, NOTE_A4,
    NOTE_B4, NOTE_D4, NOTE_D4,
    NOTE_B4, NOTE_A4, NOTE_G4, NOTE_A4, NOTE_B4, NOTE_B4, NOTE_B4,
    NOTE_B4, NOTE_A4, NOTE_A4, NOTE_B4, NOTE_A4, NOTE_G4
  };

  int noteDurations[] = {
    4, 4, 4, 4, 4, 4, 2,
    4, 4, 2,
    4, 4, 2,
    4, 4, 4, 4, 4, 4, 2,
    4, 4, 4, 4, 4, 2
  };

  int lengthOfMusic = sizeof(melody) / sizeof(melody[0]);

  for (int i = 0; i < lengthOfMusic; i++)
  {
    int duration = noteDurations[i] * 200;
    buz.playTone(melody[i], duration);
    delay(50); // slight gap
  }
}

void displayTemperature(int8_t temperature)
{
  int8_t temp[4];
  if (temperature < 0)
  {
    temp[0] = INDEX_NEGATIVE_SIGN;
    temperature = abs(temperature);
  } else {
    if (temperature < 100)
      temp[0] = INDEX_BLANK;
    else
      temp[0] = temperature / 100;
  }
  temperature %= 100;
  temp[1] = temperature / 10;
  temp[2] = temperature % 10;
  temp[3] = 12; // symbol
  disp.display(temp);
}

void displayHumidity(int8_t humi)
{
  int8_t humidity[4];
  if (humi < 100) humidity[0] = INDEX_BLANK;
  else humidity[0] = humi / 100;
  humi %= 100;
  humidity[1] = humi / 10;
  humidity[2] = humi % 10;
  humidity[3] = 18; // index of 'H'
  disp.display(humidity);
}

void ledLightsUp_RED()
{
  for (int i = 0; i < 26; i++)
  {
    digitalWrite(LED_RED, HIGH);
    delay(200);
    digitalWrite(LED_RED, LOW);
    delay(200);
  }
}

void ledLightsUp_BLUE()
{
  for (int i = 0; i < 26; i++)
  {
    digitalWrite(LED_BLUE, HIGH);
    delay(200);
    digitalWrite(LED_BLUE, LOW);
    delay(200);
  }
}

void knobLedBuzzer ()
{
  humi = dht.readHumidity();
  displayHumidity(humi);

  if (humi > 65)
  {
    Serial.println("LOW HUMIDITY");
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_YELLOW, LOW);
    Serial.println("Password hint : What is my gpa?");
 
    for (int i = 0; i < 15; i++)
    {  
      buz.playTone(392, 100);
      delay(100);
    }
  } else
  {
    Serial.println("NORMAL HUMIDITY");  
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(LED_BLUE, LOW);
    digitalWrite(LED_YELLOW, LOW);
    return 0;
  }
  knobValue = analogRead(KNOB_PIN);
  Serial.print("HUMDITIY VALUE : ");
  Serial.println(humi);
  delay(500);

  int ledPinNum[] = {LED_RED, LED_GREEN, LED_BLUE, LED_YELLOW};  // FIRST ADDITION ARRAY
Serial.print("Knob value : ");
Serial.println(knobValue);
   if (abs(knobValue - targetValue) <= tolerance)
    {
      Serial.println("Knob in position! Press button to continue...");
    }

  if (digitalRead(BUTTONK1) == 0)
  {
     delay(30);                 // add a small delay to debounce the button
     disp.display(knobValue);
     Serial.println("Button K1 is pressed");//send the string to Serial monitor
     if (abs(knobValue - targetValue) <= tolerance)
    {
      Serial.print("Knob value");
      Serial.println(knobValue);
      digitalWrite(ledPinNum[0],LOW );
      digitalWrite(ledPinNum[1],LOW );
      digitalWrite(ledPinNum[2],HIGH );
      digitalWrite(ledPinNum[3],LOW);
      delay(500);

      playMusic();

    }
     else
    {
      buz.playTone(392, 100);
      delay(100);
      Serial.print("Knob value ");
      Serial.print("WRONG");
      Serial.println(knobValue);
      delay(500);
      digitalWrite(ledPinNum[0],LOW );
      digitalWrite(ledPinNum[1],LOW );
      digitalWrite(ledPinNum[2],LOW);
      digitalWrite(ledPinNum[3],HIGH);
      delay(500);
  }
     
     while (digitalRead(BUTTONK1) == 0);
 }
}
