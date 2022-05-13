#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int request_botton = 0 ;



/* we lisening the interrup in D0 */
ISR (PCINT2_vect){
  request_botton = 1 ;
}

ISR (PCINT1_vect){
  request_botton = 1 ;
}

void turnoff_leds(int p_v, int time) { //p_v = peatones o veiculos, time = unidad en 1/2 s 
                                                // s_p -> stop or pass
  if (p_v == 1){ //peatones

    for (int i = 0; i < time; i++) {
      PORTB |= ( 1 <<PB1)|( 1 <<PB5);
        _delay_ms(50000);
        i++;
    }
    PORTB &= ~( 1 <<PB1) & ~( 1 <<PB5);
  }
  else {
    for (int i = 0; i < time; i++) {
      PORTB |= ( 1 <<PB3);
      _delay_ms(50000);
      i++;
    }    
    PORTB &= ~( 1 <<PB3);
  }
}


int state = 1;
/* we lisening the timer interruption */
ISR (TIMER0_OVF_vect)
{
  /*if (request_botton == 1 )
    PORTB |= ( 1 <<PB0)|( 1 <<PB1)|( 1 <<PB2);*/
  switch (state){
    case 1: // botton presed
      if (request_botton == 1 ) {

        PORTB |= ( 1 <<PB1)|( 1 <<PB5);
        _delay_ms(8000);
        PORTB &= ~( 1 <<PB1)& ~( 1 <<PB5);
        _delay_ms(8000);
        PORTB |= ( 1 <<PB1)|( 1 <<PB5);
        _delay_ms(8000);
        PORTB &= ~( 1 <<PB1)& ~( 1 <<PB5);
        _delay_ms(8000);
        PORTB |= ( 1 <<PB1)|( 1 <<PB5);
        _delay_ms(8000);
        PORTB &= ~( 1 <<PB1)& ~( 1 <<PB5);
        _delay_ms(8000);
        PORTB |= ( 1 <<PB1)|( 1 <<PB5);
        _delay_ms(8000);
        PORTB &= ~( 1 <<PB1)& ~( 1 <<PB5);
        _delay_ms(8000);

        //turnoff_leds(1, 20);_delay_ms(50000);turnoff_leds(1, 1);_delay_ms(50000);turnoff_leds(1, 1);_delay_ms(50000);turnoff_leds(1, 1);

        //state = 2;
      }
      break;
    case 2:

      /*PORTB &= ~(1<<PB0);
      PORTB &= ~(1<<PB1);
      PORTB &= ~(1<<PB2);
      _delay_ms(30000);*/
      state = 1;
      break;
  }
}

void  settings (){
  /* we set the outputs */
  DDRB |= ( 1 <<PB0) | ( 1 <<PB1) | ( 1 <<PB2) | ( 1 <<PB3) | ( 1 <<PB4)| ( 1 <<PB5);

  /* we set the interrup for the D0 input */
  GIMSK |= ( 1 << PCIE2);
  PCMSK2 = 0b00000001 ;
  GIMSK |= ( 1 << PCIE1);
  PCMSK1 = 0b00000001 ;
  
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