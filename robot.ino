//
#include <driverL293D_Shift595.h>
#include <IRremote.h>
#include <Servo.h>

Shild_motor koleso_l(1);
Shild_motor koleso_r(2);

IRrecv pult(9);
decode_results signal_pult;

Servo servo;

#define zvuk_otpr 14
#define zvuk_preem 15

int ugol = 90;
int smotrim_vlevo;
int smotrim_vpravo;
int smotrim_priamo;
int vremia;
boolean autopilot_on = false;


void setup() {
  servo.attach(10);
  pult.enableIRIn();
  pinMode(zvuk_otpr,OUTPUT);
  pinMode(zvuk_preem, INPUT);
  Serial.begin(9600);
  servo.write(80);
}

void ehat_priamo()
{ 
  koleso_r.forward();
  koleso_l.forward(); 
}

void ehat_nazad()
{ 
  koleso_l.backward();
  koleso_r.backward();
}

void ehat_vpravo()
{
  koleso_l.forward();
  koleso_r.backward();
}

void ehat_vlevo()
{ 
  koleso_l.backward();
  koleso_r.forward();
}

void stoiat()
{                 
  koleso_l.stop();
  koleso_r.stop();
}

void kak_meriat_sleva()
{               
  digitalWrite(zvuk_otpr, HIGH);
  delayMicroseconds(10);
  digitalWrite(zvuk_otpr, LOW);
  vremia = pulseIn(zvuk_preem, HIGH);
  smotrim_vlevo = vremia/58;
}

void kak_meriat_priamo()
{        
  digitalWrite(zvuk_otpr, HIGH);
  delayMicroseconds(10);
  digitalWrite(zvuk_otpr, LOW);
  vremia = pulseIn(zvuk_preem, HIGH);
  smotrim_priamo = vremia/58;
}

void kak_meriat_sprava()
{           
  digitalWrite(zvuk_otpr, HIGH);
  delayMicroseconds(10);
  digitalWrite(zvuk_otpr, LOW);
  vremia = pulseIn(zvuk_preem, HIGH);
  smotrim_vpravo = vremia/58;
}

unsigned long time_start, time_stop;
int ehat,temp;


void loop() 
{
  if (pult.decode(&signal_pult))
  {
    switch(signal_pult.value)
    {
      case 0x4DE74847: case 0x1002C2D: case 0x64830176:
        ehat = 1;
        time_start = millis();
        break;
      case 0xB8781EF: case 0x100ACAD:
        ehat = 2;
        time_start = millis();
        break;
      case 0xBE3BB37:
        ehat = 3;
        time_start = millis();
        break;
      case 0xD4D9F2A7:
        ehat = 4;
        time_start = millis();
        break;
      case 0x7D168BCF:
        ehat = 5;
        break;
      case 0xBB0ED9E1:
        ehat = 6;
        break;
      case 0x757FB4DF:
        ehat = 7;
        break;
      default:
        ehat = temp;
        break; 
    }
    pult.resume();
    temp = ehat;
  }
  if (ehat == 6)
    autopilot_on = true;
  if (ehat == 7)
    autopilot_on = false;
  if (autopilot_on)
  {
    servo.write(80);
    kak_meriat_priamo();
    if(smotrim_priamo < 15)
    {
      stoiat();
      servo.write(35);
      kak_meriat_sleva();
      if(smotrim_vlevo < 15)
      {
        servo.write(10);
        kak_meriat_sleva();
        if(smotrim_vlevo < 15)
        {
          servo.write(120);
          kak_meriat_sprava();
          if(smotrim_vpravo < 15)
          {
            servo.write(160);
            kak_meriat_sprava();
            if(smotrim_vpravo < 15)
            {
              ehat_vpravo();
              
            }
            else
            {
              ehat_vpravo();
              
            }
          }
          else
          {
            ehat_vpravo();
            
          }
        }
        else
        {
          ehat_vlevo();
          
        }
      }
      else
      {
        ehat_vlevo();
        
      }
    }
    else
    {
      ehat_priamo();
    }
  }
  else if (!autopilot_on)
  {
    switch(ehat)
    {
    case 1:
      ehat_priamo();
      break;
    case 2:
      ehat_nazad();
      break;
    case 3:
      ehat_vpravo();
      break;
    case 4:
      ehat_vlevo();
      break;
    case 5:
      stoiat();
      break;
    }
  }
  time_stop = millis();  
  if (((time_stop - time_start) > 205) && autopilot_on == false) 
  {
    stoiat();
    ehat = -1;
  }
}
