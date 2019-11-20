
#ifndef _KEYBOARD_H
#define _KEYBOARD_H


#include <stdint.h>
#include "stm32f1xx_hal.h"


/*Total number of buttons in the device*/
#define COMMON_COUNT_BUTTONS		2

/*For array of button values*/
#define COUNT_LINE					1
#define COUNT_COLUMN				2


/*For fight with contact bounce*/
#define KEY_SENSE					(uint8_t)(7)


/*temprory value*/
#define SET							GPIO_PIN_RESET
#define RESET 						GPIO_PIN_SET



typedef enum
{
  	BUTTON_RELEASED,
	BUTTON_PRESSED
}button_state;






/**********************************************************/
typedef struct
{
	/*input data WITHOUT filter
		0 - NOT pressed
		1 - pressed
	*/
	GPIO_PinState 	current_status[COUNT_LINE][COUNT_COLUMN];

	/*for filter that fight with bounce contact*/
	uint8_t			count[COUNT_LINE][COUNT_COLUMN];

	/*real button data*/
	uint8_t 		button_data[COUNT_LINE][COUNT_COLUMN];

	/*Array for old and new data for buttons/keyboard*/
	uint8_t 		old_button_data[COUNT_LINE][COUNT_COLUMN];

	/*button state: pressed/ not pressed*/
	button_state	state[COUNT_LINE][COUNT_COLUMN];

	/*OLD button state: pressed/ not pressed*/
	button_state	old_state[COUNT_LINE][COUNT_COLUMN];

	/*for future use: calculate press time*/
	uint32_t 		pressed_time[COUNT_LINE][COUNT_COLUMN];

}input_button_data_t;
/**********************************************************/





/*
	Main function for user.
	This function is called in timer interrupt
*/
void 			scan_keyboard				(void);


/*This function sequentially reads the buttons
*/
GPIO_PinState 	sequence_reading			(void);


/*Fiter for detect wrong value
And this function defenition bitton state: BUTTON_RELEASED or BUTTON_PRESSED
*/
void 			bounce_contact_filter		(uint8_t x, uint8_t y);


/*Check ANY changed in keyboard
*/
void			check_change_state			(void);



/*function for use in user program:
reads state button in x,y address
*/
button_state	get_button_state			(uint8_t x, uint8_t y);


/*Function for use in user program:
reads old button state
*/
button_state	get_old_button_state		(uint8_t x, uint8_t y);


#endif /*_KEYBOARD_H*/





















