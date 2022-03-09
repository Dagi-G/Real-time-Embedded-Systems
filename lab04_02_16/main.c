#include "common.h"

// Comment this to remove "heartbeat" and led flash in PCINT
//#define DEBUG 1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "leds.h"
#include "buttons.h"

#define LRed 1
#define LYellow 2
#define LGreen 4

void blink_yellow();
void callBack();
void led_out_on();
void led_out_on2();
void led_out_on3();
void Timer0();
void change_speed();
volatile uint8_t Y_state = 1;


/****************************************************************************
   ALL INITIALIZATION
****************************************************************************/

void initialize_system(void)
{
  initialize_led(GREEN);
  initialize_led(YELLOW);
  DDRD |= (1 << LGreen);
  //DDRD |= (1 << LYellow);
  DDRD |= (1 << LRed);
   
  // initalize only buttonA and buttonC because they are connected to PCINT
  // NOTE: button C and the RED led are on the same line.
  initialize_button(&_buttonA);
  initialize_button(&_buttonC);
  enable_pcint(&_interruptC);
  setup_button_action(&_interruptC, 1, change_speed);
  DDRD |= (1 << 5);
  DDRB |= (1 << 5);
  Timer0();
}
// blinks green out led.
void led_out_on()
{
  DDRD |= (1 << LGreen);
  PORTD |= (1 << LGreen);
}
// blinks yellow led
void led_out_on2()
{
  DDRD |= (1 << LYellow);
  PORTD |= (1 << LYellow);
}
// turns on red led
void led_out_on3()
{
  DDRD |= (1 << LRed);
  PORTD |= (1 << LRed);
}

void Timer0(void) 
{

 ICR1 = 6500;         // top value
 OCR1A = ICR1 / 2;   // match
 TCCR1B |= (1 << WGM13) | (1 << WGM12);
 TCCR1A |= (1 << WGM11);
 TCCR1B |= (1 << CS12) | (1 << CS10);  // prescaler 1024
 TCCR1A |= (1 << COM1A1);
 //TIMSK1 |= (1 << OCIE1A);
}


void blink_yellow()
{
  TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
  _delay_ms(300);
  TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
  _delay_ms(300);
}

void change_speed(){
  uint8_t max = 3000;
  uint8_t min = 500;
  if(Y_state){// Blinking the led faster(decreasing the ISR)  
    if(min > ICR1 - 500){ // if there is space to go up.
      ICR1 = min;
      Y_state = 0; // turn of the light.
    }
    else{  // if there is no space to go up.
        ICR1 = ICR1 - 500;
    }
  }
  else{           //Blinking the led slower(increasing the ISR)
        if(max < ICR1 + 500){
          ICR1 = max;
          Y_state = 1;     // yellow led is on
        }
        else{
          ICR1 += 500;
        }
  }
  OCR1A =ICR1 / 2;
}

/****************************************************************************
   MAIN
****************************************************************************/

int main(void)
{
  // This prevents the need to reset after flashing
  USBCON = 0;

  initialize_system();

  sei();
  while (1)
  {
    blink_yellow();  // make sure board works. 
    }
  
} /* end main() */
