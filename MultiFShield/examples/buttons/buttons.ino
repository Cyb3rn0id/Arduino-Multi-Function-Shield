/*
MultiFShield by cyb3rn0id 
example:
check buttons
pressing button 1 a countes in incremented and showed on display
pressing button 2 counter is decremented
pressing button 3 counter is resetted
keeping button 1 or 2 pressed for more than 3 seconds, will speed-up the counter variation

connections:
- close J2 jumper

Libraries required:
none
*/

#include <MultiFShield.h>

MultiFShield mfshield;
int16_t counter=0;

void setup ()
  {
  Serial.begin(9600);
  mfshield.begin();
  mfshield.puts("butn");
  delay(1000);
  mfshield.displayValue(counter);
  }
 
void loop()
  {
  static uint8_t d=200; // delay between variations
  static uint8_t f=0;
  static uint32_t t=millis();

  if (mfshield.b1Pressed())
    {
    if (counter<10000) counter++; // increment only if we not reached to 9999 (maximum displayable value)
    if (f!=1)
      {
      t=millis();
      f=1;
      }
    }
  else if (mfshield.b2Pressed())
    {
    if (counter>-1000) counter--;  // decrement only if we not reached -999 (minimum displayable value)
    if (f!=2)
      {
      t=millis();
      f=2;
      }
    }
  else if (mfshield.b3Pressed())
    {
    counter=0;
    f=0;
    }
  else
    {
    f=0;  
    }

  mfshield.displayValue(counter);
  
  if ((f) && ((millis()-t)>3000))
    {
    d=50;  
    }
  else
    {
    d=200;  
    }

  delay(d);
  }