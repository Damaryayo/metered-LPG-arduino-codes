// Rotary Encoder Inputs
#define CLK 4
#define DT 5
#define SW 2
#include <LiquidCrystal.h>

const int rs = 7, en = 13, d4 = 11, d5 = 10, d6 = 9, d7 = 8;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

int benchmark =200;

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir ="";
unsigned long lastButtonPress = 0;
int i=0; //temperature sensor
int IRsensorPin=1;
const int buzzer = 3; //buzzer to arduino pin 1

int red_light_pin= 49;
int green_light_pin = 51;
int blue_light_pin = 53;

//int IRsensorPin = A1; // IR tracking sensor in place of gas sensor
int sensorValue = 0;

void setup() {
  
  // Set encoder pins as inputs
  pinMode(CLK,INPUT);
  pinMode(DT,INPUT);
  pinMode(SW, INPUT_PULLUP);

  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

  pinMode(A0,INPUT); // enabling analog temp sensor
  pinMode(IRsensorPin,INPUT); // enabling analog tracking sensor
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 1 as an output

  // Setup Serial Monitor
  Serial.begin(9600);
  lcd.begin(16, 1);
  lcd.print("PayAsYouCook");
  delay(500);
  lcd.clear();

  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);

}

void loop() {

  
   
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK  && currentStateCLK == 1){

    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      counter --;
      currentDir ="CCW";
    } else {
      // Encoder is rotating CW so increment
      counter ++;
      currentDir ="CW";
    }

    
    int i=analogRead(A0)*0.028;//reading temperature and calibrate in Celcius
    int consumption= counter*20;
    int sensorValue = analogRead (IRsensorPin);
    Serial.println(sensorValue,DEC);

        if (sensorValue < 100){
          lcd.print("LPG leakage !!!!");
          Serial.println(sensorValue);
          digitalWrite(buzzer, HIGH); // Send 1KHz sound signal..
          digitalWrite(green_light_pin, LOW);
          digitalWrite(red_light_pin, HIGH);
          delay(1000);
          digitalWrite(buzzer, LOW);
          digitalWrite(red_light_pin, LOW);
          
            
          }

        else{ 
               
          
    if(consumption <=benchmark){
      Serial.println("LPG milligram consumed");
      Serial.println(consumption);

        if (i<=30){
          lcd.print("Temp.: ");
          lcd.print(i,DEC);
          digitalWrite(green_light_pin, HIGH);        
         }
        else{
         lcd.print("High Temperature ");
         digitalWrite(red_light_pin, HIGH);
         //tone(buzzer, 200); // Send 1KHz sound signal..
          
          }      
      delay(1000);
      lcd.clear();
      lcd.print("mg consumed: ");
      lcd.print(consumption);
      delay(1000);
      lcd.clear();
      }
    else {
      Serial.println("Buy LPG");
      //Serial.println(consumption);
      //lcd.print("Temp.: ");
      //lcd.print(i,DEC);

      if (i<=30){
          lcd.print("Temp.: ");
          lcd.print(i,DEC); 
          digitalWrite(green_light_pin, HIGH);        
         }
        else{
         lcd.print("High Temperature ");
         digitalWrite(green_light_pin, LOW);
         digitalWrite(red_light_pin, HIGH);
         //tone(buzzer, 200); // Send 1KHz sound signal..
          
          }      
      delay(1000);
      lcd.clear();
      lcd.print("Buy LPG ");
      digitalWrite(red_light_pin, HIGH); 
      delay(1000);
      lcd.clear(); 
       
      } 
    

    if (i>30){    
      tone(buzzer, 200); // Send 1KHz sound signal...
      lcd.print("High temperature!");
      delay(1000);        // ...for 1 sec
      lcd.clear();  
    }
    
  }
}

  // Remember last CLK state
  lastStateCLK = currentStateCLK;

  // Read the button state
  int btnState = digitalRead(SW);

   
    
  delay(1);
  
}
