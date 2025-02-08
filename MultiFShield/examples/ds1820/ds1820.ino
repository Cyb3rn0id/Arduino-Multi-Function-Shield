/*
MultiFShield by cyb3rn0id 
example:
Shows temperature from DS1820 on the display
differencies between DS18(x)20 sensors:
DS1820  = 9bit resolution
DS18S20 = 12bit resolution with same configuration registers of DS1820
DS18B20 = configurable resolution from 9 to 12 bit
more on: https://www.analog.com/en/resources/design-notes/1wire-protocol-pdf-of-ds18s20-vs-ds18b20-digital-themormeters.html

connections:
- attach a DS1820 on J4 header as showed on silkscreen
- close J1 jumper

Libraries required:
- onewire by Jim Studt, Tom Pollard and others (https://github.com/PaulStoffregen/OneWire)
- DallasTemperature by Miles Burton (https://github.com/milesburton/Arduino-Temperature-Control-Library)
*/

#include <MultiFShield.h>
#include <OneWire.h>
#include <DallasTemperature.h>

MultiFShield mfshield;
OneWire oneWire(SENSOR_PIN); // sensor pin is defined in the multifshield library

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// array to hold device address
DeviceAddress sensorAddress;

void setup ()
  {
  Serial.begin(9600);
  sensors.begin();
  mfshield.begin();
  delay(100);
  
  // check for Dallas devices on the bus and store address if we find one
  if (!sensors.getAddress(sensorAddress, 0))
    {
    // no sensors found
    mfshield.puts("err",1); // write "err" starting from digit 1 (the first digit, the ledt-most, is 0)
    mfshield.beep();
    while(1);
    }

  sensors.setResolution(sensorAddress, 9); // works only on DS18B20
  mfshield.puts("temp");
  delay(1000);
  }
 
void loop()
  {
  sensors.requestTemperatures();
  delay(1000);
  float tempC = sensors.getTempC(sensorAddress);
  if (tempC == DEVICE_DISCONNECTED_C)
    {
    Serial.println("Sensor Disconnected");
    mfshield.puts("err",1);
    mfshield.beep();
    }
  else
    {
    Serial.println(tempC);
    tempC*=10; // multiply by 10, so I can print an integer and I can put the decimal point on the third digit
    mfshield.displayValue((uint16_t)tempC, 2); // put the comma on the third digit (number 2)
    }
  }