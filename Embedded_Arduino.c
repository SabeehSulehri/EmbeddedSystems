/*
 *
 * Created: 10/18/2022 2:17:17 PM
 *  Author: Sabeeh ur Rehman Sulehri
 *
 */ 

// This project consists of 3 LEDs, Breadboard, Arduino,
// potentiometer, press button, wires and resistors

// In this project 3 LEDs constantly change their brightness 
// gradually from dark to bright and when at full brightness it
// resets the brightness to dark and when the button is pressed, 
// the direction of brightness changes to the complete opposite.

// I.e., after pressing the button the 3 LEDs brightness 
// goes from light to dark and when you press it again the
// brightness goes from dark to light.

// The potentiometer in the project varies the speed of the
// gradual change of the brightness of the 3 LEDs.

// I.e., when potentiometer at full then the speed of 
// increase/decrease of brightness would be at it's slowest
// and the potentiometer's value is decreased, the speed of the
// gradual change would increase to a point where no change can
// be seen as its variance is too small.


#include <avr/interrupt.h>
#include <avr/io.h>
#define F_CPU 8000000UL
#include <util/delay.h>


int main(void)
{
	cli(); //clear interrupts;
	DDRD = (1<<PORTD1);
	//OCR1A = 5;
	TCCR1B |= (1<<COM1A1)|(1<<COM1B1);
	TCCR1B |= (1<<WGM10);
	TCCR1B |= (1<<WGM12);
	TIMSK1 |= (1<< OCIE1A);
	TCCR1B |= (1<<CS10); //fast pwm mode
	sei(); //set interrupts

	EIMSK = 0b00000001; // interrupt enabler
	EICRA = 0b00000011; // interrupt control. when 11 at the end it takes 1 of both the ISC01 and ISC00
	// so it will change on just the rising edge

	// ADC Setup
	ADCSRA = 0b10000111;	// Enable Analog Digital Converter, set pre-scaler to 128
	ADCSRB = 0b00000000;	// Free Running mode
	ADMUX  = 0b00000000;	// Select ADC0 as input, AREF pin to be V_Ref
	DDRC   = 0x00;			// Set PORTC as input

	while(1)
	{
				// Set "Start Conversion" bit
				ADCSRA |= 1 << ADSC;
				
				// Adjust LED brightness based off of potentiometer
				//OCR1A = ADC * 64;
				// ADC = 1024*V(in)/V(aref) [0, 1023]
				// map [0, 1023] to [0, 65535]
				// -> multiply x by 64
		
		// ADC gives us the return value from the potentiometer which can be used to vary the brightness
	}
}

// global variables declaration
int num=10;
int on=0;


ISR(TIMER1_COMPA_vect) 
// interrupt enabled from the timer which increases the brightness from low to high until
// the limit is reached and then re-dos the thing from low to high in a forever loop
{

	if(on){
		OCR1A = 62500 ; //0xFF
		on=0;
	}
	else {
		OCR1A = num;
		on=1;
	}
	num=num+100;

	if(num>=55000000) {num=10;}
		// if i put ADC*64 in the limit for num then it varies the change in brightness but
		// doesn't actually do the potentiometer task perfectly 
	PORTD=PORTD ^ (0xFF);
}

 ISR(INT0_vect)
 // interrupt enabled from the button which decreases the brightness from high to low until 
 // the limit is reached and then re-dos the thing from high to low in a forever loop
{
	int num=1000;
	int on=0;
	
	if(on){
		OCR1A = 62500; //0xFF
		on=0;
	}
	else {
		OCR1A = num;
		on=1;
	}
	num=num-100;

	if(num<=10) {num=55000000;}
	PORTD=PORTD ^ (0xFF);
} 

// to toggle in between the brightness from low to high or high to low, pressing the button changes
// the direction forever until the direction is changed again by pressing the button