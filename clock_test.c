/*****************************************************
 This program was produced by the
 CodeWizardAVR V2.05.3 Professional
 Automatic Program Generator
 � Copyright 1998-2011 Pavel Haiduc, HP InfoTech s.r.l.
 http://www.hpinfotech.com

 Project :
 Version :
 Date    : 02.05.2016
 Author  : ttfaire
 Company :
 Comments:


 Chip type               : ATmega8
 Program type            : Application
 AVR Core Clock frequency: 8,000000 MHz
 Memory model            : Small
 External RAM size       : 0
 Data Stack size         : 256
 *****************************************************/

#include <mega8.h>
#include <delay.h>
#include "ds3231_twi.c"

//segments
#define A   1
#define B   2
#define C   4 
#define D   8  
#define E  16 
#define F  64 
#define G  128
#define DP  32
//didits pins 
#define DIGIT_1  8
#define DIGIT_2  4
#define DIGIT_3  2
#define DIGIT_4  1

#define MODE_SHOW_MAIN_INFO 0
#define MODE_SET_DATE_YEAR 1
#define MODE_SET_DATE_MONTH 2
#define MODE_SET_DATE_DAY_OF_WEEK 3
#define MODE_SET_DATE_DAY 4
#define MODE_SET_TIME_HOUR 5
#define MODE_SET_TIME_MINUTE 6

static flash unsigned char digit[] = {
	0b11111111 ^(A+B+C+D+E+F),   // 0
	0b11111111 ^(B+C),// 1
	0b11111111 ^(A+B+D+E+G),// 2
	0b11111111 ^(A+B+C+D+G),// 3
	0b11111111 ^(B+C+F+G),// 4
	0b11111111 ^(A+C+D+F+G),// 5
	0b11111111 ^(A+C+D+E+F+G),// 6
	0b11111111 ^(A+B+C),// 7
	0b11111111 ^(A+B+C+D+E+F+G),// 8
	0b11111111 ^(A+B+C+D+F+G),// 9
	0b11111111 ^(A+B+C+E+F+G),// A - 10
	0b11111111 ^(C+D+E+F+G),// b - 11
	0b11111111 ^(A+D+E+F),// C - 12
	0b11111111 ^(B+C+D+E+G),// d - 13
	0b11111111 ^(A+D+E+F+G),// E - 14
	0b11111111 ^(A+E+F+G),// F - 15
	0b11111111 ^(G),// 16 - minus
	0b11111111 ^(A+B+F+G),// 17 - grad?
	0b11111111 ^(0),// 18 - blank
	0b11111111 ^(C+E+G),// n
	0b11111111 ^(D+E+F+G),// t
	0b11111111 ^(A),// upper
	0b11111111 ^(D)// lower
};

#define D_SYMBOL   13
#define T_SYMBOL   20

static flash unsigned char commonPins[] = {
	DIGIT_1,
	DIGIT_2,
	DIGIT_3,
	DIGIT_4
};

unsigned char digit_out[4], cur_dig = 0;

unsigned char seconds;
unsigned char minutes;
unsigned char hours;
unsigned char day;
unsigned char date;
unsigned char month;
unsigned char year;

bit button_1_on1;
bit button_2_on1;
bit button_3_on1;
bit button_1_on2;
bit button_2_on2;
bit button_3_on2;
bit button_1_on3;
bit button_2_on3;
bit button_3_on3;

unsigned char i;

unsigned char mode;

void doBtn1Action(void) {
	mode = mode < 6 ? (mode + 1) : 0;
}

unsigned char getLastMonthDay(void) {
	unsigned char retVal = 31;
	switch (month) {
	case 1:
	case 3:
	case 5:
	case 7:
	case 8:
	case 10:
	case 12:
		retVal = 31;
		break;
	case 4:
	case 6:
	case 9:
	case 11:
		retVal = 30;
		break;
	case 2:
		retVal = year % 4 == 0 ? 29 : 28;
		break;
	}
	return retVal;
}
void doBtn2Action(void) {
	switch (mode) {
	case MODE_SHOW_MAIN_INFO: {
		// mode = MODE_SHOW_DAY_INFO;
		break;
	}
	case MODE_SET_DATE_YEAR: {
		year = year < 99 ? (year + 1) : 0;
		rtc_set_time(minutes, hours, day, date, month, year);
		break;
	}
	case MODE_SET_DATE_MONTH: {
		month = month < 12 ? (month + 1) : 1;
		rtc_set_time(minutes, hours, day, date, month, year);
		break;
	}
	case MODE_SET_DATE_DAY_OF_WEEK: {
		day = day < 7 ? (day + 1) : 1;
		rtc_set_time(minutes, hours, day, date, month, year);
		break;
	}
	case MODE_SET_DATE_DAY: {
		date = date < getLastMonthDay() ? (date + 1) : 1;
		rtc_set_time(minutes, hours, day, date, month, year);
		break;
	}
	case MODE_SET_TIME_HOUR: {
		hours = hours < 23 ? (hours + 1) : 0;
		rtc_set_time(minutes, hours, day, date, month, year);
		break;
	}
	case MODE_SET_TIME_MINUTE: {
		minutes = minutes < 59 ? (minutes + 1) : 0;
		rtc_set_time(minutes, hours, day, date, month, year);
		break;
	}

	}
}

void doBtn3Action(void) {
	switch (mode) {
	case MODE_SHOW_MAIN_INFO: {
		//LED_BACKLIGT = ~LED_BACKLIGT;
		// LED_GREEN = ~LED_GREEN;
		break;
	}
	case MODE_SET_DATE_YEAR: {
		year = year > 0 ? (year - 1) : 99;
		rtc_set_time(minutes, hours, day, date, month, year);
		break;
	}
	case MODE_SET_DATE_MONTH: {
		month = month > 1 ? (month - 1) : 12;
		rtc_set_time(minutes, hours, day, date, month, year);
		break;
	}
	case MODE_SET_DATE_DAY_OF_WEEK: {
		day = day > 1 ? (day - 1) : 7;
		rtc_set_time(minutes, hours, day, date, month, year);
		break;
	}
	case MODE_SET_DATE_DAY: {
		date = date > 1 ? (date - 1) : getLastMonthDay();
		rtc_set_time(minutes, hours, day, date, month, year);
		break;
	}
	case MODE_SET_TIME_HOUR: {
		hours = hours > 0 ? (hours - 1) : 23;
		rtc_set_time(minutes, hours, day, date, month, year);
		break;
	}
	case MODE_SET_TIME_MINUTE: {
		minutes = minutes > 0 ? (minutes - 1) : 59;
		rtc_set_time(minutes, hours, day, date, month, year);
		break;
	}
	}
}

// Timer1 overflow interrupt service routine
interrupt [TIM1_OVF] void timer1_ovf_isr(void)
{
	if(!PINC.0) {
		if(button_1_on1) {
			if(button_1_on2) {
				if(button_1_on3) {
					doBtn1Action();

					button_1_on1 = 0;
					button_1_on2 = 0;
					button_1_on3 = 0;
				} else {
					button_1_on3 = 1;
				}
			} else {
				button_1_on2 = 1;
				button_1_on3 = 0;
			}
		} else {
			button_1_on1 = 1;
			button_1_on2 = 0;
			button_1_on3 = 0;
		}
	}
	if(!PINC.1) {
		if(button_2_on1) {
			if(button_2_on2) {
				if(button_2_on3) {
					doBtn2Action();

					button_2_on1 = 0;
					button_2_on2 = 0;
					button_2_on3 = 0;
				} else {
					button_2_on3 = 1;
				}
			} else {
				button_2_on2 = 1;
				button_2_on3 = 0;
			}
		} else {
			button_2_on1 = 1;
			button_2_on2 = 0;
			button_2_on3 = 0;
		}
	}
	if(!PINC.2) {
		if(button_3_on1) {
			if(button_3_on2) {
				if(button_3_on3) {
					doBtn3Action();

					button_3_on1 = 0;
					button_3_on2 = 0;
					button_3_on3 = 0;
				} else {
					button_3_on3 = 1;
				}
			} else {
				button_3_on2 = 1;
				button_3_on3 = 0;
			}
		} else {
			button_3_on1 = 1;
			button_3_on2 = 0;
			button_3_on3 = 0;
		}
	}

//	if(!PINC.1) {
//		if(button_2_on) {
//			doBtn2Action();
//		}
//		button_2_on = !button_2_on;
//	}
//	if(!PINC.2) {
//		if(button_3_on) {
//			doBtn3Action();
//		}
//		button_3_on = !button_3_on;
//	}
	TCNT1=0;
	PORTB.6=~PORTB.6;
}

// Timer2 overflow interrupt service routine
interrupt [TIM2_OVF] void timer2_ovf_isr(void) {
	PORTB&=0b11110000;
	PORTD=digit[digit_out[cur_dig]];

	PORTB |= commonPins[cur_dig];
	cur_dig++;
	if (cur_dig > 3) {
	cur_dig = 0;
	}
}

// Declare your global variables here

void displayInfo(void) {
	switch (mode) {
	case MODE_SHOW_MAIN_INFO:
		digit_out[0] = hours / 10;
		digit_out[1] = hours % 10;
		digit_out[2] = minutes / 10;
		digit_out[3] = minutes % 10;
		break;

	case MODE_SET_DATE_YEAR:
		digit_out[0] = D_SYMBOL;
		digit_out[1] = 1;
		digit_out[2] = year / 10;
		digit_out[3] = year % 10;
		break;
	case MODE_SET_DATE_MONTH:
		digit_out[0] = D_SYMBOL;
		digit_out[1] = 2;
		digit_out[2] = month / 10;
		digit_out[3] = month % 10;
		break;
	case MODE_SET_DATE_DAY_OF_WEEK:
		digit_out[0] = D_SYMBOL;
		digit_out[1] = 3;
		digit_out[2] = day / 10;
		digit_out[3] = day % 10;
		break;
	case MODE_SET_DATE_DAY:
		digit_out[0] = D_SYMBOL;
		digit_out[1] = 4;
		digit_out[2] = date / 10;
		digit_out[3] = date % 10;
		break;

	case MODE_SET_TIME_HOUR:
		digit_out[0] = T_SYMBOL;
		digit_out[1] = 1;
		digit_out[2] = hours / 10;
		digit_out[3] = hours % 10;
		break;
	case MODE_SET_TIME_MINUTE:
		digit_out[0] = T_SYMBOL;
		digit_out[1] = 2;
		digit_out[2] = minutes / 10;
		digit_out[3] = minutes % 10;
		break;
	}
}

void main(void) {
unsigned char i=0;

	PORTB = 0xFF;
	DDRB = 0xFF;

	PORTC = 0x07;
	DDRC = 0xF8;
	;

	PORTD = 0xFF;;
	DDRD = 0xFF;

// Timer/Counter 0 initialization
// Clock source: System Clock
// Clock value: Timer 0 Stopped
	TCCR0 = 0x00;
	TCNT0 = 0x00;

	// Timer/Counter 1 initialization
	// Clock source: System Clock
	// Clock value: 7,813 kHz
	// Mode: Normal top=0xFFFF
	// OC1A output: Discon.
	// OC1B output: Discon.
	// Noise Canceler: Off
	// Input Capture on Falling Edge
	// Timer1 Overflow Interrupt: On
	// Input Capture Interrupt: Off
	// Compare A Match Interrupt: Off
	// Compare B Match Interrupt: Off
	TCCR1A=0x00;
	TCCR1B=0x02;
	TCNT1H=0x00;
	TCNT1L=0x00;
	ICR1H=0x00;
	ICR1L=0x00;
	OCR1AH=0x00;
	OCR1AL=0x00;
	OCR1BH=0x00;
	OCR1BL=0x00;

	// Timer/Counter 2 initialization
	// Clock source: System Clock
	// Clock value: 62,500 kHz
	// Mode: Normal top=0xFF
	// OC2 output: Disconnected
	ASSR = 0x00;
	TCCR2 = 0x05;
	TCNT2 = 0x00;
	OCR2 = 0x00;

// External Interrupt(s) initialization
// INT0: Off
// INT1: Off
	MCUCR = 0x00;

// Timer(s)/Counter(s) Interrupt(s) initialization
	TIMSK = 0x44;

// USART initialization
// USART disabled
	UCSRB = 0x00;

// Analog Comparator initialization
// Analog Comparator: Off
// Analog Comparator Input Capture by Timer/Counter 1: Off
	ACSR = 0x80;
	SFIOR = 0x00;

// ADC initialization
// ADC disabled
	ADCSRA = 0x00;

// SPI initialization
// SPI disabled
	SPCR = 0x00;

//twi_master_init(100);
	TWBR = 0x0C;
	TWAR = 0xD0;
	TWCR = 0x44;
	;

// Global enable interrupts
#asm("sei")

//    PORTB = 0x00;
//	PORTD = 0xFF;
	

	ds3231_init();

	rtc_get_time(&seconds, &minutes, &hours, &day, &date, &month, &year);
////
//	digit_out[0] = 1;
//	digit_out[1] = 2;
//	digit_out[2] = 3;
//	digit_out[3] = 4;
    
//    PORTB.1= 1;
//    
//    PORTD.4 = 0;

	while (1) {
		rtc_get_time(&seconds, &minutes, &hours, &day, &date, &month, &year);
		// Place your code here
		displayInfo();
		delay_ms(100);
//i++;
//digit_out[0] = i%10;
//	digit_out[1] = (i+1)%10;
//	digit_out[2] = (i+2)%10;
//	digit_out[3] = (i+3)%10;
//    delay_ms(1000);

	}
}
