/*
 Multi-Function Shield Library by Cyb3rn0id
 https://www.github.com/cyb3rn0id
 
 
 Started from Florian Schäffer original one:
 https://github.com/coderfls/Arduino_MultiFShield
 */

#include <MultiFShield.h>

// numbers to digit segments
// index value=number showed on digit
const uint8_t DIGIT_NUMBERS[]=
  {                                                     
  (uint8_t)~(SEGA|SEGB|SEGC|SEGD|SEGE|SEGF),           //  0=0
  (uint8_t)~(SEGB|SEGC),                               //  1=1
  (uint8_t)~(SEGA|SEGB|SEGD|SEGE|SEGG),                //  2=2
  (uint8_t)~(SEGA|SEGB|SEGC|SEGD|SEGG),                //  3=3
  (uint8_t)~(SEGB|SEGC|SEGF|SEGG),                     //  4=4
  (uint8_t)~(SEGA|SEGC|SEGD|SEGF|SEGG),                //  5=5
  (uint8_t)~(SEGA|SEGC|SEGD|SEGE|SEGF|SEGG),           //  6=6
  (uint8_t)~(SEGA|SEGB|SEGC),                          //  7=7
  (uint8_t)~(SEGA|SEGB|SEGC|SEGD|SEGE|SEGF|SEGG),      //  8=8
  (uint8_t)~(SEGA|SEGB|SEGC|SEGD|SEGF|SEGG),           //  9=9
  (uint8_t)~(SEGA|SEGB|SEGC|SEGE|SEGF|SEGG),           // 10=A
  (uint8_t)~(SEGC|SEGD|SEGE|SEGF|SEGG),                // 11=b
  (uint8_t)~(SEGA|SEGD|SEGE|SEGF),                     // 12=C
  (uint8_t)~(SEGB|SEGC|SEGD|SEGE|SEGG),                // 13=d
  (uint8_t)~(SEGA|SEGD|SEGE|SEGF|SEGG),                // 14=E
  (uint8_t)~(SEGA|SEGE|SEGF|SEGG),                     // 15=F
  (uint8_t)~(0)                                        // 16=all segments OFF
  };

// chars to digit segments
const uint8_t DIGIT_CHARS[]{
    ~(SEGA|SEGB|SEGC|SEGE|SEGF|SEGG),      				// A
    ~(SEGC|SEGD|SEGE|SEGF|SEGG),           				// b
    ~(SEGD|SEGE|SEGG),                     				// c (C=SEGA|SEGF|SEGE|SEGD)
    ~(SEGB|SEGC|SEGD|SEGE|SEGG),           				// d
    ~(SEGA|SEGD|SEGE|SEGF|SEGG),           				// E
    ~(SEGA|SEGE|SEGF|SEGG),                				// F
    ~(SEGA|SEGC|SEGD|SEGE|SEGF),           				// G
    ~(SEGC|SEGE|SEGF|SEGG),                				// h
    ~(SEGC),                               				// i (I=SEGB|SEGC)
    ~(SEGB|SEGC|SEGD|SEGE),                				// J
    ~(SEGB|SEGC|SEGE|SEGF|SEGG),           				// K
    ~(SEGF|SEGE|SEGD),                     				// L
    ~(SEGA|SEGF|SEGB|SEGE|SEGC),           				// M (looks like a N)
    ~(SEGG|SEGE|SEGC),                     				// n
    ~(SEGC|SEGE|SEGD|SEGG),                				// o
    ~(SEGA|SEGB|SEGE|SEGF|SEGG),           				// P
    ~(SEGA|SEGF|SEGB|SEGG|SEGC),           				// q
    ~(SEGE|SEGG),                          				// r
    ~(SEGA|SEGF|SEGG|SEGC|SEGD),           				// S
    ~(SEGF|SEGG|SEGE|SEGD),                				// t
    ~(SEGE|SEGD|SEGC),                     				// u
    ~(SEGF|SEGE|SEGD|SEGC|SEGB),           				// v (looks like a U)
    ~(0),                                  				// W (no way to render)
    ~(0),                                  				// X (no way to render)
    ~(SEGF|SEGB|SEGG|SEGC|SEGD),           				// Y
    ~(SEGA|SEGB|SEGG|SEGE|SEGD)            				// Z (same as 2)
    };
	
// pre-defined symbols
const uint8_t DIGIT_BLANK = 0xFF;
const uint8_t DIGIT_SYMBOL_MINUS = ~(SEGG);
const uint8_t DIGIT_SYMBOL_UNDERSCORE = ~(SEGD);
const uint8_t DIGIT_SYMBOL_DOT = ~(SEGP);

// digit select (LSB, commod anode display)
const uint8_t DIGIT_SELECT[] = {0b00000001,0b00000010,0b00000100,0b00001000};

// array containing segments value for each display digit
volatile uint8_t DIGIT_VALUE[4];

static MultiFShield *instance;

//constructor
MultiFShield::MultiFShield(void)  
	{
	instance = this;
	}

// shield init
void MultiFShield::begin(void)
	{
	// buzzer setup
	// buzzer pin high=buzzer off
	// buzzer pin low=buzzer on
	digitalWrite(BUZZER, HIGH); // first turn off the output latch otherwise will emit a short sound at startup
	pinMode(BUZZER, OUTPUT);
	
	// fill digits buffers with blank values
	displayClear();
	
	// shift register setup
	pinMode(SR_LATCH,OUTPUT);
	pinMode(SR_CLOCK,OUTPUT);
	pinMode(SR_DATA,OUTPUT);
    
	// led setup
	// led pin high=led off
	// led pin low=led on
	digitalWrite(LED1,HIGH);
	digitalWrite(LED2,HIGH);
	digitalWrite(LED3,HIGH);
	digitalWrite(LED4,HIGH);
	pinMode(LED1, OUTPUT);
	pinMode(LED2, OUTPUT);
	pinMode(LED3, OUTPUT);
	pinMode(LED4, OUTPUT);
  
	// closing J2 will enable on board buttons pullups
	pinMode(BUTTON1, INPUT);
	pinMode(BUTTON2, INPUT);
	pinMode(BUTTON3, INPUT);
  
	// Timer 1 interrupt
	TCCR1A = 0;                                           // Register loeschen
	OCR1A = 1000;                                         // Vergleichswert x = (CPU / (2 x Teiler x f)) - 1
	TCCR1B |= (1 << CS10) | (1 << CS11) | (1 << WGM12);   // CTC-Mode, Teiler = 64
	TIMSK1 |= (1 << OCIE1A);                              // Output Compare A Match Interrupt Enable
	sei();                                                // IRQ Enable
	}

// show numeric value (positive or negative), eventually turning on the decimal point on the selected digit (1=most left digit, 4=last digit)
void MultiFShield::displayValue(int16_t value, uint8_t pointPos=5)
	{
	if ((value > 9999) || (value < -999))   // out of range
		{
		puts(" out");
		}
	else    // possible range
		{
		if (value >= 0)   // positive values
			{
			if (value > 999)
				DIGIT_VALUE[0] = DIGIT_NUMBERS [(uint8_t) (value / 1000)];
			else
				DIGIT_VALUE[0] = DIGIT_BLANK;
    
			if (value > 99)
				DIGIT_VALUE[1] = DIGIT_NUMBERS [(uint8_t) ((value / 100) % 10)];
			else
				DIGIT_VALUE[1] = DIGIT_BLANK;
    
			if (value > 9) 
				DIGIT_VALUE[2] = DIGIT_NUMBERS [(uint8_t) ((value / 10) % 10)];
			else
				DIGIT_VALUE[2] = DIGIT_BLANK;
    
			DIGIT_VALUE[3] = DIGIT_NUMBERS [(uint8_t) (value % 10)];
			}
		
		// negative value: show minus sign on most-right digit (digit 0)
		if (value < 0)
			{
			value *= -1; // turn value to positive
			DIGIT_VALUE[0] = DIGIT_SYMBOL_MINUS;
    
			if (value > 99)
				DIGIT_VALUE[1] = DIGIT_NUMBERS [(uint8_t) ((value / 100) % 10)];
			else
				DIGIT_VALUE[1] = DIGIT_BLANK;
    
			if (value > 9) 
				DIGIT_VALUE[2] = DIGIT_NUMBERS [(uint8_t) ((value / 10) % 10)];
			else
				DIGIT_VALUE[2] = DIGIT_BLANK;
    
			DIGIT_VALUE[3] = DIGIT_NUMBERS [(uint8_t) (value % 10)];
			}
		}
		
	// add the point to the selected digit
	if (pointPos<4)
		{
		DIGIT_VALUE[pointPos] &= ~SEGP;
		}
	}

void MultiFShield::putch(uint8_t digit, char ch)
    {
    // space
    if (ch==' ')
        {
        DIGIT_VALUE[digit] = DIGIT_BLANK;
        return;
        }
    // minus
    if (ch=='-')
        {
        DIGIT_VALUE[digit] = DIGIT_SYMBOL_MINUS;
        return;
        }
    // underscore
    if (ch=='_')
        {
        DIGIT_VALUE[digit] = DIGIT_SYMBOL_UNDERSCORE;
        return;
        }
    // comma and point
    if ((ch==',') || (ch=='.'))
        {
        DIGIT_VALUE[digit] = DIGIT_SYMBOL_DOT;
        return;
        }
    
    ch-='0'; // subtract 48
    if (ch<10) // it's a number (0-9)
        {
        DIGIT_VALUE[digit] = DIGIT_NUMBERS[ch];
        return;
        }
    else // not a number: remember that I've subtracted '0' (48)
        {
        // uppercase letters (ASCII 65 to 90)
        if ((ch>('A'-'0'-1)) && (ch<('Z'-'0'+1)))
            {
            ch-='A'-'0';
            }
        // lowercase letters (ASCII 97 to 122)
        else if ((ch>('a'-'0'-1)) && (ch<('z'-'0'+1)))
            {
            ch-='a'-'0';
            }
        else
            {
            ch=0;
            }
        }
	DIGIT_VALUE[digit]=DIGIT_CHARS[ch];
    }
	
void MultiFShield::puts(const char *s, uint8_t startpos=0)
    {
    displayClear();
    uint8_t i=0;
    if (startpos>4) startpos=4;
	while(*s)
        {
        putch(startpos+i, *s++);
        i++;
        if ((startpos+i)==4) return; // stop if string reached the end of display
        }
    }
	
void MultiFShield::buzzerOn(void)
	{
	digitalWrite(BUZZER, LOW);
	}
	
void MultiFShield::buzzerOff(void)
	{
	digitalWrite(BUZZER, HIGH);
	}
	
void MultiFShield::beep(uint16_t duration=300)
	{
	buzzerOn();
	delay(duration);
	buzzerOff();
	}

void MultiFShield::displayClear(void) 
	{
	DIGIT_VALUE[0] = DIGIT_BLANK;
	DIGIT_VALUE[1] = DIGIT_BLANK;
	DIGIT_VALUE[2] = DIGIT_BLANK;
	DIGIT_VALUE[3] = DIGIT_BLANK;
	}

void MultiFShield::led1On(void)
	{
	digitalWrite(LED1, LOW);
	}

void MultiFShield::led2On(void)
	{
	digitalWrite(LED2, LOW);
	}

void MultiFShield::led3On(void)
	{
	digitalWrite(LED3, LOW);
	}

void MultiFShield::led4On(void)
	{
	digitalWrite(LED4, LOW);
	}
	
void MultiFShield::led1Off(void)
	{
	digitalWrite(LED1, HIGH);
	}

void MultiFShield::led2Off(void)
	{
	digitalWrite(LED2, HIGH);
	}

void MultiFShield::led3Off(void)
	{
	digitalWrite(LED3, HIGH);
	}

void MultiFShield::led4Off(void)
	{
	digitalWrite(LED4, HIGH);
	}

bool MultiFShield::b1Pressed(void)
	{
	return !digitalRead(BUTTON1);
	}
	
bool MultiFShield::b2Pressed(void)
	{
	return !digitalRead(BUTTON2);
	}
	
bool MultiFShield::b3Pressed(void)
	{
	return !digitalRead(BUTTON3);
	}
	
void MultiFShield::sendValueToDigit(uint8_t digit)
	{
	digitalWrite(SR_LATCH, LOW);                                      // Uebernahme-Takt: Ausgang Aus
	shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, DIGIT_VALUE[digit]);      // Segmente passend zum Zahlwert rausschieben
	shiftOut(SR_DATA, SR_CLOCK, MSBFIRST, DIGIT_SELECT[digit]);     // Welche Stelle leuchten soll hinterher schieben
	digitalWrite(SR_LATCH, HIGH);                                     // Uebernahme-Takt: Ausgang Ein
	}


// Called on Timer 1 interrupt
// multiplexing digits
void MultiFShield::Timer1_ISR(void) 
	{
	static uint8_t actual_digit=0;
	switch (++actual_digit)
		{
		case 1: 
			sendValueToDigit(0); 
			break;     
		case 2: 
			sendValueToDigit(1);
			break; 
		case 3: 
			sendValueToDigit(2); 
			break; 
		case 4: 
			sendValueToDigit(3); 
			actual_digit = 0; 
			break; 
		}
	}
	
// Timer 1 interrupt
ISR(TIMER1_COMPA_vect)
	{
	instance->Timer1_ISR();
	}