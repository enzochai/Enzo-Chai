#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 2,1,0,4,5,6,7,3,POSITIVE);
int flowPin1 = A1;
int flowPin2 = A2;
int waterSens = A0;
int greenled = 3;
int yellowled = 4;
int redled = 5;
int Buzzer = 6;
int trigPin = 8;
int echoPin = 9;
int X1;
int X2;
int Y1;
int Y2;
double TIME1 = 0;
double FREQUENCY1 = 0;
double WATER1 = 0;

double TIME2 = 0;
double FREQUENCY2 = 0;
double WATER2 =0;

int waterVal;


void setup() 
{
  pinMode(flowPin1, INPUT);   
  pinMode(flowPin2, INPUT);
  pinMode(waterSens, INPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(greenled, OUTPUT);
  pinMode(yellowled, OUTPUT);
  pinMode(redled, OUTPUT);
  lcd.begin(16,2);
  lcd.setCursor(3,0);
  lcd.print("SAFE FLOW");
  lcd.setCursor(2,1);
  lcd.print("TECHNOLOGIES");
  delay(3000);
  lcd.clear();
  Serial.begin(9600); 
}

void loop() {    
  X1 = pulseIn(flowPin1, HIGH);
  Y1 = pulseIn(flowPin1, LOW);
  TIME1 = X1 + Y1;
  FREQUENCY1 = 1000000/TIME1;
  WATER1 = FREQUENCY1/7.5;
  
  X2 = pulseIn(flowPin2, HIGH);
  Y2 = pulseIn(flowPin2, LOW);
  TIME2 = X2 + Y2;
  FREQUENCY2 = 1000000/TIME2;
  WATER2 = FREQUENCY2/7.5;
  
  waterVal = analogRead(waterSens);
  double duration, distance;
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(500);
  digitalWrite(trigPin, LOW);
  duration=pulseIn(echoPin, HIGH);
  distance =(duration/2)/29.1;
   
  Serial.println(String(WATER1)+"L/min for sensor1   "+String(WATER2)+"L/min for sensor2    "+"water depth: "+distance+"cm    "+"water level: "+waterVal);      

 if(isinf(FREQUENCY1)) //If no water flow is detected at the valley.
 {
      lcd.setCursor(0,0);
      lcd.print("1st water flow");
      lcd.setCursor(0,1);
      lcd.print("sensor N/A.");
      delay(2000);
      lcd.clear();
      digitalWrite(greenled, HIGH);
      digitalWrite(yellowled, HIGH);
      digitalWrite(redled, HIGH);
      digitalWrite(Buzzer,LOW);
 }
 else
 {
 if((WATER1>5)&&(distance<5)) //If water flow at valley is high and water level is high.
    {
      digitalWrite(greenled, LOW);
      digitalWrite(yellowled, LOW);
      digitalWrite(redled,HIGH);
      digitalWrite(Buzzer,HIGH);
      lcd.setCursor(0,0);
      lcd.print("Danger!");
      delay(2000);
      lcd.clear();
    }
 else 
    {
      if(isinf(FREQUENCY2)) //If no water flowis detected at the peak of the waterfall.
      {
        digitalWrite(greenled, HIGH);
        digitalWrite(yellowled, HIGH);
        digitalWrite(redled,HIGH);
        digitalWrite(Buzzer,LOW);
        lcd.setCursor(0,0);
        lcd.print("2nd water flow");
        lcd.setCursor(0,1);
        lcd.print("sensor N/A.");
        delay(2000);
        lcd.clear();
      }
     else if((WATER2<=5)&&(waterVal<=100)) //If water flow at peak is low and no rain is detected.
     {
      digitalWrite(greenled, HIGH);
      digitalWrite(yellowled, LOW);
      digitalWrite(redled,LOW);
      digitalWrite(Buzzer,LOW);
      lcd.setCursor(0,0);
      lcd.print("Safe to use.");
      delay(2000);
      lcd.clear();
     }
     else if((WATER2>5)||(waterVal>100)) //If water flow at peak is high or rain is detected.
     {
      digitalWrite(greenled, LOW);
      digitalWrite(yellowled, HIGH);
      digitalWrite(redled,LOW);
      digitalWrite(Buzzer,HIGH);
      lcd.setCursor(0,0);
      lcd.print("Incoming strong");
      lcd.setCursor(0,1);
      lcd.print("current!");
      delay(2000);
      lcd.clear();
     }
  }
 }
 }
