

#include "Init.h"
#include "main.h"
#include "adc.h"
#include "keyboard.h"
#include "queue.h"
#include "usbd_midi.h"
#include "mpu6050_ll_driver.h"
#include "midi_baby.h"
#include "task.h"
#include "software_timer.h"
#include "tim.h"
#include "usr_led_blnk.h"

extern CRC_HandleTypeDef 		hcrc;
extern ADC_HandleTypeDef 		hadc1;
extern ADC_HandleTypeDef		hadc2;
extern TIM_HandleTypeDef        htim4;

/*
	keyboard flags
*/
extern keyboard_flags_t			keyboard_flags;


/*
	keyboard struct:
	info about pressed buttons and
	about press time (for future use)
*/
extern input_button_data_t    	input_button_data;

/*
	Structure contain information about cuttern midi_baby state and info
	for flash memory(for pribore electronics configurator)
*/
midi_device_t            		midi_baby_device = {0};


/*
	Queue of usb midi events
*/
midi_queue_t					midi_event_queue = {0};
/********************************************/



/*
	unions with all flags
*/
midi_flags_t 					midi_flags 		= {0};
common_flags_t					common_flags 	= {0};
/********************************************/


/*
	Pointer to the nearest midi event in queue, that needs to be performed
*/
midi_event_packet_t* 			current_midi_event = {0};
/********************************************/


/*
	This functions check any changes in MIDI BABY system:
	keyboard, rotary knob, transport battons. And push midi cmd to usb and ble queue
*/
void 		midi_flags_heandler				(void);
void 		keyboard_scan 					(void);
/************************************************************************/





int main(void)
{
	HAL_Init();

  	SystemClock_Config();

  	GPIO_init();

	//Disable power supply if press time for enable button less then 'ENABLE_DISABLE_TIME'
  	power_supply_enable();

	//all LEDs off
	bat_led_off();
	heart_led_off();
	motion_led_off();

	//reset cc2541
	ble_chip_reset();

	//enable 1.5kOm to D+ line
	usb_pin_enable();

	//enable detect battary charging
	bat_sens_disable();

	/*
		Init default channel and other parameters (FLASH MEMORY FOR CONFIGURATOR)
	  	Also init usb_midi, uart_midi and ble_midi if it defined in midi_kernel.h file.
	*/
	midi_baby_init();

  	/*create task - ADC conversion	*/
  	adc1_init();
  	midi_baby_start_adc_conversion();

  	/*Motion sensor initialization*/
  	mpu6050_init();

  	/*for measurement battary charging*/
  	adc2_init();
  	HAL_Delay(10);

  	/*start first measurement*/
  	bat_sens_enable();
  	HAL_Delay(10);

  	//it is FIRST batary level measurement
  	common_flags.bit.first_batary_measurement = 1;
  	HAL_ADC_Start_IT(&hadc2);

	/*Init all tasks in the MIDI BABY system*/
  	system_tasks_init();

  	/*hw timer for heart beat led init*/
  	tim4_pwm_init();

	while (1)
  	{
  		/*-----------Transmit MIDI cmd, if it is neccessery---------------*/
		if(is_midi_queue_empty(&midi_event_queue) == NO)
		{
			current_midi_event = get_event_from_midi_queue(&midi_event_queue);

			//this flag is setting when 2 second is elapsed after device start
			if (common_flags.bit.allow_transmiting == 1)
			{
				++midi_baby_device.midi_events_counts;
				midi_baby_transmit_event(current_midi_event);
			}
		}
		/*----------------------------------------------------------------*/

		/*checking and doing task processes-------------------------------*/
		if(common_flags.bit.systick_it_happend == 1)
		{
			process_handler();
			common_flags.bit.systick_it_happend = 0;
		}
		/*----------------------------------------------------------------*/

		/*if new ADC data from DMA is ready
		Программа попадает сюда, когда отработает таймер в ONE_PULSE режиме
		и удалит этот процесс запуска DMA. В прерывании DMA останавливается, а тут стартует заново
		*/
		if(common_flags.bit.adc_dma_data_ready == 1)
		{
			midi_baby_analyze_new_adc_data();
			midi_baby_start_adc_conversion();
			common_flags.bit.adc_dma_data_ready = 0;
		}
		/*----------------------------------------------------------------*/

		//Data from MPU-6050 is ready
		/*----------------------------------------------------------------*/
		//start new recieving data every 'PEROID_FOR_GET_MOTION_DATA' miliseconds
		if ((common_flags.bit.motion_button_pressed == 1) && (common_flags.bit.time_for_poll_motion_elapsed == 1))
		{
			if (common_flags.bit.read_motion_started == 0)
			{
		  		midi_baby_start_read_motion();
				common_flags.bit.read_motion_started = 1;
			}

			if (common_flags.bit.motion_data_ready == 1)
			{
				midi_baby_analyze_new_motion_data();

				common_flags.bit.time_for_poll_motion_elapsed = 0;
				common_flags.bit.motion_data_ready = 0;
				common_flags.bit.read_motion_started = 0;
			}
		}
		/*----------------------------------------------------------------*/

		/*----------------CHECK ANY CHANGED-------------------------------*/
		//check CC - buttons or keyboard and add midi event
		keyboard_scan();

		//check midi event and add to queue
		midi_flags_heandler();
		/*----------------------------------------------------------------*/

		//Calculate real volt from device batery. Max value = 2.4V = 2935 in adc reg
		/*----------------------------------------------------------------*/
		if (common_flags.bit.adc_charge_measure_ready == 1)
		{
			midi_baby_analyze_new_batary_data();

			common_flags.bit.adc_charge_measure_ready = 0;
			common_flags.bit.first_batary_measurement = 0;
		}
		/*----------------------------------------------------------------*/

		if (common_flags.bit.reset_ble == 1)
		{
			ble_chip_reset();
			common_flags.bit.reset_ble = 0;
		}
	}
}






//it for program timers
/*------------------------------------------------------------*/
void HAL_SYSTICK_Callback(void)
{
	/*flag for checking and doing task processes*/
	common_flags.bit.systick_it_happend = 1;
}
/*------------------------------------------------------------*/





//ADC complete conversion
/*------------------------------------------------------------*/
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	if (hadc->Instance == ADC1)
	{
		HAL_ADC_Stop_DMA(&hadc1);

     	/*set flag that data is ready*/
      	common_flags.bit.adc_dma_data_ready = 1;
	}

	if (hadc->Instance == ADC2)
	{
		common_flags.bit.adc_charge_measure_ready = 1;
	}
}
/*------------------------------------------------------------*/




//this IT is called when midi baby had read data bytes from MPU-6050
/*------------------------------------------------------------*/
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef *hi2c)
{
	if (hi2c->Instance == I2C1)
	{
		common_flags.bit.motion_data_ready = 1;
	}
}
/*------------------------------------------------------------*/



/*------------------------------------------------------------*/
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART3)
	{
		midi_ble_parce_data();
	}
}
/*------------------------------------------------------------*/


/*------------------------------------------------------------*/
void HAL_TIM_PWM_PulseFinishedCallback(TIM_HandleTypeDef *htim)
{
  	//duty cycle - скважность
	if (htim->Instance == TIM4)//interrupt is happened when PWM period elapsed
	{
		static uint8_t  led_phase = HEART_RISE_1_PHASE;
		static uint16_t current_duty = HEART_PWM_MIN_VAL;

		switch (led_phase)
		{
			case HEART_RISE_1_PHASE:
			{
				current_duty += get_pwm_step();
				htim4.Instance->CCR1 = current_duty;

				if (current_duty >= HEART_PWM_MAX_VAL)
				{
					led_phase = HEART_SHORT_SILENCE_PHASE;
					htim4.Instance->CCR1 = HEART_PWM_MIN_VAL;
					current_duty = HEART_PWM_MIN_VAL;
				}
				break;
			}

			case HEART_SHORT_SILENCE_PHASE:
			{
				static uint16_t tmp = 0;
				++tmp;

				if (tmp >= get_pwm_short_silent_time())
				{
					tmp = 0;
					led_phase = HEART_RISE_2_PHASE;
				}
				break;
			}

			case HEART_RISE_2_PHASE:
			{
				current_duty += get_pwm_step();
				htim4.Instance->CCR1 = current_duty;

				if (current_duty >= HEART_PWM_MAX_VAL)
				{
					led_phase = HEART_LONG_SILENCE_PHASE;
					htim4.Instance->CCR1 = HEART_PWM_MIN_VAL;
					current_duty = HEART_PWM_MIN_VAL;
				}
				break;
			}

			case HEART_LONG_SILENCE_PHASE:
			{
				static uint16_t tmp = 0;
				++tmp;

				if ( tmp >= get_pwm_long_silent_time())
				{
					tmp = 0;
					led_phase = HEART_RISE_1_PHASE;
				}
				break;
			}
		}
	}
}
/*------------------------------------------------------------*/



/*---------------------------------------------------------------------------------*/
/*								FUNCTION DEFINITION								   */
/*---------------------------------------------------------------------------------*/
void midi_flags_heandler (void)
{
	//=====================================
	if(midi_flags.bit.change_rotor_0_value == 1)
	{
		midi_baby_rotary_0_changed();
	 	midi_flags.bit.change_rotor_0_value = 0;
	}
	//=====================================

	//=====================================
	if(midi_flags.bit.change_rotor_1_value == 1)
	{
		midi_baby_rotary_1_changed();
	 	midi_flags.bit.change_rotor_1_value = 0;
	}
	//=====================================


	/*TRANSPORT BUTTONS*/

	//=====================================
	if(midi_flags.bit.stop == 1)
	{
		midi_baby_stop_but_changed();

		/*reset midi play event flag*/
	 	midi_flags.bit.stop = 0;
	}
	//=====================================

	//=====================================
	if(midi_flags.bit.play == 1)
	{
		midi_baby_play_but_changed();

		/*reset midi play event flag*/
		midi_flags.bit.play = 0;
	}
	//=====================================

	//=====================================
	if(midi_flags.bit.rew == 1)
	{
		midi_baby_rewind_but_changed();

	 	/*reset midi play event flag*/
	 	midi_flags.bit.rew = 0;
	}
	//=====================================

	//=====================================
	if(midi_flags.bit.rec == 1)
	{
		midi_baby_rec_but_changed();

	 	/*reset midi play event flag*/
	 	midi_flags.bit.rec = 0;
	}
	//=====================================

	//=====================================
	if(midi_flags.bit.loop == 1)
	{
		midi_baby_loop_but_changed();

	 	/*reset midi play event flag*/
	 	midi_flags.bit.loop = 0;
	}
	//=====================================

	return;
}
/*---------------------------------------------------------------------------------*/














/*---------------------------------------------------------------------------------*/
void keyboard_scan (void)
{
	/*Принцип работы: если кнопку жмут 1 раз, она шлёт CC 127, если второй раз, то СС 0.
	Сначало проверяется были ли вообще какие-нибудь изменения на клавиатуре. Если да, то
	Сравнивается новое состояние КОНКРЕТНОЙ кнопки со старым. Если они не равно значит с ДАННОЙ
	кнопкой произошли изменения (нажатие или отпускание).

	Если pressed_button_0  == 0, то кнопку "нажали - отпустили" 1 раз. Если == 1, то второй раз.
	*/
	if(keyboard_flags.bit.buttons_change_state == 1)
	{
		//BUTTON_0: - LEFT BUTTON
		//If any changed was happend
		if((get_button_state(0,0)) != (get_old_button_state(0,0)))
		{
			/*define old state. New ctate defined in @bounce_contact_filter@ function*/
			input_button_data.old_state[0][0] = input_button_data.state[0][0];

			if((get_button_state(0, 0) ==  BUTTON_PRESSED) && (keyboard_flags.bit.pressed_button_0 == 0))
			{
				keyboard_flags.bit.pressed_button_0 = 1;
				midi_control_change(midi_baby_device.cc_left_usr_button_ch, midi_baby_device.cc_left_usr_button_id, midi_baby_device.cc_left_usr_button_val);
				midi_baby_device.cc_left_usr_button_val = 0;
			}
			else if ((get_button_state(0, 0) ==  BUTTON_PRESSED) && (keyboard_flags.bit.pressed_button_0 == 1))
			{
				keyboard_flags.bit.pressed_button_0 = 0;
				midi_control_change(midi_baby_device.cc_left_usr_button_ch, midi_baby_device.cc_left_usr_button_id, midi_baby_device.cc_left_usr_button_val);
				midi_baby_device.cc_left_usr_button_val = 0x7F;
			}
		}

		//BUTTON_1: - RIGHT BUTTON
		//If any changed was happend
		if((get_button_state(0,1)) != (get_old_button_state(0,1)))
		{
			/*define old state. New ctate defined in @bounce_contact_filter@ function*/
			input_button_data.old_state[0][1] = input_button_data.state[0][1];

			if((get_button_state(0, 1) ==  BUTTON_PRESSED) && (keyboard_flags.bit.pressed_button_1 == 0))
			{
				keyboard_flags.bit.pressed_button_1 = 1;
				midi_control_change(midi_baby_device.cc_right_usr_button_ch, midi_baby_device.cc_right_usr_button_id, midi_baby_device.cc_right_usr_button_val);
				midi_baby_device.cc_right_usr_button_val = 0;
			}
			else if ((get_button_state(0, 1) ==  BUTTON_PRESSED) && (keyboard_flags.bit.pressed_button_1 == 1))
			{
				keyboard_flags.bit.pressed_button_1 = 0;
				midi_control_change(midi_baby_device.cc_right_usr_button_ch, midi_baby_device.cc_right_usr_button_id, midi_baby_device.cc_right_usr_button_val);
				midi_baby_device.cc_right_usr_button_val = 0x7F;
			}
		}
		/*reset any changed flag*/
		keyboard_flags.bit.buttons_change_state = 0;
	}

  	return;
}
/*---------------------------------------------------------------------------------*/





























