#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>

#include "uart.h"

#define F_CPU 16000000UL
#define BAUD_RATE 9600
#define BAUD_PRESCALER (((F_CPU / (BAUD_RATE * 16UL))) - 1)

volatile int edge1 = 0;
volatile int edge2 = 0;
volatile int period = 0;
volatile int flag = 0;
volatile int judge[5];

char String[25];
char String2[25];

volatile int Morse[5];
volatile int read = 0;


int input_table[36][5] = {{0,1,2,2,2},{1,0,0,0,2},{1,0,1,0,2},{1,0,0,2,2},{0,2,2,2,2},{0,0,1,0,2},{1,1,0,2,2},{0,0,0,0,2},{0,0,2,2,2},{0,1,1,1,2},{1,0,1,2,2},{0,1,0,0,2},{1,1,2,2,2},
	{1,0,2,2,2},{1,1,1,2,2},{0,1,1,0,2},{1,1,0,1,2},{0,1,0,2,2},{0,0,0,2,2},{1,2,2,2,2},{0,0,1,2,2},{0,0,0,1,2},{0,1,1,2,2},{1,0,0,1,2},{1,0,1,1,2},{1,1,0,0,2},{0,1,1,1,1},{0,0,1,1,1},
	{0,0,0,1,1},{0,0,0,0,1},{0,0,0,0,0},{1,0,0,0,0},{1,1,0,0,0},{1,1,1,0,0},{1,1,1,1,0},{1,1,1,1,1}};
char *output_table[]={"A","B","C","D","E","F","G","H","I","J","K","L","M","N","O","P","Q","R","S","T","U","V","W","X","Y","Z","1","2","3","4","5","6","7","8","9","0"};



void Initialize()
{
	//disable global interrupts
	cli();
	
	//GPIO pins setup
	DDRB &= ~(1<<DDB0);
	DDRB |= (1<<DDB4);
	DDRB |= (1<<DDB3);
	DDRB |= (1<<DDB2);
	DDRB |= (1<<DDB1);
	PORTB &=~(1<<PORTB4);
	PORTB &=~(1<<PORTB3);
	PORTB &=~(1<<PORTB2);
	PORTB &=~(1<<PORTB1);
	
	//timer1 setup, clkI/O/1024 (from prescaler)
	TCCR1B |= (1<<CS00);
	TCCR1B &= ~(1<<CS01);
	TCCR1B |= (1<<CS02);
	
	TCCR1A &= ~(1<<WGM10);
	TCCR1A &= ~(1<<WGM11);
	TCCR1B &= ~(1<<WGM12);
	TCCR1B &= ~(1<<WGM13);
	

	// looking for rising edge
	TCCR1B |= (1<<ICES1);	
	
	//enable interrupts
	TIMSK1 |= (1<<ICIE1);

	//clearing the input capture flag
	TIFR1 |= (1<<ICF1);

/*
	 //looking for falling edge
	TCCR1B &= ~(1<<ICES1);	
	//clearing the input capture flag
	TIFR1 |= (1<<ICF1);	
	//enable interrupts
	TIMSK1 |= (1<<ICIE1);
*/
	//enable global interrupts
	sei();

}



ISR(TIMER1_CAPT_vect)
{
	flag++;

	TCCR1B ^= (1<<ICES1); //looking for an opposite edge
	
	if (flag==1)
	{
		edge1 = ICR1;
	}
	
	if (flag==2)
	{
		edge2 = ICR1;
		period = (edge2 - edge1);
		
			if ((period>=468)&&(period<3125))
			{
				//sprintf(String,"dot \n");
				//UART_putstring(String);
				PORTB ^= (1<<PORTB2); // light red led on pb2
				_delay_ms(50);
				PORTB ^= (1<<PORTB2);
				Morse[read] = 0;
				read++;

			}
			if ((period>=3125)&&(period<=6250))
			{
				//sprintf(String,"dash \n");
				//UART_putstring(String);
				PORTB ^= (1<<PORTB1); //light green led on pb1
				_delay_ms(50);
				PORTB ^= (1<<PORTB1);
				Morse[read] = 1;
				read++;
				
			}

				
		edge1 = 0;
		edge2 = 0;
		flag = 0;
		period = 0;
		
	}
	while ((flag==0)&&((TCNT1-edge2)>6250))
	{
		sprintf(String," ");
		UART_putstring(String);
		for(int a=0; a<5; a++)
		{
			Morse[a] = 2;
			judge[a] = 0;
		}
		read = 0;
		_delay_ms(2000);
		
	}

//	sprintf(String2,"period is %u \n",period);
//	UART_putstring(String2);


}



int main(void)
{
	UART_init(BAUD_PRESCALER); 
    Initialize();
	sprintf(String2,"ready \n");
	UART_putstring(String2);
	
	
    while (1)
    {
		
		for(int i=0;i<36;i++)
		{
			for(int j=0;j<5;j++)
			{
				if(Morse[j] == input_table[i][j])
				{
					judge[j]=1;
					while(judge[0]&&judge[1]&&judge[2]&&judge[3]&&judge[4])
					{
						printf(output_table[i]);
						UART_putstring(output_table[i]);
						_delay_ms(1000);
					}
				}
			}
		}	

		
			
    }
}
