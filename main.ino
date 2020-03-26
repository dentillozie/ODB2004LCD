
// include the library code:
#include <LiquidCrystal.h>
#include <Arduino.h>
#include <Wire.h>
#include <OBD2UART.h>
#include <Adafruit_NeoPixel.h>

#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#define LED_PIN    2
#define LED_COUNT 10

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

COBD obd;

bool debug = false;

const int rs = 9, en = 8, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String Version = "V0.0.5 Alpha";
// These constants won't change. They're used to give names to the pins used:
const int analogBtn = A5;
const int analogInPin2 = A0;// Analog input pin that the potentiometer is attached to

int sensorValue = 0;        // value read from the pot
int btnOutput = 0;        // value output to the PWM (analog out)

//LEDSettings
int MinRPM=5200;
int MaxRPM=7200;
int FlashRPM=7050;


///////setting up values
int RPM = 0; 
int COOLANT_TEMP = 0;
int SPEED = 0; 
int ENGINE_LOAD = 0;


unsigned long previousMillis = 0; 
const long interval = 75;    

int screenSelect = 1;
void screen1();
void screen2();

int maxScreens = 2;

void setup() {


  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(60);
  
  lcd.begin(20, 4);
  lcd.print("NurburgRing Clio172");
  lcd.setCursor(0, 1);
  lcd.print(Version);
  lcd.setCursor(6, 3);
  lcd.print("Loading");
  sensorValue = analogRead(analogBtn);
  if(sensorValue > 300 && sensorValue < 500 ) //menu
  {
        debug=true;
  }
  sensorValue = 0;
  obd.begin();
  // initiate OBD-II connection until success
  if(debug==false) //menu
  {
       while (!obd.init());  
  }
  lcd.clear();
}

void loop() {

  sensorValue = analogRead(analogBtn);
  btnOutput = map(sensorValue, 0, 1023, 0, 255);

//reading values
  obd.readPID(PID_RPM, RPM);
  obd.readPID(PID_COOLANT_TEMP, COOLANT_TEMP);
  obd.readPID(PID_SPEED,SPEED);
  obd.readPID(PID_ENGINE_LOAD, ENGINE_LOAD);
  
  if(RPM>(MinRPM-100))
  {
    RPMtoLEDmap(RPM);
  } 
 
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
  if(debug==true) //menu
  {
     lcd.setCursor(0, 3);
     lcd.print("SCREEN 1");
  }

}

void RPMtoLEDmap(int RPM)
{
  int engineRPM = map(RPM, MinRPM, MaxRPM,0,12);
  if(engineRPM < 5){
    strip.setPixelColor(engineRPM, strip.Color(0, 255, 0));
    strip.show();//    Serial.println(engineRPM);
  }
  if(engineRPM >= 5 && engineRPM < 8){
    strip.setPixelColor(engineRPM, strip.Color(200, 255, 0));
    strip.show();
  }
  if(engineRPM >= 8 && engineRPM <= 10){
    strip.setPixelColor(engineRPM, strip.Color(255, 0, 0));
    strip.show();
  }
    if( engineRPM > 10)
    {
      RevLimitFlash();
    }
}

void RPMtoLED(int RPM)
{
  int engineRPM = RPM;
    if(engineRPM> 5200 && engineRPM< 5799)
    {
      strip.setPixelColor(0, strip.Color(0, 255, 0));
      strip.setPixelColor(1, strip.Color(0, 255, 0));
      strip.show();
    }
    if(engineRPM> 5800 && engineRPM< 6199)
    {
      strip.setPixelColor(2, strip.Color(0, 255, 0));
      strip.setPixelColor(3, strip.Color(0, 255, 0));
      strip.show();
    }
    if(engineRPM> 6200 && engineRPM< 6499)
    {
      strip.setPixelColor(4, strip.Color(0, 255, 0));
      strip.setPixelColor(5, strip.Color(0, 255, 0));
      strip.show();
    }
    if(engineRPM> 6500 && engineRPM< 6799)
    {
      strip.setPixelColor(6, strip.Color(255, 255, 0));
      strip.setPixelColor(7, strip.Color(255, 255, 0));
      strip.show();
    }
    if(engineRPM> 6900 && engineRPM< 7049)
    {
      strip.setPixelColor(8, strip.Color(255,0, 0));
      strip.setPixelColor(9, strip.Color(255, 0, 0));
      strip.show();
    }
    if( engineRPM> FlashRPM)
    {
      RevLimitFlash();
    }
}

void RevLimitFlash()
{
    strip.fill(strip.Color(255,0,0));
    strip.show();
    delay(70);
    strip.fill(strip.Color(0,0,0));
    strip.show();
    delay(70);
}

void screen2()
{

 lcd.setCursor(0, 3);
 lcd.print("SCREEN 2");
}
