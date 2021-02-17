#include <SoftwareSerial.h> 
#include "ACS712.h"

ACS712 sensor(ACS712_05B, A7);


int TX = 5; 
int RX = 6; 
int ST=13;
SoftwareSerial HC_06(TX, RX);
int value,change=1,index=5,temp,voltage,flag=0,STATE;
double volt,tol=0.4;

int rall=7,rs=8;
int charge=0,chargechange=0;
float currentReading;
float ucutoff=-0.05,lcutoff=-0.70;

void setup()
{
  
  HC_06.begin(57600);
  Serial.begin(57600);

  pinMode(ST,INPUT);
  pinMode(rall,OUTPUT);
  pinMode(4,OUTPUT);
  digitalWrite(rall,LOW);
  delay(1000);
   sensor.calibrate();
}
void loop()
{   
    STATE=digitalRead(ST); 
    currentReading = sensor.getCurrentDC();
    //Serial.println(currentReading);
    if(chargingornot())
    { if(chargechange==1)
      {chargechange=0;
      digitalWrite(rall,HIGH);
      digitalWrite(rs,HIGH);
      digitalWrite(4,HIGH);
      }
    }
 else
 { if(chargechange==0)
      {
        digitalWrite(rall,LOW);
        digitalWrite(rs,HIGH);
        digitalWrite(4,LOW);
        chargechange=1;
      }
       voltage=analogRead(A6);
   volt=map(voltage,0,1023,0,2500);
   volt/=100;
   temp=index;
  // Serial.print(volt);
  // Serial.println(" voltage");   

   batteryvoltagelevel();
  
 if(STATE==HIGH)
  { 
     if(temp!=index)
    {  
        HC_06.println(index);
        Serial.print(index);
        Serial.println(" change");
    }
    if(flag==0)
     {
         HC_06.println(index);
       Serial.print(index);
       Serial.println(" first");
          flag=1;
     }
  }
  else
  { if(flag==1)
    flag=0;
  }
  
}
    delay(50);
}
int chargingornot()
{
  if(currentReading>ucutoff)
  {charge=0;
   return charge;
  }
  else if(currentReading<lcutoff)
  {charge=1;
   return charge;
  }
  else
  return charge;
}
void batteryvoltagelevel()
{
  if(volt>13.6)
   {
    if((volt<13.6+tol)||index==5)
    index=5;
   }
   else if(volt>13.35)
   {
    if((volt<13.35+tol)||index==4)
    index=4;
   }
    else if(volt>13.03)
   {
    if((volt<13.03+tol)||index==3)
    index=3;
   }
    else if(volt>12.74)
   {
    if((volt<12.74+tol)||index==2)
    index=2;
   }
    else if(volt>12.49)
   {
    if((volt<12.49+tol)||index==1)
    index=1;
   }
    else if(volt>12.27)
   { 
    if((volt<12.27+tol)||index==1)
    index=0;
   }
   else
   {
    index=0;
   }
}

