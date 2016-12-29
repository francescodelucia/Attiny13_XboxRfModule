/*
	Xbox 360 Rf Module

*/


#define F_CPU 1200000 // clock 1.2MHz, internal oscillator

#include <util/delay.h>
#include <avr/io.h> 
#include <avr/sleep.h>
#include <stdint.h> 
#include <avr/interrupt.h>

#define readPin(x,y) x & _BV(y)
#define BIT_L(port,pin) port &= ~(1<<pin)
#define BIT_H(port,pin) port |= (1<<pin)

#define OUTPUT 	1
#define INPUT 	0
#define HI		1
#define LO		0
#define LOW		0
#define HIGH	1

typedef struct 
{ 
  unsigned int bit0:1; 
  unsigned int bit1:1; 
  unsigned int bit2:1; 
  unsigned int bit3:1; 
  unsigned int bit4:1; 
  unsigned int bit5:1; 
  unsigned int bit6:1; 
  unsigned int bit7:1; 
} _io_reg; 

#define REGISTER_BIT(rg,bt) ((volatile _io_reg*)&rg)->bit##bt 
#define DATA 	PB3
#define CLOCK 	PB4
#define SYNC 	PB0
//#define DEBUG 	PB1

#define DATA_PIN  	REGISTER_BIT(PINB,3) 
#define CLOCK_PIN   REGISTER_BIT(PINB,4) 
#define SYNC_PIN    REGISTER_BIT(PINB,0) 
//#define DEBUG_PIN 	REGISTER_BIT(PINB,1)


#define DATA_PORT  	REGISTER_BIT(PORTB,3) 
#define CLOCK_PORT  REGISTER_BIT(PORTB,4) 
#define SYNC_PORT   REGISTER_BIT(PORTB,0) 
//#define DEBUG_PORT 	REGISTER_BIT(PORTB,1)

#define DATA_DIR  	REGISTER_BIT(DDRB,3) 
#define CLOCK_DIR 	REGISTER_BIT(DDRB,4)
#define SYNC_DIR 	REGISTER_BIT(DDRB,0) 
//#define DEBUG_DIR 	REGISTER_BIT(DDRB,1)


char led_cmd[10] =  {0,0,1,0,0,0,0,1,0,0}; //Activates/initialises the LEDs, leaving the center LED lit.
char anim_cmd[10] = {0,0,1,0,0,0,0,1,0,1}; //Makes the startup animation on the ring of light.
char sync_cmd[10] = {0,0,0,0,0,0,0,1,0,0}; //Initiates the sync process.

volatile int sync_enable = 0;

ISR (PCINT0_vect) // Interrupt on Int0 vector
 {
 	sync_enable = 1;	
 }
 
void sendData(char cmd_do[]) {	
		
	int prev = 1;
	int i=0;
	DATA_DIR = OUTPUT;			
	DATA_PORT = LOW;				
	for(; i < 11; i++)
	{    		
		while (prev == CLOCK_PIN){} //detects change in clock		
		prev = CLOCK_PIN;		
		DATA_PORT = cmd_do[i];					
		while (prev == CLOCK_PIN){} //detects upward edge of clock			
		prev = CLOCK_PIN;
	}
	DATA_PORT = HI;
	DATA_DIR = INPUT;	
}

int main(void){	
		
	DATA_DIR = INPUT;
	CLOCK_DIR = INPUT;			
			
	PCMSK |= (1<<PCINT0);   // pin change mask: listen to portb bit 2 
	GIMSK |= (1<<PCIE);   // enable PCINT interrupt 
	MCUCR |= (1<<ISC00);
	sei();         // enable all interrupts 
	
	_delay_ms(100);
	sendData(anim_cmd);	
	
	while(1)
	{				
		if(sync_enable==1) {  		
		sendData(sync_cmd);
		_delay_ms(1000);		
		sync_enable = 0;   
		}		
	}	
}



