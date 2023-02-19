/* 
 * kekse23.de RCUSB4 v1.1
 * Copyright (c) 2020, Nicholas Regitz
 * 
 * Diese Datei ist Lizensiert unter der Creative Commons 4.0 CC BY-NC-SA
 * https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode
 */

#include <Joystick.h>
#include "AVRPort23.h"

#define CHAN1 D,0
#define _INT1 0
#define CHAN2 D,1
#define _INT2 1
#define CHAN3 D,3
#define _INT3 3
#define CHAN4 D,2
#define _INT4 2
#define RXLED B,0
#define TXLED D,5

//#define ENABLE_CH4
//#define ENABLE_DEBUG
//#define ENABLE_CH34_ANALOG

#ifdef ENABLE_CH4
const int BUTTON_COUNT = 2;
#else
const int BUTTON_COUNT = 1;
#endif

Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_GAMEPAD,
  BUTTON_COUNT, 0,      // Button Count, Hat Switch Count
  true, true, false,    // X, Y, Z
  false, false, false,  // Rx, Ry, Rz
  false, false,         // Rudder, Throttle
  false, false, false); // Accelerator, Brake, Steering

void setup()
{
#ifdef ENABLE_DEBUG
  Serial.begin(9600);
#endif
  portMode(CHAN1, INPUT, HIGH);
  portMode(CHAN2, INPUT, HIGH);
  portMode(CHAN3, INPUT, HIGH);
  portMode(CHAN4, INPUT, HIGH);
  portMode(RXLED, OUTPUT, LOW);
  portMode(TXLED, OUTPUT, LOW);

  Joystick.begin();
  Joystick.setXAxisRange(2250, 750);
  Joystick.setYAxisRange(2250, 750);
#ifdef ENABLE_CH34_ANALOG
#ifdef ENABLE$_CH4
  Joystick.setRxAxisRange(2250, 750);
#endif
  Joystick.setRyAxisRange(2250, 750);
#endif

  attachInterrupt(_INT1, isr1, CHANGE);
  attachInterrupt(_INT2, isr2, CHANGE);
#ifdef ENABLE$_CH4
  attachInterrupt(_INT3, isr3, CHANGE);
#endif
  attachInterrupt(_INT4, isr4, CHANGE);
}

volatile unsigned long Time[4];
volatile unsigned int Value[4];
volatile bool ValChanged[4];
unsigned int NewValue[4];

void loop()
{   
  if (ValChanged[0])
  {
    NewValue[0] = (NewValue[0]+Value[0])/2;
    Joystick.setXAxis(NewValue[0]);
    ValChanged[0] = false;
  }

  if (ValChanged[1])
  {
    NewValue[1] = (NewValue[1]+Value[1])/2;
    Joystick.setYAxis(NewValue[1]);
    ValChanged[1] = false;
  }
#ifdef ENABLE$_CH4
  if (ValChanged[2])
  {
    NewValue[2] = (NewValue[2]+Value[2])/2;
#ifdef ENABLE_DEBUG
    Serial.println(NewValue[2]);
#endif
#ifdef ENABLE_CH34_ANALOG
    Joystick.setRyAxis(NewValue[2]);
#else
    if(NewValue[2] > 1500)
    {
      Joystick.pressButton(1);
    }
    else
    {
      Joystick.releaseButton(1);
    }
#endif
    ValChanged[2] = false;
  }
#endif
  
  if (ValChanged[3])
  {
    NewValue[3] = (NewValue[3]+Value[3])/2;
#ifdef ENABLE_DEBUG
    Serial.println(NewValue[3]);
#endif
#ifdef ENABLE_CH34_ANALOG
    Joystick.setRyAxis(NewValue[3]);
#else
    if(NewValue[3] > 1500)
    {
      Joystick.pressButton(0);
    }
    else
    {
      Joystick.releaseButton(0);
    }
#endif
    ValChanged[3] = false;
  }
  
  delay(1);
}

void isr1()
{
  if (portRead(CHAN1)) Time[0] = micros();
  else if (micros() > Time[0])
  {
    Value[0] = (Value[0]+(micros()-Time[0]))/2;
    ValChanged[0] = true;
  }
}

void isr2()
{
  if (portRead(CHAN2)) Time[1] = micros();
  else if (micros() > Time[1])
  {
    Value[1] = (Value[1]+(micros()-Time[1]))/2;
    ValChanged[1] = true;
  }
}

#ifdef ENABLE$_CH4
void isr3()
{
  if (portRead(CHAN3)) Time[2] = micros();
  else if (micros() > Time[2])
  {
    Value[2] = (Value[2]+(micros()-Time[2]))/2;
    ValChanged[2] = true;
  }
}
#endif

void isr4()
{
  if (portRead(CHAN4)) Time[3] = micros();
  else if (micros() > Time[3])
  {
    Value[3] = (Value[3]+(micros()-Time[3]))/2;
    ValChanged[3] = true;
  }
}
