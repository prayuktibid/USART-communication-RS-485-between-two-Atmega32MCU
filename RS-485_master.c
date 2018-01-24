#include <inttypes.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#define MAXTTEpin 2
#define SET_MAXTE(a,b) ( (a) |=  (1 << (b) ) )
#define CLR_MAXTE(a,b) ( (a) &= ~(1 << (b) ) )
#define Led 1
#define Button 1 
/*Initilize RS485 data Transmission*/ 
extern volatile char flag =0;
void RS485init(void)
	{
	DDRD|= (1<<MAXTTEpin); 											//make max transmitter enabled pin OUTPUT
	CLR_MAXTE(PORTD,MAXTTEpin);			 							//Clear Transmitter enabled pin
	UCSRB|=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE);	                        //Enabled transmitter and Receiver and Interrupt on RXC
	UCSRC|=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0);                        //SET 8 Character, one stop bit, No parity
	UBRRL=0x33;						        						//Set baud 9600
	sei();															//Set Interrupt
	} 
/*IRS485 data Transmitter*/
void RS485Transmitt(char x)
	{
	  SET_MAXTE(PORTD,MAXTTEpin);									//Set Transmitter enabled pin
      _delay_ms(1); 				        						
	 while(!(UCSRA&(1<<UDRE)));										//Check whether Transmitter data buffer is empty or not
	 UDR=x;                                          				//Receive data on UDR register
	 while(!(UCSRA&(1<<TXC)));										//Check whether Transmission complete or not
	 UCSRA|=(1<<TXC);                                				//Make TXC HIGH
	 CLR_MAXTE(PORTD,MAXTTEpin);									//Clear Transmitter enabled pin
	 _delay_ms(1);                                      
	}

/*IRS485 data Receive Inturrept Vector*/
ISR(USART_RXC_vect)
	{
     cli();															 //Clear Interruppt 
	 PORTA = UDR; 													 //send data to port A from UDR
     flag = 1;                                                       //Set Flag
     sei();                                                          //Set Interrupt 
    //_delay_ms(1);                                                 
	}
/*Main Function*/
int main()
	{ 
    char data=0;
	DDRA|=(1<<Led);                                                  //Make Led as Output
    DDRB&=~(1<<Button);   							                 //Make Button as Input
	RS485init();                                                     //Initialize Trans receiver 
	while(1)
		{
        if(flag)
		  {
		   data=PINB;                                              	//Take data from Pin B
		   RS485Transmitt(data);									//Transmitt Data
		   _delay_ms(1);                       
        }                                        
	 }
  }	
