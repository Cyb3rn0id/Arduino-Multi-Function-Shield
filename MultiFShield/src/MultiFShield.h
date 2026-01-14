#ifndef MultiFShield_h
  #define MultiFShield_h

#ifndef __AVR_ATmega328P__  // Arduino UNO, NANO
  #error : Library only for Arduino Uno, Nano and so on with ATmega328
#endif

#include "Arduino.h"
#include <avr/interrupt.h>

// leds
#define LED1 		13
#define LED2 		12
#define LED3 		11
#define LED4 		10
// multi-turn trimmer
#define POT			A0
// buzzer
#define BUZZER		3
// buttons
#define BUTTON1		A1
#define BUTTON2		A2 
#define BUTTON3		A3
// shift register
#define	SR_LATCH	4
#define	SR_CLOCK	7
#define	SR_DATA		8
// other pins
#define SENSOR_PIN	A4
#define U5_PIN		A4
#define U4_PIN		2



// digit segment to bit
#define SEGA 0b00000001  
#define SEGB 0b00000010 
#define SEGC 0b00000100  
#define SEGD 0b00001000  
#define SEGE 0b00010000  
#define SEGF 0b00100000 
#define SEGG 0b01000000  
#define SEGP 0b10000000  // decimal point

class MultiFShield
	{
	public:
		MultiFShield(void);
		void begin(void);
		void displayValue(int16_t value, uint8_t pointPos=5);
		void displayClear(void);
		void putch(uint8_t digit, char ch);
		void puts(const char *s, uint8_t startpos=0);
		void buzzerOn(void);
		void buzzerOff(void);
		void beep(uint16_t duration=300);
		void led1On(void);
		void led2On(void);
		void led3On(void);
		void led4On(void);
		void led1Off(void);
		void led2Off(void);
		void led3Off(void);
		void led4Off(void);
		bool b1Pressed(void);
		bool b2Pressed(void);
		bool b3Pressed(void);
		void Timer1_ISR(void);

   private:
		void sendValueToDigit(uint8_t);
		
	};

#endif
