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

void blink_yellow();
void yellow_on();
void stateCheck();
void stateCheck2();
int state = 0;
int stateC = 0;

/****************************************************************************
   ALL INITIALIZATION
****************************************************************************/
void initialize_system(void)
{
  // initalize green and yellow only.
	// initialization defines the IO_structs and sets DDR
	initialize_led(GREEN);
	initialize_led(YELLOW);

	// The "sanity check".
	// When you see this pattern of lights you know the board has reset
	light_show();

	// initalize only buttonA and buttonC because they are connected to PCINT
	// NOTE: button C and the RED led are on the same line.
  initialize_button(&_buttonA);
	initialize_button(&_buttonC);

  //DDRB &= ~(1<<BUTTONC);
  //PORTB |= (1<<BUTTONC);

  //CLEAR_BIT(*(&_buttonC)->ddr, _buttonC.pin);
  // enable pull-up resistor
  //SET_BIT(*(&_buttonC)->port, _buttonC.pin);

  setup_button_action(&_interruptA, 1, stateCheck);
  setup_button_action(&_interruptC, 1, stateCheck2);
  
  enable_pcint(&_interruptC);
  enable_pcint(&_interruptA);

}

void blink_yellow() {
  TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
  _delay_ms(300);
  TOGGLE_BIT(*(&_yellow)->port, _yellow.pin);
}


void yellow_on(){
  led_on(&_yellow,1);
}
// state checker for button A.
void stateCheck(){
  state++;
}

// state checker for button B.
void stateCheck2(){
  stateC++;
}
int toggleOnYellow = 0;
int toggleOnGreen = 0;

/****************************************************************************
   MAIN
****************************************************************************/

int main(void) {
  // This prevents the need to reset after flashing
  USBCON = 0;

	initialize_system();


  //*************************************************************//
  //*******         THE CYCLIC CONTROL LOOP            **********//
  //*************************************************************//

  // FILL THIS IN TO BLINK LEDS at the requested frequency given the
  // current state of the system (i.e. if a button was pressed or released)
  // Even if you know how to set up a timer interrupt, do not use it
  // for this assignment. You may use _delay_ms()

  // without keyword volatile, the compiler optimizes away count
  volatile uint32_t count = 5;
  
  sei();
  while(1) {
  // Yellow led is in a blink state.
  if(toggleOnYellow & !toggleOnGreen){
    _delay_ms(1250);
    led_toggle(&_yellow);
  }
  // Green led is in a blink state.
  if(toggleOnGreen & !toggleOnYellow){
    _delay_ms(250);
    led_toggle(&_green);
  }
  //both leds are in a blink state.
  if(toggleOnYellow && toggleOnGreen){
    _delay_ms(250);
    led_toggle(&_green);
    count--;
    if(count == 0){
      led_toggle(&_yellow);
      count = 5;
    }
  }
  /**** BUTTON A   ****/
  if(state == 0){
    led_off(&_yellow,1);
  }
  if(state == 1){
    led_on(&_yellow,1);
  }
  if(state == 2){
    toggleOnYellow = 1;
  }
  if(state > 2){
    state = 0;
    toggleOnYellow = 0; 
  }

  /**BUTTON C **/
  if(stateC == 0){
    led_off(&_green,0);
  }
  if(stateC == 1){
    led_on(&_green,0);
  }
  if(stateC == 2){
    toggleOnGreen =1;
  }
  if(stateC > 2){
    stateC = 0; 
    toggleOnGreen = 0;
  }



  } /* end while(1) loop */
} /* end main() */
