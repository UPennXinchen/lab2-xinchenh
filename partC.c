#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 16000000UL

void Initialize()
{
	//disable global interrupts
	cli();
	
	//GPIO pins setup
	DDRB |= (1<<DDB5);
	DDRB &= ~(1<<DDB0);
	PORTB &=~(1<<PORTB5);
	
	//
	TCCR1B &= ~(1<<CS10);
	TCCR1B |= (1<<CS11);
	TCCR1B &= ~(1<<CS12);
	
	// 
	TCCR1A &= ~(1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	
/*
	// looking for rising edge
	TCCR1B |= (1<<ICES1);	
	//clearing the input capture flag
	TIFR1 |= (1<<ICF1);	
	//enable interrupts
	TIMSK1 |= (1<<ICIE1);
*/
	// looking for falling edge
	TCCR1B &= ~(1<<ICES1);	
	//clearing the input capture flag
	TIFR1 |= (1<<ICF1);	
	//enable interrupts
	TIMSK1 |= (1<<ICIE1);
	
	//enable global interrupts
	sei();

}

ISR(TIMER1_CAPT_vect)
{
	PORTB ^= (1<<PORTB5); //toggle led
	TCCR1B ^= (1<<ICES1); //looking for an opposite edge

}



int main(void)
{
    Initialize();
    while (1)
    {
		
    }
}

