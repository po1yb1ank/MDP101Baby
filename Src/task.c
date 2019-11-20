#include "main.h"
#include "midi_baby.h"
#include "stm32f1xx_hal.h"
#include "software_timer.h"
#include "mpu6050_ll_driver.h"
#include "keyboard.h"
#include "usr_led_blnk.h"

extern ADC_HandleTypeDef		hadc2;
extern TIM_HandleTypeDef 		htim4;
extern common_flags_t			common_flags;

//task heandle for 'batary_is_discharged' function
extern uint8_t batary_discharge_process_heandle;

/*Start reading keyboard.
period_time_ms - time for calling function @scan_keyboard@
in timer interrupts
*/
extern void scan_keyboard (void);


extern void midi_ble_timestamp_inc (void);


extern void pwm_calculate_difference (void);






/*-------------------------------------------------------------------------*/
void power_supply_detect (void)
{
  	static uint8_t power_button_press_time = 0;//1 uinit = PERIOD_FOR_POLL_POWER_BUTTON_MS

	if ((HAL_GPIO_ReadPin(POWER_BUTTON_GPIO_Port, POWER_BUTTON_Pin)) == GPIO_PIN_RESET)
	{
	  	++power_button_press_time;

		if ((power_button_press_time == 3) && (common_flags.bit.allow_transmiting == 1))
		{
			power_supply_disable();
			power_button_press_time = 0;
		}
	}
	else
		power_button_press_time = 0;
}
/*-------------------------------------------------------------------------*/





/*-------------------------------------------------------------------------*/
void motion_button_detect (void)
{
	static uint8_t motion_button_press_time = 0;//1 uinit = PERIOD_FOR_DETECT_MOTION_BUTTON

	if ((HAL_GPIO_ReadPin(MOTION_KEY_GPIO_Port, MOTION_KEY_Pin)) == GPIO_PIN_RESET)
	{
		static uint8_t led_state = 0;

		if (motion_button_press_time == 1)
		{
			if ((led_state == 0) && (common_flags.bit.motion_but_just_pressed == 0))
			{
				motion_led_on();
				led_state = 1;

				//set flag that button was pressed
				common_flags.bit.motion_button_pressed = 1;
			}
			else if ((led_state == 1) && (common_flags.bit.motion_but_just_pressed == 0))
			{
				motion_led_off();
				led_state = 0;

				//set flag that button was pressed
				common_flags.bit.motion_button_pressed = 0;
			}

			//set flag that motion button jush was pressed
			common_flags.bit.motion_but_just_pressed = 1;

			//reset press time
			motion_button_press_time = 0;
		}

		++motion_button_press_time;
	}
	else
	{
		common_flags.bit.motion_but_just_pressed = 0;
	  	motion_button_press_time = 0;
	}
}
/*-------------------------------------------------------------------------*/






/*-------------------------------------------------------------------------*/
void usb_connect_detect (void)
{
  	static uint8_t usb_insert_time = 0;

	if ((HAL_GPIO_ReadPin(USB_DETECT_GPIO_Port, USB_DETECT_Pin)) == GPIO_PIN_RESET)
	{
	  	++usb_insert_time;

		if (usb_insert_time == 5)
		{
			common_flags.bit.usb_connect = 1;
			usb_insert_time = 0;
		}
	}
	else
	{
	  	usb_insert_time = 0;
		common_flags.bit.usb_connect = 0;
	}
	return;
}
/*-------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*/
void batary_is_discharged (void)
{
	bat_led_toggle();
	return;
}
/*---------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------*/
void charger_connect_detect (void)
{
	//var for detecion inserting cable throuth some limit time
  	//time during device is already charged!
  	static uint32_t charger_insert_time = 0;

  	//if inseret
	if ((HAL_GPIO_ReadPin(CHARGER_DETECT_GPIO_Port, CHARGER_DETECT_Pin)) == GPIO_PIN_RESET)
	{
	  	++charger_insert_time;// equivalent 'PERIOD_FOR_DETECT_CHARGER_CABLE' ms

		if (charger_insert_time == 2)
		{
			//delete the toggle batary led process
			//if process is already exist!!!!!!!!
			//удаляем процесс мограния светодиодом о том, что нужна зарядка, потому что вставлен зарядник
			if (common_flags.bit.batary_discharged == 1)
			{
				timer_t temp_process = {0};
				temp_process.number = batary_discharge_process_heandle;
				delete_process(&temp_process);
			}

			//set flag that charger cable is inserted
			common_flags.bit.charger_insert = 1;
		}
		bat_led_on();//indicate that charging is ongoing
	}
	else
	{
		//если сейчас девайсу НЕ нужна зарядка
		//если НЕ создавали процесс toggle говорящий о том, что нужна зарядка
		//когда батарейка заряжена, то common_flags.bit.batary_discharged == 0;
		if (common_flags.bit.batary_discharged == 0)
			bat_led_off();

		// if less then 5 minute and if process will deleted
		if ((charger_insert_time < 1500) && (common_flags.bit.batary_discharged == 1) && (charger_insert_time != 0))
		{
		    //create task for toggle batary led
            timer_t temp_process = {0};

			temp_process.mode                   = CYCLE;
			temp_process.cycle_period_ms        = PERIOD_TOGGLE_BAT_LED_WHEN_DISCHARGE;
			temp_process.func_process           = batary_is_discharged;
			batary_discharge_process_heandle    = create_process(&temp_process);
		}
		//if charging time more then 5 miunes
		//и мы создавали процесс, говорящий о том, что нужна зарядка!
		else if ((charger_insert_time >= 1500) && (common_flags.bit.batary_discharged == 1))
		{
			//batary has enough charging
			common_flags.bit.batary_discharged = 0;
			bat_led_off();
		}

	  	charger_insert_time = 0;
		common_flags.bit.charger_insert = 0;
	}
	return;
}
/*-------------------------------------------------------------------------*/




/*-------------------------------------------------------------------------*/
void is_time_for_read_motion_elapsed (void)
{
	common_flags.bit.time_for_poll_motion_elapsed = 1;
	return;
}
/*-------------------------------------------------------------------------*/






/*-------------------------------------------------------------------------*/
void wait_for_adc_average_first_data (void)
{
	common_flags.bit.allow_transmiting = 1;
}
/*-------------------------------------------------------------------------*/





/*-------------------------------------------------------------------------*/
static void batary_start_conversion (void);

void batary_measurement (void)// called with PERIOD_FOR_MEASUREMENT_BAT_CHARGE period (5 minute)
{
	/*
		A temporary pointer to the structure of the process
		Used to fill a structure @soft_process[i]@
	*/
	timer_t temp_process = {0};

	bat_sens_enable();

	temp_process.mode 					= WAIT_TIME_AND_ON;
	temp_process.time_to_on_ms			= 100;
	temp_process.func_process			= batary_start_conversion;
  	create_process(&temp_process);
}

void batary_start_conversion (void)
{
    HAL_ADC_Start_IT(&hadc2);
    return;
}
/*---------------------------------------------------------------------------*/







/*-------------------------------------------------------------------------*/
void system_tasks_init (void)
{
	/*
		A temporary pointer to the structure of the process
		Used to fill a structure @soft_process[i]@
	*/
	timer_t temp_process = {0};

  	/*Create process for calculate TIMESTAMP byte.
  	  ++timestamp every 1 milisecond. It's need for midi ble protocol for sync.
  	*/
	temp_process.mode 					= CYCLE;
	temp_process.cycle_period_ms		= 1;
	temp_process.func_process			= midi_ble_timestamp_inc;
	create_process(&temp_process);

	/*
		detect enable/disable button pin
	*/
	temp_process.mode 					= CYCLE;
	temp_process.cycle_period_ms		= PERIOD_FOR_POLL_POWER_BUTTON_MS;
	temp_process.func_process			= power_supply_detect;
	create_process(&temp_process);

	/*
		usb connection
	*/
	temp_process.mode 					= CYCLE;
	temp_process.cycle_period_ms		= 59;
	temp_process.func_process			= usb_connect_detect;
	create_process(&temp_process);


	/*
		wait_for_adc_average_first_data
	*/
	temp_process.mode 					= ONE_PULSE;
	temp_process.repeat_num 			= 1;
	temp_process.cycle_period_ms		= 2500;
	temp_process.func_process			= wait_for_adc_average_first_data;
	create_process(&temp_process);


	/*
		is_time_for_read_motion_elapsed
	*/
	temp_process.mode 					= CYCLE;
	temp_process.cycle_period_ms		= PEROID_FOR_GET_MOTION_DATA;
	temp_process.func_process			= is_time_for_read_motion_elapsed;
	create_process(&temp_process);

	/*
		motion_button_detect
	*/
	temp_process.mode 					= CYCLE;
	temp_process.cycle_period_ms		= PERIOD_FOR_DETECT_MOTION_BUTTON;
	temp_process.func_process			= motion_button_detect;
	create_process(&temp_process);

	/*
		create task - keyboard scaninng
	*/
	temp_process.mode 					= CYCLE;
	temp_process.cycle_period_ms 		= PERIOD_KEYBOARD_SCAN;
	temp_process.func_process			= scan_keyboard;
  	create_process(&temp_process);

	/*
		create task - detect batary charging. One times in 5 minutes
	*/
	temp_process.mode 					= CYCLE;
	temp_process.cycle_period_ms 		= PERIOD_FOR_MEASUREMENT_BAT_CHARGE;
	temp_process.func_process			= batary_measurement;
  	create_process(&temp_process);

	/*
		create task - detect inserting charger cable
	*/
	temp_process.mode 					= CYCLE;
	temp_process.cycle_period_ms 		= PERIOD_FOR_DETECT_CHARGER_CABLE;
	temp_process.func_process			= charger_connect_detect;
  	create_process(&temp_process);


  	/*
  	*/
  	temp_process.mode 					= CYCLE;
	temp_process.cycle_period_ms 		= PERIOD_FOR_POLL_CALC_DIFFERENT;
	temp_process.func_process			= pwm_calculate_difference;
  	create_process(&temp_process);

	return;
}
/*-------------------------------------------------------------------------*/


