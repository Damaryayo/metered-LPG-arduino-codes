
/*This is the original code to send LPG amount and temperature to thingspeak by nodeMCU
 * created by Jean Damascene
 * on 2 May 2021
 */ 
#include <LiquidCrystal.h>
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include "ThingSpeak.h"

int x;
const int rs = D0, en =D3, d4 = D5, d5 = D6, d6 = D7, d7 = D8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const char* ssid ="safeguard";
const char* password ="damascene1";
WiFiClient  client;
unsigned long myChannelNumber = 1380936;
const char * myWriteAPIKey = "XBGNHI3DNFK3VL0K";

unsigned long myChannelNumbert = 1373348;
const char * myWriteAPIKeyt = "P5XKKKRG5WDL7MLB";

// nothing special about these pins, just make sure they don't collide with
// other active features
#define ClockPin 4  // labeled either as CLK or as A
#define DataPin 5   // labeled either as DT or as B
#define ButtonPin 2 // labeled as SW

// include libraries
#include <Task.h>

// include sub files
#include "RotaryEncoderTask.h" // this implements the rotary encoder task

TaskManager taskManager;

// foreward delcare functions passed to task constructors now required
void HandleButtonChanged(EncoderButtonState state);
void HandleRotationChanged(int8_t rotationDelta);


RotaryEncoderTask RotaryTask(HandleRotationChanged,
    HandleButtonChanged,
    ClockPin,
    DataPin,
    ButtonPin);

    int data;

void setup()
{
    // set up the LCD's number of columns and rows:
    lcd.begin(16, 2);
    
    WiFi.begin(ssid, password);

    ThingSpeak.begin(client);

    pinMode(A0,INPUT);
    //delay(10);
  
    Serial.begin(9600);
    while (!Serial); // wait for serial monitor to connect


    taskManager.StartTask(&RotaryTask);

    Serial.println("Running...");
   // ESP.deepSleep(15e6); 
}

void loop()
 
{
    taskManager.Loop();
    
}



void HandleButtonChanged(EncoderButtonState state)
{
     //Serial.println("Amount of LPG:");
    
    if (state == EncoderButtonState_Pressed)
    {
      
        Serial.println("Pressed - ");
        
    }
    else if (state == EncoderButtonState_Released)
    {
        Serial.println("Released - ");
    } 
    else
    {
        Serial.println("Auto-repeat - ");
    }
}

void HandleRotationChanged(int8_t rotationDelta)
{
    if (rotationDelta > 0)
    {
        Serial.print("pulses = ");
    }
    /*else
    {
        Serial.print("counter-clockwise = ");
    }*/
    x=RotaryTask.RotationValue();
    /*if (D8==0)
    {
      
      x=0;
      }
      */

    

    Serial.println(x);    
    int newPulses=RotaryTask.RotationValue();
    int newMg=newPulses*20; // converting from pulses to milligrams
    int oldMg=newMg;
    int netMg=oldMg+newMg;

    int i=analogRead(A0)*0.028;//reading temperature in Celcius

    lcd.print(netMg);
    //lcd.display();

    ThingSpeak.writeField(myChannelNumber,1,netMg,myWriteAPIKey); 
    ThingSpeak.writeField(myChannelNumbert,1,i,myWriteAPIKeyt);

  
 
  // Turn on the display:
  lcd.display();
 //delay(500);
}
