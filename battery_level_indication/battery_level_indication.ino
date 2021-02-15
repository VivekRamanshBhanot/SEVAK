#include <SoftwareSerial.h> 

int TX = 5; 
int RX = 6; 
int ST=13;
int value,change=1,index=5,temp,voltage,flag=0,STATE;
double volt,tol=0.2;

char ch;
SoftwareSerial HC_06(TX, RX);  //Bluetooth TX to 10 and Bluetooth RX to 11.

void setup()
{
  pinMode(ST,INPUT);
  Serial.begin(57600);
  HC_06.begin(57600);
}

void loop()
{ STATE=digitalRead(ST); 
  voltage=analogRead(A6);
   volt=map(voltage,0,1023,0,2500);
   volt/=100;
   temp=index;
   Serial.print(volt);
   Serial.println(" voltage");   
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

 if(STATE==HIGH)
  { 
     if(temp!=index)
    {  
        HC_06.print(index);
    }
    if(flag==0)
     {
         HC_06.print(index);
          flag=1;
     }
  }
  else
  { 
    flag=0;
  }
  
  if(HC_06.available()> 0 ) 
  {
      value = HC_06.parseInt();
      ch=HC_06.read();
      Serial.print(value); 
      Serial.print(" value "); 
      Serial.print(ch); 
      Serial.print("\n");

  } 
  delay(100);
}
