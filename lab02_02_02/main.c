#include "common.h"

#include <avr/io.h>
#include <util/delay.h>

#include "leds.h"
#include "buttons.h"

#define INVERTED 1

/****************************************************************************
   ALL INITIALIZATION
****************************************************************************/
void initialize_system(void) {
	// initalize green and yellow only.
	// initialization defines the IO_structs and sets DDR
	initialize_led(GREEN);
	initialize_led(YELLOW);

	// The "sanity check".
	// When you see this pattern of lights you know the board has reset
	light_show();

	// initalize only buttonA and buttonC because they are connected to PCINT
	// NOTE: button C and the RED led are on the same line.
    initialize_button(BUTTONA);
	initialize_button(BUTTONC);
}

/****************************************************************************
   MAIN
****************************************************************************/

int main(void) {
  // This prevents the need to reset after flashing
  USBCON = 0;

	initialize_system();
//	initialize_button(BUTTONA);

  //*************************************************************//
  //*******         THE CYCLIC CONTROL LOOP            **********//
  //*************************************************************//

  // FILL THIS IN TO MEET FUNCTIONAL REQUIREMENTS:

	// Always toggle yellow LED every 1000ms
	// Toggle greed LED every 500ms, but only if is in an on state
	// Press and release button A switches green to on state.
	// Press and release button C switches green to off state.

	// Assume both buttons start in a not pressed state.

	uint8_t green_on = 1;
	uint8_t Areleased = 0;
	uint8_t Creleased = 0;
	uint8_t Cprevious = is_button_pressed(&_buttonC);
	uint8_t Aprevious = is_button_pressed(&_buttonA);
	uint8_t count = 2;
	uint8_t toggle_on =0;
while(1) {

	_delay_ms(500);
	if (toggle_on) {
		led_toggle(&_green);
	}

	count--;
	if(count == 0){
		led_toggle(&_yellow);
		count = 2;
	}

	if(!is_button_pressed(&_buttonA)){    // if the has not been pressed.
		if(Aprevious != 0){	  // previous state was pressed.	
			Aprevious = 0;       // set the previous state to not pressed
			Areleased = 1;		
		}
	} else {
		if(Aprevious ==0){    // previous state was not pressed
			Aprevious = 1;
			Areleased = 0;
		}
	}
	
	// if button A was released.
	if(Areleased){
		//led_on(&_green,0);
		toggle_on = 1;
		Areleased = 0;
	}
	
	/**************** BUTTON C	*****************/
	if(!is_button_pressed(&_buttonC)){    // if the has not been pressed.
		if(Cprevious != 0){	  // previous state was pressed.	
			Cprevious = 0;       // set the previous state to not pressed
			Creleased = 1;		
		}
	} else {
		if(Cprevious ==0){    // previous state was not pressed
			Cprevious = 1;
			Creleased = 0;
		}
	}
		// if button A was released.
	if(Creleased){
		led_off(&_green,0);
		toggle_on = 0;
		Creleased = 0;
	}
	} // end while(1)

} /* end main() */
