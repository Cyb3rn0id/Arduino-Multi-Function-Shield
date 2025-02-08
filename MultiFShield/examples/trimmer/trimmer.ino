/*
MultiFShield by cyb3rn0id 
example:
Shows reading from onboard trimmer

connections:
none

Libraries required:
none
*/

#include <MultiFShield.h>

MultiFShield mfshield;

void setup ()
  {
  Serial.begin(9600);
  mfshield.begin();
  mfshield.puts("trim");
  delay(1000);
  }
 
void loop()
  {
  uint16_t an=0;
  // perform 16 analog readings
  for (uint8_t i=0; i<16; i++)
    {
    an += analogRead(POT);
    delay(10);
    }
  // right shift by 4 = division by 2^4 (16. faster than a division)
  an >>= 4; 
  mfshield.displayValue(an);
  Serial.println(an);
  delay(100);
  }