#include <SoftwareSerial.h>
#include<Servo.h>
#include "ACS712.h"

ACS712 sensor(ACS712_05B, A7);
Servo m,M;
Servo servol,servor,pwm;
int TX = 5; 
int RX = 6; 
int ST=13;
int change=1,index=4,temp,voltage,flag=0,STATE;
double volt,tol=0.2;

int charge=0,chargechange=0;
float currentReading;
float ucutoff=-0.05,lcutoff=-0.70;

SoftwareSerial HC_06(TX, RX);  //Bluetooth TX to 5 and Bluetooth RX to 6.
char ch,UorD;
int HEpin=A0,HEsense,HED=0;//orange colour wire
int x,y;
int AngleMicro;
int record;char Direction;
int CPWD=1,CPWDu=1,LPWD=1,RPWD=1,UPWD=1,DPWD=1;//All the <Button> Pressed Work Done initiators
int Passed=0,temprecord;char tempDirection;//Variables of Pass Function
int rall=7,rs=8;
const int maxspeed=90;
int rmove1=11,rmove2=12;
int speedplus=5,speedminus=5,speedfall=3,turngap=3,ind=0;
int value,wheelalign=-4,angle=90,targetangle=90,speedangle=0,targetspeed=0,bgstate=0;
int trigger=10,echo=9,Collide=0,OneTime1=0,OneTime2=0;int measure4time=1;float mindistance=0;//CollisionOrNot Related variables
float Time=0,distance=0;
//=====================================================
void setup()
{Serial.begin(57600);
HC_06.begin(57600);
  pinMode(ST,INPUT);
  servol.attach(A4); 
  servor.attach(A5);
  pwm.attach(A3);
   pinMode(trigger,OUTPUT);
 pinMode(echo,INPUT);
  pinMode(rmove1,OUTPUT);
  pinMode(rmove2,OUTPUT);
   pinMode(4,OUTPUT);//white LED
  digitalWrite(rmove1,HIGH);
  digitalWrite(rmove2,HIGH);
  servol.write(90+wheelalign);
  servor.write(90);
  pwm.write(0);
 m.attach(A1);//white colour wire
M.attach(A2);  //yellow colour wire
pinMode(HEpin,INPUT);
 pinMode(rall,OUTPUT); //Relay pins being given initial values
 digitalWrite(rall,LOW);
 pinMode(rs,OUTPUT);
 digitalWrite(rs,HIGH);
 delay(2000);
if(HEADC()!=1)
  {while(HEADC()!=1)
  M.write(86);
  delay(89);
  M.write(91);
  }
else
  {while(HEADC()==1)
   M.write(89);
   delay(5);
   M.write(97);
   delay(200);
   M.write(91);
  }
  AngleMicro=130;
 m.write(AngleMicro);
 sensor.calibrate(); 
}
//=====================================================Hall Effect Analog to Digital Converter
int HEADC()
{HEsense=analogRead(HEpin);
  if(HEsense<=611&&HEsense>=576)
  {HED=0;return HED;}
  else if(HEsense>=647)
   {HED=2;return HED;}
   else if(HEsense<=557)
   {HED=1;return HED;}
   else
   return HED;
}
//=======================================Assign the direction in which the 360 Servo must rotate to be centered
int ClockOrAnti(int R,char D)
{if(R==1)
   {if(D=='C')
      return 86;
    else
      return 97;
   }
 else
   {if(D=='C')
      return 97;
    else
      return 86;
   }
}
//===============================Keeping record of the direction and magnet number in one "pass"(HEADC() 0 then non 0 )
void Pass(char D)
{x=HEADC();
  if(x!=0)
    { temprecord=x;
      tempDirection=D;
      Passed=1;
    }
 else if(Passed==1)
    { record=temprecord;
      Direction=tempDirection;
      Passed=0;
    }
}
//===================================battery level indication
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

//==================================================Which direction should the micro servo move to come to the center
char UpOrDown()
{if(AngleMicro>130)
return 'U';
if(AngleMicro<130)
return 'D';
return 'N';
}
//========================================================Function for Camera Movement
void CameraMove(char CH)
{if(CH=='C')
  {UorD=UpOrDown();
  if(UorD=='U')
  CPWDu=0;
  else if(UorD=='D')
  CPWDu=-1;
  else;
    if(HEADC()==1)
      CPWD=-1;
    else
      CPWD=0;
  }
if(CPWD==-1||CPWD==0||CPWDu==0||CPWDu==-1)
{
  if(CPWDu==0)
  {if(AngleMicro!=130)
    {AngleMicro-=1;m.write(AngleMicro);}
    else
    CPWDu=1;}
    if(CPWDu==-1)
    {if(AngleMicro!=130)
      {AngleMicro+=1;m.write(AngleMicro);}
      else
      CPWDu=1;}
 if(CPWD==-1)
 {if(HEADC()==1)
  M.write(89);
  else
   {delay(5);
   M.write(97);
   delay(200);
   M.write(91);CPWD=1;
   }
 }
 if(CPWD==0)
 {if(HEADC()!=1)
  M.write(ClockOrAnti(record,Direction));
  else
    {delay(84);M.write(91);CPWD=1;}
 }
}
else
{switch(CH)
    { case 'L':LPWD=0;break;
      case 'R':RPWD=0;break;
      case 'U':UPWD=0;break;
      case 'D':DPWD=0;break;
      case '#':M.write(91);LPWD=1;RPWD=1;break;
      case '@':UPWD=1;DPWD=1;break;
    }
   if(LPWD==0)
     { M.write(89);
      Pass('A');
     }
   else if(RPWD==0)
     {M.write(94);
      Pass('C');
     }
   else if(UPWD==0&& AngleMicro>40)
     {AngleMicro-=1;m.write(AngleMicro);
     }
   else if(DPWD==0&&AngleMicro<180)
     {AngleMicro+=1;m.write(AngleMicro);
     }
 }
}
//==================================================Collision Alert and Ultrasonic Sensor Control
int CollisionOrNot()
{ if(measure4time<=20)
{  digitalWrite(trigger,LOW);
    delayMicroseconds(2);
    digitalWrite(trigger,HIGH);
    delayMicroseconds(10);
    digitalWrite(trigger,LOW);
    Time=pulseIn(echo,HIGH);
    distance=Time*0.034/2;
     if(measure4time==1)
     mindistance=distance;
      if(distance<=mindistance)
          mindistance=distance;  
          measure4time++;
 Serial.print(distance); Serial.print("  ");
 Serial.print(mindistance); Serial.print(" \n");
           
        return 2;
}
else
    {measure4time=1;
      if(mindistance>39.62)
         {Collide=1;return Collide;}
      else if(mindistance<39)
         {Collide=0;return Collide;}
      else
         return Collide;
}
}
//=====================================Spraying Function
void Spraying(int W)
{if(W==1)
  digitalWrite(rs,LOW);
 else
   digitalWrite(rs,HIGH);
}
//====================================determines whether charging is occuring or not
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

//=======================================The loop function
void loop()
{    STATE=digitalRead(ST); 
    currentReading = sensor.getCurrentDC();
    Serial.println(currentReading);
 if(chargingornot())
 {    if(chargechange==1)
      {chargechange=0;
      flag=0;
      digitalWrite(rall,HIGH);
      digitalWrite(rs,HIGH);
      digitalWrite(4,HIGH);
      }
          if(STATE==HIGH)
          { 
             if(flag==0)
             {
             HC_06.print(6);
              flag=1;
             }
          }
         else
         { 
            flag=0;
         }
 }
 else
 { if(chargechange==0)
      {
        digitalWrite(rall,LOW);
        digitalWrite(rs,HIGH);
        digitalWrite(4,LOW);
        chargechange=1;
        flag=0;
      }
       voltage=analogRead(A6);
   volt=map(voltage,0,1023,0,2500);
   volt/=100;
   temp=index;
   Serial.print(volt);
   Serial.println(" voltage");   

  // batteryvoltagelevel();
  
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
  if(CollisionOrNot()==1&& OneTime1==0)
 {OneTime1=1;OneTime2=0;HC_06.println(8);}
else if(CollisionOrNot()==0&& OneTime2==0)
{OneTime2=1;OneTime1=0;HC_06.println(7);}
else;
  if(HC_06.available()> 0 ) 
  {
      value = HC_06.parseInt();
      ch=HC_06.read();
     CameraMove(ch);
      if(ch=='S')   //calling spray function if pressed 'S'
      Spraying(1);
      if(ch=='s')
      Spraying(0);
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
          else
          {digitalWrite(rmove1,LOW);
          digitalWrite(rmove2,HIGH);
          }
        }
        else if(value>72&&value<=120)
        {
          targetspeed=map(value,72,120,0,maxspeed);
          digitalWrite(rmove1,HIGH);
          digitalWrite(rmove2,LOW);
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
  else
   CameraMove('$');
   if(bgstate==0)
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
    }if(HC_06.available()> 0 ) 
  {
      value = HC_06.parseInt();
      ch=HC_06.read();
     CameraMove(ch);
      if(ch=='S')   //calling spray function if pressed 'S'
      Spraying(1);
      if(ch=='s')
      Spraying(0);
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
          else
          {digitalWrite(rmove1,LOW);
          digitalWrite(rmove2,HIGH);
          }
        }
        else if(value>72&&value<=120)
        {
          targetspeed=map(value,72,120,0,maxspeed);
          digitalWrite(rmove1,HIGH);
          digitalWrite(rmove2,LOW);
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
  else
   CameraMove('$');
   if(bgstate==0)
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
}
  
  delay(40);
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
