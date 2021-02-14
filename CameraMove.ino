#include <SoftwareSerial.h>
#include<Servo.h>
Servo m,M;
int TX = 5; 
int RX = 6; 
int value;
char ch,UorD;
int HEpin=A0,HEsense,HED=0;//orange colour wire
int x;
int AngleMicro;
int record;char Direction;
int CPWD=1,CPWDu=1,LPWD=1,RPWD=1,UPWD=1,DPWD=1;//All the <Button> Pressed Work Done initiators
int Passed=0,temprecord;char tempDirection;//Variables of Pass Function
int rall=7;
SoftwareSerial HC_06(TX, RX);  //Bluetooth TX to 10 and Bluetooth RX to 11.
//=====================================================
void setup()
{Serial.begin(57600);
HC_06.begin(57600);
 m.attach(A1);//white colour wire
M.attach(A2);  //yellow colour wire
pinMode(HEpin,INPUT);
 pinMode(rall,OUTPUT);
 digitalWrite(rall,LOW);
 delay(100);
AngleMicro=100;
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
}
//=====================================================Hall Effect Analog to Digital Converter
int HEADC()
{HEsense=analogRead(HEpin);
  if(HEsense<=611&&HEsense>=575)
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
//==================================================Which direction should the micro servo move to come to the center
char UpOrDown()
{if(AngleMicro>100)
return 'U';
if(AngleMicro<100)
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
  {if(AngleMicro!=100)
    {AngleMicro-=2;m.write(AngleMicro);}
    else
    CPWDu=1;}
    if(CPWDu==-1)
    {if(AngleMicro!=100)
      {AngleMicro+=2;m.write(AngleMicro);}
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
   else if(UPWD==0&& AngleMicro>20)
     {AngleMicro-=2;m.write(AngleMicro);
     }
   else if(DPWD==0&&AngleMicro<180)
     {AngleMicro+=2;m.write(AngleMicro);
     }
 }
}
void loop()
{
  if(HC_06.available()> 0 ) 
  {
      value = HC_06.parseInt();
      ch=HC_06.read();
      CameraMove(ch);
      Serial.print(value); 
      Serial.print(" value "); 
      Serial.print(ch); 
      Serial.print("\n"); 
  }
  else
  CameraMove('$');
  delay(5);
}
