#include <avr/io.h>

int main(void)
{
    //set PB0 to be input
	DDRB &= ~(1<<DDB0);
	//set PB5 to be output
	DDRB |= (1<<DDB5);
	while(1)
	{
		
	
		if (PINB & (1<<PINB0))
		{
			//printf("OFF")
			PORTB &= ~(1<<PORTB5);
		}
		else
		{
			//printf("ON")
			PORTB |= (1<<DDB5);
		}
	
	}
}
