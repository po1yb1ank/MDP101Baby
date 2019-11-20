


#include "keyboard.h"
#include "stm32f1xx_hal.h"
#include "midi_baby.h"


/*keyboard flags*/
keyboard_flags_t			keyboard_flags = {0};




/*struct:
info about pressed button
info about press time (for future use)*/
input_button_data_t   input_button_data;


/*
This function must be filled in if new buttons appear
*/
/**************************************************/
GPIO_PinState sequence_reading (void)
{
	GPIO_PinState pinstate = GPIO_PIN_RESET;

	static uint8_t count_buttons = 1;

	if(count_buttons == 1)
	{
		pinstate = HAL_GPIO_ReadPin(CC_BTN0_GPIO_Port, CC_BTN0_Pin);
		count_buttons = 2;
	}

	else if (count_buttons == 2)
	{
		pinstate = HAL_GPIO_ReadPin(CC_BTN1_GPIO_Port, CC_BTN1_Pin);
		count_buttons = 1;
	}
	return pinstate;
}
/**************************************************/




/**************************************************/
void scan_keyboard (void)
{
	uint8_t i = 0;
	uint8_t j = 0;

	for(i = 0; i < COUNT_LINE; ++i)
	{
		for(j = 0; j < COUNT_COLUMN; ++j)
		{
			input_button_data.current_status[i][j] = sequence_reading();
			bounce_contact_filter(i, j);
		}
	}
	check_change_state();

	return;
}
/**************************************************/






/**************************************************/
void bounce_contact_filter (uint8_t x, uint8_t y)
{
	if(input_button_data.current_status[x][y] == SET)
	{
		if(input_button_data.count[x][y]  < KEY_SENSE)
		{
			++input_button_data.count[x][y];
		}
		else
		{
			input_button_data.button_data[x][y] = 1;
			input_button_data.state[x][y] = BUTTON_PRESSED;
		}
	}
	else
	{
		if(input_button_data.count[x][y]  > 0)
		{
			--input_button_data.count[x][y];
		}
		else
		{
			input_button_data.button_data[x][y] = 0;
			input_button_data.state[x][y] = BUTTON_RELEASED;
		}

	}

	return;
}
/**************************************************/







/**************************************************/
button_state get_button_state (uint8_t x, uint8_t y)
{
	return input_button_data.state[x][y];
}
/***************************************************/



/**************************************************/
button_state get_old_button_state (uint8_t x, uint8_t y)
{
	return input_button_data.old_state[x][y];
}
/********************************7******************/






/**************************************************/
void check_change_state (void)
{
	uint8_t i = 0;
	uint8_t j = 0;

	for(i = 0; i < COUNT_LINE; ++i)
	{
		for(j = 0; j < COUNT_COLUMN; ++j)
		{
			if(input_button_data.old_button_data[i][j] != input_button_data.button_data[i][j])
			{
				keyboard_flags.bit.buttons_change_state = 1;
				input_button_data.old_button_data[i][j] = input_button_data.button_data[i][j];
			}
		}
	}
	return;
}
/**************************************************/






