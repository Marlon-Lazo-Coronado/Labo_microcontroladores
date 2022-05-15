#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

int state = 0;
int request_botton = 0;

/* we lisening the interrup in D0 */
ISR (PCINT2_vect){  
  request_botton = 1 ;
}
ISR (PCINT1_vect){
  request_botton = 1 ;
}

void turnon_leds(int p_v, int time) { //p_v = peatones o veiculos, time = unidad en 1/2 s 
  
  if (p_v == 1){ //peatones
    for (int i = 0; i < 2*time; i++) {
      PORTB |= ( 1 <<PB1)|( 1 <<PB5);
        _delay_ms(8000);
        i++;
    }
    PORTB &= ~( 1 <<PB1) & ~( 1 <<PB5);
  }
  else {
    for (int i = 0; i < 2*time; i++) {
      PORTB |= ( 1 <<PB3);
      _delay_ms(8000);
      i++;
    }    
    PORTB &= ~( 1 <<PB3);
  }
}

void turnon_leds_aux(int p_v, int stop) { //p_v = peatones o veiculos, time = unidad en 1/2 s 
  
  if (stop == 1){
    if (p_v == 1) //peatones
        PORTB |= ( 1 <<PB0)|( 1 <<PB4);
    else 
        PORTB |= ( 1 <<PB2);
  }
  else {
    if (p_v == 1) //peatones
        PORTB |= ( 1 <<PB1)|( 1 <<PB5);
    else 
        PORTB |= ( 1 <<PB3);
  }
}

void turnoff_leds_aux(int p_v, int stop) { //p_v = peatones o veiculos, time = unidad en 1/2 s 
  
  if (stop == 1){
    if (p_v == 1) //peatones
        PORTB &= ~( 1 <<PB0) & ~( 1 <<PB4);
    else 
        PORTB &= ~( 1 <<PB2);
  }
  else {
    if (p_v == 1) //peatones
        PORTB &= ~( 1 <<PB1) & ( 1 <<PB5);
    else 
        PORTB &= ~( 1 <<PB3);
  }
}

/* we lisening the timer interruption */
ISR (TIMER0_OVF_vect)
{
  switch (state){

    case 0:
        PORTB |= ( 1 <<PB0)|( 1 <<PB4)|( 1 <<PB3)|( 1 <<PB6);
        PORTB &= ~( 1 <<PB1)&~( 1 <<PB5)&~( 1 <<PB2);      
      if(request_botton == 1){
        request_botton = 0;
        _delay_ms(8000);
        turnon_leds(0, 1); _delay_ms(8000); turnon_leds(0, 1); _delay_ms(8000); turnon_leds(0, 1);
        PORTB &= ~( 1 <<PB6);
        state = 1;
      }
      
      break;

    case 1: // botton presed
      //encendemos LDPD y pasamos en tiempo 0
      turnon_leds_aux(1,1);
      // LDPV encendido por 10 segundos
      turnon_leds(0, 20);
      //Alternamos 3 segundos
      _delay_ms(8000);turnon_leds(0, 1); _delay_ms(8000); turnon_leds(0, 1); _delay_ms(8000); turnon_leds(0, 1);
      // encendemos LDVD
      turnon_leds_aux(0,1);
      //un segundo
      _delay_ms(8000); _delay_ms(8000);

      state = 2;
      break;

    case 2:  // veiculos pasan
      // apagamos LDVD en tiempo 0
      turnoff_leds_aux(1,1);
      // LDPP encendido por 10 segundos
      turnon_leds(1, 20);
      //Alternamos 3 segundos
      _delay_ms(8000); turnon_leds(1, 1); _delay_ms(8000); turnon_leds(1, 1); _delay_ms(8000); turnon_leds(1, 1); turnon_leds_aux(1,1);
      //un segundo
      _delay_ms(8000); _delay_ms(8000);

      state = 0;
      break;
  }
  request_botton = 0;
}

void  settings (){
  /* we set the outputs */
  DDRB |= ( 1 <<PB0) | ( 1 <<PB1) | ( 1 <<PB2) | ( 1 <<PB3) | ( 1 <<PB4)| ( 1 <<PB5) | ( 1 <<PB6);

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