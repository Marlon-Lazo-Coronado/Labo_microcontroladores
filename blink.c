#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int request_botton = 0 ;

/* we lisening the interrup in D0 */
ISR (PCINT2_vect){
  request_botton = 1 ;
}

/* we lisening the timer interruption */
ISR (TIMER0_OVF_vect)
{
  if (request_botton == 1 )

      PORTB |= ( 1 <<PB0)|( 1 <<PB1)|( 1 <<PB2);    
}

void  settings (){
  /* we set the outputs */
  DDRB |= ( 1 <<PB0) | ( 1 <<PB1) | ( 1 <<PB2);

  /* we set the interrup for the D0 input */
  GIMSK |= ( 1 << PCIE2);
  PCMSK2 = 0b00000001 ;
  
  /* settings for timer interrup */
  TCCR0A= 0x00 ;   //couters in normal mode
  TCCR0B= 0x00 ;
  TCCR0B |= ( 1 <<CS00)|( 1 <<CS02);   // prescaling in 1024
  sei (); // we call global interrups
  TCNT0= 0 ;  
  TIMSK|=( 1 <<TOIE0); // we set the timer0
}

int main( )
{
  

  /* llamada de funcion */
  settings ();

  while( 1 ) {
  }
}