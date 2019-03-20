
// include the library code:
#include <LiquidCrystal.h>
#include <Arduino.h>
#include <Wire.h>
#include <OBD2UART.h>


COBD obd;


const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String Version = "V0.0.4 Alpha";
// These constants won't change. They're used to give names to the pins used:
const int analogBtn = A5;
const int analogInPin2 = A0;// Analog input pin that the potentiometer is attached to
const int ledPin = 13; // Analog output pin that the LED is attached to

int sensorValue = 0;        // value read from the pot
int btnOutput = 0;        // value output to the PWM (analog out)
int redLine = 7125;

///////TESTING ONLY
int RPM = 0; 
int COOLANT_TEMP = 0;
int SPEED = 0; 
int ENGINE_LOAD = 0;


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
  
 
  lcd.begin(20, 4);
  lcd.print("NurburgRing Clio172");
  lcd.setCursor(0, 1);
  lcd.print(Version);
  lcd.setCursor(6, 3);
  lcd.print("Loading");
  obd.begin();
  // initiate OBD-II connection until success
  while (!obd.init());  
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
  obd.readPID(PID_RPM, RPM);
  obd.readPID(PID_COOLANT_TEMP, COOLANT_TEMP);
  obd.readPID(PID_SPEED, SPEED);
  obd.readPID(PID_ENGINE_LOAD, ENGINE_LOAD);
    
  if(RPM < redLine && ledState == HIGH)
  { 
    digitalWrite(ledPin, LOW);
  }
  if(RPM >= redLine)
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
 if(RPM < 100) lcd.print("Car: OFF"); 
 if(RPM < 1000 && RPM > 100) lcd.print("0");
 if(RPM > 100){
   lcd.print(RPM);
   lcd.print(" RPM");
 }
 if(COOLANT_TEMP < 100)
 {
   lcd.setCursor(15, 0); lcd.print(" ");
   lcd.print(COOLANT_TEMP); lcd.print(" C");
 }
 if(COOLANT_TEMP > 99)
 {
   lcd.setCursor(15, 0);
   lcd.print(COOLANT_TEMP); lcd.print(" C");
 }
 
 if(SPEED < 10)
 {
   lcd.setCursor(0, 1); lcd.print("  ");
   lcd.print(SPEED); lcd.print(" km/h");
 }
 if(SPEED > 9 && SPEED < 100)
 {
   lcd.setCursor(0, 1); lcd.print(" ");
   lcd.print(SPEED); lcd.print(" km/h");
 }
 
 if(SPEED > 99)
 {
   lcd.setCursor(0, 1);
   lcd.print(SPEED); lcd.print(" km/h");
 }

 lcd.setCursor(12, 1);
 lcd.print("Load ");
 if(ENGINE_LOAD < 10)
 {
   lcd.setCursor(17, 1); lcd.print("  ");
   lcd.print(ENGINE_LOAD);
 }
 if(ENGINE_LOAD > 9 && ENGINE_LOAD < 100)
 {
   lcd.setCursor(17, 1); lcd.print(" ");
   lcd.print(ENGINE_LOAD);
 }
 if(ENGINE_LOAD > 99)
 {
   lcd.setCursor(17, 1);
   lcd.print(ENGINE_LOAD);
 }
 lcd.setCursor(0, 2);
 lcd.print(obd.getVoltage(), 1);
 lcd.print("Volt");
 lcd.setCursor(0, 3);

 lcd.setCursor(0, 3);
 lcd.print("SCREEN 1");
}


void screen2()
{

 lcd.setCursor(0, 3);
 lcd.print("SCREEN 2");
}
