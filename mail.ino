// include the library code:
#include <LiquidCrystal.h>
#include "CurieIMU.h"


const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String Version = "V0.0.1 Pre-Alpha";
// These constants won't change. They're used to give names to the pins used:
const int analogBtn = A5;
const int analogInPin2 = A0;// Analog input pin that the potentiometer is attached to
const int ledPin = 13; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int btnOutput = 0;        // value output to the PWM (analog out)
int redLine = 7125;

///////TESTING ONLY
int PID_RPM = 0; 
int PID_COOLANT_TEMP = 89;

int ledState = LOW;  

unsigned long previousMillis = 0; 
const long interval = 75;    

int screenSelect = 1;
void screen1();
void screen2();

int maxScreens = 2;

void setup() {

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, ledState);
  
  Serial.begin(9600);
  
  lcd.begin(20, 4);
  lcd.print("NurburgRing Clio172");
  lcd.setCursor(0, 1);
  lcd.print(Version);
  lcd.setCursor(6, 3);
  lcd.print("Loading");
  delay(1450);
  lcd.clear();
}

void loop() {

  sensorValue = analogRead(analogBtn);
  btnOutput = map(sensorValue, 0, 1023, 0, 255);
 
  if(sensorValue < 150) //back
  {
    lcd.clear();
        screenSelect = screenSelect + 1;
        if(screenSelect > maxScreens)
        {
          screenSelect = maxScreens;  
        }
  }
  if(sensorValue > 150 && sensorValue < 300 ) //down
  {
    lcd.clear();
        screenSelect = screenSelect - 1;
        if(screenSelect < 1)
        {
          screenSelect = 1;  
        }
  }

    switch (screenSelect) {
    case 1:    
      screen1();
      break;
    case 2:    
      screen2();
      break;
   } 
}

void screen1()
{
   unsigned long currentMillis = millis();


  lcd.setCursor(0, 0);
  int PID_RPM = map(analogRead(analogInPin2), 0, 1023, 0, 7800);
  int PID_COOLANT_TEMP = map(analogRead(analogInPin2), 0, 1023, 0, 130);


  if(PID_RPM < redLine && ledState == HIGH)
  { 
    digitalWrite(ledPin, LOW);
  }
  if(PID_RPM >= redLine)
  { 
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;
      if (ledState == LOW)
      {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }
      digitalWrite(ledPin, ledState);
    }
  }
 if(PID_RPM < 100) lcd.print("Car: OFF"); 
 if(PID_RPM < 1000 && PID_RPM > 100) lcd.print("0");
 if(PID_RPM > 100){
   lcd.print(PID_RPM);
   lcd.print(" RPM");
 }
 if(PID_COOLANT_TEMP < 100)
 {
   lcd.setCursor(15, 0); lcd.print(" ");
   lcd.print(PID_COOLANT_TEMP); lcd.print(" C");
 }
 if(PID_COOLANT_TEMP > 99)
 {
   lcd.setCursor(15, 0);
   lcd.print(PID_COOLANT_TEMP); lcd.print(" C");
 }

 lcd.setCursor(0, 3);

 lcd.setCursor(0, 3);
 lcd.print("SCREEN 1");
}


void screen2()
{

 lcd.setCursor(0, 3);
 lcd.print("SCREEN 2");
}




void Menu(){
  
  sensorValue = analogRead(analogBtn);
  btnOutput = map(sensorValue, 0, 1023, 0, 255);
  
  if(sensorValue < 150) //back
  {
        Serial.print("sensor = ");
        Serial.print(sensorValue);
        Serial.print("\t output = ");
        Serial.print(btnOutput);
        Serial.print("\t btn = ");
        Serial.println("back");
        lcd.print("back");
  }
  if(sensorValue > 150 && sensorValue < 300 ) //down
  {
        Serial.print("sensor = ");
        Serial.print(sensorValue);
        Serial.print("\t output = ");
        Serial.print(btnOutput);
        Serial.print("\t btn = ");
        Serial.println("down");
        lcd.print("down");
  }
    if(sensorValue > 300 && sensorValue < 500 ) //menu
  {
        Serial.print("sensor = ");
        Serial.print(sensorValue);
        Serial.print("\t output = ");
        Serial.print(btnOutput);
        Serial.print("\t btn = ");
        Serial.println("menu");
        lcd.print("menu");
  }
    if(sensorValue > 500 && sensorValue < 800 ) //enter
  {
        Serial.print("sensor = ");
        Serial.print(sensorValue);
        Serial.print("\t output = ");
        Serial.print(btnOutput);
        Serial.print("\t btn = ");
        Serial.println("enter");
        lcd.print("enter");
  }
}
