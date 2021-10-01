#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
    /* Set PB1,2,3,4 to be output pin */
	DDRB |= (1<<DDB1);
	DDRB |= (1<<DDB2);
	DDRB |= (1<<DDB3);
	DDRB |= (1<<DDB4);
	DDRD &= ~(1<<DDD7); // PD7 setup as input pin
	
	//PORTB |= (1<<PORTB1);
	int count = 1;
	
    while (1) 
    {
		if ((PIND & (1<<DDD7)) & (count == 1))
		{
			PORTB |= (1<<PORTB1);
			PORTB &= ~(1<<PORTB2);
			PORTB &= ~(1<<PORTB3);
			PORTB &= ~(1<<PORTB4);
			count=2;
			_delay_ms(500);
		}
		if ((PIND & (1<<DDD7)) & (count == 2))
		{
			PORTB &= ~(1<<PORTB1); 
			PORTB |= (1<<PORTB2);
			PORTB &= ~(1<<PORTB3);
			PORTB &= ~(1<<PORTB4);
			count=3;
			_delay_ms(500);
		}
		if ((PIND & (1<<DDD7)) & (count == 3))
		{
			PORTB &= ~(1<<PORTB1);
			PORTB &= ~(1<<PORTB2);
			PORTB |= (1<<PORTB3);
			PORTB &= ~(1<<PORTB4);
			count=4;
			_delay_ms(500);
		}
		if ((PIND & (1<<DDD7)) & (count == 4))
		{
			PORTB &= ~(1<<PORTB1);
			PORTB &= ~(1<<PORTB2);
			PORTB &= ~(1<<PORTB3);
			PORTB |= (1<<PORTB4);
			count=1;
			_delay_ms(500);
		}
		
		
    }
}
