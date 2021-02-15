#include <SoftwareSerial.h> 
#include <Servo.h> 

Servo servol,servor,pwm;
const int maxspeed=90;
int TX =5; 
int RX =6; 
int rmove1=11,rmove2=12,rall=7;
int speedplus=5,speedminus=5,speedfall=3,turngap=3,ind=0;
int value,wheelalign=-4,angle=90,targetangle=90,speedangle=0,targetspeed=0,bgstate=0;
char ch;
SoftwareSerial HC_06(TX, RX);  //Bluetooth TX to 10 and Bluetooth RX to 11.

void setup()
{
  servol.attach(A4); 
  servor.attach(A5);
  pwm.attach(A3);
  pinMode(rmove1,OUTPUT);
  pinMode(rmove2,OUTPUT);
  pinMode(rall,OUTPUT);
  digitalWrite(rmove1,HIGH);
  digitalWrite(rmove2,HIGH);
  digitalWrite(rall,LOW);;
  Serial.begin(57600);
  HC_06.begin(57600);
  servol.write(90+wheelalign);
  servor.write(90);
  pwm.write(0);
}

void loop()
{
  if(HC_06.available()> 0 ) 
  {
      value = HC_06.parseInt();
      ch=HC_06.read();
      Serial.print(value); 
      Serial.print(" value "); 
      Serial.print(ch); 
      Serial.print("\n"); 
      if(ch=='y')
      {
        if(value>=32&&value<=72)
        {
          targetspeed=map(value,32,72,0,maxspeed);
          targetspeed=maxspeed-targetspeed;
          bgstate=1;
          if(targetspeed==0)
          { bgstate=0;
          }
        }
        else if(value>72&&value<=120)
        {
          targetspeed=map(value,72,120,0,maxspeed);
          bgstate=2;
        }
      }
      if(ch=='x')
      {   if(value>=24&&value<=72)
         {
          targetangle=map(value,24,72,20,90);
         }
         else if(value>72&&value<=124)
         {
          targetangle=map(value,72,124,90,160);
         }
      }
      
  }
      
      if((ind==1 && bgstate==2)||(ind==2 && bgstate==1))
      { if(speedangle!=0)
        { halting();
          pwm.write(speedangle);
        }
        else
        { digitalWrite(rmove1,HIGH);
          digitalWrite(rmove2,HIGH);
          ind=bgstate;
        }
      }
      else
      ind=bgstate;    
      
      if(ind==0)
      {
        if(speedangle!=0)
        {
          halting();
          pwm.write(speedangle);
        }
        else
        { digitalWrite(rmove1,HIGH);
          digitalWrite(rmove2,HIGH);
        }  
      }
      else if(targetspeed>speedangle)
      {    pwm.write(speedangle);
            if(speedangle!=targetspeed)
            inc_speed();          
      }
      else if(targetspeed<speedangle)
      {
         pwm.write(speedangle);
         if(speedangle!=targetspeed)  
         dec_speed();      
      } 
    
    if(targetangle>angle)
    { angle+=turngap;
      if(targetangle<angle)
      angle=targetangle;
      
      servol.write(180-angle+wheelalign);
      servor.write(180-angle);        
    }
    else if(targetangle<angle)
    {
      angle-=turngap;
      if(targetangle>angle)
      angle=targetangle;
      
      servol.write(180-angle+wheelalign);
      servor.write(180-angle);
      
    }
    else
    {    servol.write(180-angle+wheelalign);
         servor.write(180-angle);
    }
  delay(50);
}

void inc_speed()
{
  speedangle+=speedplus;
  if(speedangle>targetspeed)
  speedangle=targetspeed;
}
void dec_speed()
{ 
  speedangle-=speedminus;
  if(speedangle<targetspeed)
  speedangle=targetspeed;
}
void halting()
{
  speedangle-=speedfall;
  if(speedangle<0)
  speedangle=0;
}
