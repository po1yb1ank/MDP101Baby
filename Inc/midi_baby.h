



#ifndef _MIDI_BABY_H
#define _MIDI_BABY_H

#include <stdint.h>
#include "midi.h"


/*For @is_queue_empty@ function*/
#define NO  -1//return  if FAIL
#define YES 0//return if OK




/*--------------------------------------------------------------------*/
#define INTERNAL_OSC							0
#define EXTERNAL_OSC							1
/*--------------------------------------------------------------------*/


//
#define  PERIOD_FOR_POLL_CALC_DIFFERENT			((uint32_t)1000) //1 секунд





//If one of this defines are commented - corresponding function isn't supported
/*--------------------------------------------------------------------*/
//    If this define is 0 - ble midi not supported
#define BLE_MIDI_SUPPORT            			1

//	  If this define is 0 - usb not supported
#define USB_MIDI_SUPPORT            			1

//    MIDI OUT and MIDI IN port. It is UART.
//    This define need for transmit data throuth uart too.
#define UART_MIDI_IN_OUT_SUPPORT    			0
/*--------------------------------------------------------------------*/




//Defines for work with power button
/*--------------------------------------------------------------------*/
//time for pressing power button that device was enabled or was disabled (in milisecond)
#define ENABLE_DISABLE_TIME						250

//for detection power supply button. It means power button pin will poll every 'PERIOD_FOR_POLL_POWER_BUTTON_MS' ms
#define PERIOD_FOR_POLL_POWER_BUTTON_MS			200
/*--------------------------------------------------------------------*/



//Defines for work with motion button
/*--------------------------------------------------------------------*/
#define PERIOD_FOR_DETECT_MOTION_BUTTON			67

//with this period stm32 polling motion sensor
#define PEROID_FOR_GET_MOTION_DATA				17

//
#define GAIN_FILTER								((float)0.3)
/*--------------------------------------------------------------------*/



//Defines for work with keyboard
//with this period stm32 called 'scan_keyboard' function
/*--------------------------------------------------------------------*/
#define PERIOD_KEYBOARD_SCAN					5//200 Hz
/*--------------------------------------------------------------------*/



//Defines for work with ADC
/*--------------------------------------------------------------------*/
//with this period stm32 called 'midi_baby_start_adc_conversion' function
#define PERIOD_ADC_CALL	 						2//500 Hz
/*--------------------------------------------------------------------*/







//CC MIDI BABY DEVICE DEFAULT SETUP
/*--------------------------------------------------------------------*/
#define DEFAULT_CHANNEL_ROTARY_0				CHANNEL_1
#define DEFAULT_CC_ID_ROTARY_0					CC_UNDEFINED_1

#define DEFAULT_CHANNEL_ROTARY_1				CHANNEL_1
#define DEFAULT_CC_ID_ROTARY_1					CC_UNDEFINED_2

#define DEFAULT_CHANNEL_LEFT_BUTTON				CHANNEL_1
#define DEFAULT_CC_ID_LEFT_BUTTON				CC_UNDEFINED_3

#define DEFAULT_CHANNEL_RIGHT_BUTTON			CHANNEL_1
#define DEFAULT_CC_ID_RIGHT_BUTTON				CC_UNDEFINED_4

#define DEFAULT_CHANNEL_REW						CHANNEL_1
#define DEFAULT_CC_ID_REW 						CC_UNDEFINED_6
#define DEFAULT_CC_VAL_REW 						((uint8_t)0x7F)

#define DEFAULT_CHANNEL_STOP					CHANNEL_1
#define DEFAULT_CC_ID_STOP						CC_UNDEFINED_8
#define DEFAULT_CC_VAL_STOP						((uint8_t)0x7F)

#define DEFAULT_CHANNEL_PLAY					CHANNEL_1
#define DEFAULT_CC_ID_PLAY						CC_UNDEFINED_9
#define DEFAULT_CC_VAL_PLAY						((uint8_t)0x7F)

#define DEFAULT_CHANNEL_REC						CHANNEL_1
#define DEFAULT_CC_ID_REC						CC_UNDEFINED_10
#define DEFAULT_CC_VAL_REC						((uint8_t)0x7F)

#define DEFAULT_CHANNEL_LOOP					CHANNEL_1
#define DEFAULT_CC_ID_LOOP						CC_UNDEFINED_5
#define DEFAULT_CC_VAL_LOOP						((uint8_t)0x7F)

#define DEFAULT_CHANNEL_GYRO_X					CHANNEL_1
#define DEFAULT_CC_ID_GYRO_X					CC_MODULATION_WHEEL
#define DEFAULT_CC_VAL_GYRO_X					((uint8_t)0)

#define DEFAULT_CHANNEL_GYRO_Y					CHANNEL_1
#define DEFAULT_CC_ID_GYRO_Y					CC_BREATH
#define DEFAULT_CC_VAL_GYRO_Y					((uint8_t)0)

#define DEFAULT_CHANNEL_GYRO_Z					CHANNEL_1
#define DEFAULT_CC_ID_GYRO_Z					CC_PAN
#define DEFAULT_CC_VAL_GYRO_Z					((uint8_t)0)

#define DEFAULT_CHANNEL_ACCEL_X					CHANNEL_1
#define DEFAULT_CC_ID_ACCEL_X					CC_UNDEFINED_11
#define DEFAULT_CC_VAL_ACCEL_X					((uint8_t)0)

#define DEFAULT_CHANNEL_ACCEL_Y					CHANNEL_1
#define DEFAULT_CC_ID_ACCEL_Y					CC_UNDEFINED_12
#define DEFAULT_CC_VAL_ACCEL_Y					((uint8_t)0)

#define DEFAULT_CHANNEL_ACCEL_Z					CHANNEL_1
#define DEFAULT_CC_ID_ACCEL_Z					CC_UNDEFINED_13
#define DEFAULT_CC_VAL_ACCEL_Z					((uint8_t)0)
/*--------------------------------------------------------------------*/






//ADC1 defines for rotary and buttons
/*--------------------------------------------------------------------*/
/*Принцип считывание данных с 3х каналов АЦП:
	- заводится програмный таймер на запуск АЦП по DMA в одноразовом режиме @HAL_ADC_Start_DMA@
	(ContinuousConvMode = DISABLE,ScanConvMode = ENABLE,
	разрешены прерывания от DMA. В обработчике прерывания проверяется прерывание ли это от половины
	заполнения буфера или от полного заполнения)

	- DMA настроено в нормальном (не циклическом) режиме, потому что програмый таймер сам его
	стартует

	- прерывание от DMA генерится по окончанию преобразования.
	В нём выставлятся флаг, что данные готовы. В main данные обрабатываются
	и заново запускается преобразование
*/

#define ADC1_COUNT_CHANNEL						(uint8_t)(3)

#define COUNT_TRANSPORT_BUTTONS					(uint8_t)(5)

/*Averaging factor for ADC data*/
#define ADC_AVERAGE_FACTOR						(uint16_t)(11)

/*показание ацп в крайнем положении при 3.3 вольтах*/
#define ROTATRY_MAX_VALUE						(uint16_t)(4035)

/*adc value when transport button is pressed*/
#define REWIND_ADC_VALUE						(uint8_t)(17)
#define STOP_ADC_VALUE							(uint8_t)(80)
#define PLAY_ADC_VALUE							(uint8_t)(0)
#define REC_ADC_VALUE							(uint8_t)(32)
#define LOOP_ADC_VALUE							(uint8_t)(49)

/*погрешность*/
#define ADC_ERROR_OFFSET						(uint8_t)(5)

/*количество одинаковых значений АЦП, после которого происходит установка флага*/
#define ADC_SENSITIVITY							(uint8_t)(7)
/*--------------------------------------------------------------------*/




//Defines for measurement batary charging
/*--------------------------------------------------------------------*/
//in mV - CHANGE ONLY IT define if your schematic will change
#define MAX_BATARY_VOLTAGE 						((uint32_t)2360)//2,5 V

//in mV - CHANGE ONLY IT define if your schematic will change
#define MIN_BATARY_VOLTAGE 						((uint32_t)1938)//2,0 V

//in milisecond (default - 2,5 minutes)
#define PERIOD_FOR_MEASUREMENT_BAT_CHARGE	 	((uint32_t)150000)

//in mV
#define ADDITIONAL_VOLTAGE_WHEN_USB_INSERTED	((uint32_t)166)

//
#define PERIOD_TOGGLE_BAT_LED_WHEN_DISCHARGE 	150

//
#define PERIOD_FOR_DETECT_CHARGER_CABLE			207

//when midi_baby_device.battery_charge <= 'PERSENT_WHEN_DEVICE_DISCHARGED'
//midi baby starts toggle bat led task for indicate that it is needed for charger
#define PERSENT_WHEN_DEVICE_DISCHARGED 			10
/*--------------------------------------------------------------------*/









/*--------------------------------------------------------------------*/
typedef union
{
	uint16_t all;

	struct
	{
		unsigned char start_delay_elapsed			:1;
		unsigned char change_rotor_0_value			:1;
		unsigned char change_rotor_1_value			:1;

		/*flags for enable midi event*/
		unsigned char play							:1;
		unsigned char stop							:1;
		unsigned char rew							:1;
		unsigned char rec   						:1;
		unsigned char loop 							:1;

	}bit;

}midi_flags_t;
/*--------------------------------------------------------------------*/






/*--------------------------------------------------------------------*/
typedef union
{
	uint32_t all;

	struct
	{
		/*--------------------------------------------------------------------*/
		unsigned char systick_it_happend			:1;
		unsigned char adc_dma_data_ready			:1;
		unsigned char adc_charge_measure_ready		:1;
		unsigned char motion_data_ready 			:1;//set when dma_rx it is occured
		unsigned char motion_button_pressed 		:1;//set when motion button is enabled
		unsigned char charger_insert 				:1;
		unsigned char batary_discharged				:1;
		unsigned char batary_charging_finished		:1;
		unsigned char enable_x_axis 					:1;
		unsigned char enable_y_axis					:1;
		unsigned char enable_z_axis					:1;
		/*--------------------------------------------------------------------*/

		/*--------------------------------------------------------------------*/
		//connect state
		unsigned char usb_connect					:1;
		unsigned char ble_connect					:1;
		/*--------------------------------------------------------------------*/

		/*--------------------------------------------------------------------*/
		//led state
		unsigned char usb_led_active				:1;
		unsigned char charge_led_active   			:1;
		unsigned char ble_led_active   				:1;
		/*--------------------------------------------------------------------*/

		/*--------------------------------------------------------------------*/
		// helpfull secondary flags
		// turn on detection state transport button. This flag == 1, when old transport ADC data != new data
		unsigned char enable_detect_transport   	:1;

		// when device is enabled first time - wait for adc align your data with voltage value on real rotary
		unsigned char allow_transmiting 			:1;

		// Time for poll motion sensor is elapsed
		unsigned char time_for_poll_motion_elapsed	:1;

		//
		unsigned char read_motion_started			:1;

		//
		unsigned char first_it_from_batary_adc 		:1;

		//
		unsigned char motion_but_just_pressed 		:1;

		//
		unsigned char first_batary_measurement 		:1;

		//
		unsigned char reset_ble						:1;
		/*--------------------------------------------------------------------*/
	}bit;

}common_flags_t;
/*--------------------------------------------------------------------*/







/*
	Structure contain information about cuttern midi_baby state and info*******************
	for flash memory(for configurator)
*/
typedef struct
{
	//left rotary knob
	uint8_t cc_rotary_0_ch;			//channel
	uint8_t cc_rotary_0_id;			//cc id
	uint8_t cc_rotary_0_val;		//cc value (adc measurement)

	//right rotary knob
	uint8_t cc_rotary_1_ch;			//channel
	uint8_t cc_rotary_1_id;			//cc id
	uint8_t cc_rotary_1_val;		//cc value (adc measurement)

	/*--------------------------------------*/
	//user buttons
	uint8_t cc_left_usr_button_ch;		//channel
	uint8_t cc_left_usr_button_id;		//cc id
	uint8_t cc_left_usr_button_val; 	//cc value

	uint8_t cc_right_usr_button_ch;		//channel
	uint8_t cc_right_usr_button_id;		//cc id
	uint8_t cc_right_usr_button_val;	//cc value
	/*--------------------------------------*/

	/*--------------------------------------*/
	//transport buttons
	uint8_t cc_rew_ch;
	uint8_t cc_rew_id;
	uint8_t cc_rew_val;

	uint8_t cc_stop_ch;
	uint8_t cc_stop_id;
	uint8_t cc_stop_val;

	uint8_t cc_play_ch;
	uint8_t cc_play_id;
	uint8_t cc_play_val;

	uint8_t cc_rec_ch;
	uint8_t cc_rec_id;
	uint8_t cc_rec_val;

	uint8_t cc_loop_ch;
	uint8_t cc_loop_id;
	uint8_t cc_loop_val;
	/*--------------------------------------*/

	/*--------------------------------------*/
	//axis X gyro
	uint8_t cc_gyro_x_ch;
	uint8_t cc_gyro_x_id;			//cc id
	uint16_t cc_gyro_x_val;			//cc value

	//axis Y gyro
	uint8_t cc_gyro_y_ch;
	uint8_t cc_gyro_y_id;			//cc id
	uint16_t cc_gyro_y_val;			//cc value

	//axis Z gyro
	uint8_t cc_gyro_z_ch;
	uint8_t cc_gyro_z_id;			//cc id
	uint16_t cc_gyro_z_val;			//cc value
	/*--------------------------------------*/

	/*--------------------------------------*/
	uint8_t cc_acceleromtr_x_ch;
	uint8_t cc_acceleromtr_x_id;	//cc id
	uint8_t cc_acceleromtr_x_val;	//cc value

	uint8_t cc_acceleromtr_y_ch;
	uint8_t cc_acceleromtr_y_id;	//cc id
	uint16_t cc_acceleromtr_y_val;	//cc value

	uint8_t cc_acceleromtr_z_ch;
	uint8_t cc_acceleromtr_z_id;	//cc id
	uint16_t cc_acceleromtr_z_val;	//cc value
	/*--------------------------------------*/

	/*--------------------------------------*/
	//Temperature from MPU-6050
	uint8_t mpu_temprature;
	/*--------------------------------------*/

	/*--------------------------------------*/
	//midi baby charging in persent (example: 83 %)
	uint8_t battary_charge;
	/*--------------------------------------*/

	/*--------------------------------------*/
	uint32_t midi_events_counts;
	/*--------------------------------------*/

}midi_device_t;
/*--------------------------------------------------------------------*/


/*
	This union must be filled in if new buttons appear
*/
/*--------------------------------------------------------------------*/
typedef union
{
	uint8_t all;

	struct
	{
		unsigned char buttons_change_state :1;
		unsigned char pressed_button_0:1;
		unsigned char pressed_button_1:1;

		//for future use:
		unsigned char pressed_button_2:1;
		unsigned char pressed_button_3:1;
		unsigned char pressed_button_4:1;
		unsigned char pressed_button_5:1;
		unsigned char pressed_button_6:1;
	}bit;

}keyboard_flags_t;
/*--------------------------------------------------------------------*/








/*
    Init default midi channel and structure for flash memory.
    Also this function init peripheral for midi support:
    BLE_MIDI_SUPPORT or/and USB_MIDI_SUPPORT or/and UART_MIDI_IN_OUT_SUPPORT
*/
void 		midi_baby_init						(void);

/*
	Send any midi event from usb and/or ble. And transmit to
	MIDI OUT port (uart transmitting)
*/
void 		midi_baby_transmit_event			(midi_event_packet_t* event);//++




/*==============================INTERFACE WITH CURCUIT================================*/
/*====================================================================================*/
/**
*/
void 		power_supply_disable 				(void);
void 		power_supply_enable 				(void);

/**
*/
void 		heart_led_on 						(void);
void 		heart_led_off 						(void);
void 		heart_led_toggle 					(void);

/**
*/
void 		bat_led_on 							(void);
void 		bat_led_off 						(void);
void 		bat_led_toggle 						(void);

/**
*/
void 		motion_led_on 						(void);
void 		motion_led_off 						(void);
void 		motion_led_toggle 					(void);


/**
*/
void 		ble_chip_reset 						(void);
void 		ble_chip_reset_pin_to_low 			(void);
void 		ble_chip_reset_pin_to_high 			(void);

/**
*/
void 		usb_pin_enable 						(void);
void 		usb_pin_disable 					(void);


/**
*/
void 		bat_sens_enable 					(void);
void 		bat_sens_disable 					(void);



/*==============================MIDI BABT HAS FELT ANY CHANGE=========================*/
/*====================================================================================*/
/*
*/
void 		midi_baby_rotary_0_changed			(void);
void 		midi_baby_rotary_1_changed			(void);

void 		midi_baby_stop_but_changed 			(void);
void 		midi_baby_play_but_changed 			(void);
void 		midi_baby_rewind_but_changed		(void);
void		midi_baby_rec_but_changed 			(void);
void 		midi_baby_loop_but_changed			(void);

void 		midi_baby_accel_x_changed 			(void);
void 		midi_baby_accel_y_changed 			(void);
void 		midi_baby_accel_z_changed 			(void);
void 		midi_baby_gyro_x_changed 			(void);
void 		midi_baby_gyro_y_changed 			(void);
void 		midi_baby_gyro_z_changed 			(void);


/*==============================Analyze new gyro/accelerometer data===================*/
/*====================================================================================*/
/**
	'destination_buf' will be filled data from MPU-6050
*/
void 		midi_baby_start_read_motion 		(void);
void 		midi_baby_analyze_new_motion_data	(void);




/*==============================Analyze new ADC data==================================*/
/*====================================================================================*/
/*
*/
void 		midi_baby_start_adc_conversion		(void);
void 		midi_baby_stop_adc_conversion 		(void);
void 		midi_baby_analyze_new_adc_data 		(void);



/*==============================Analyze Batary data===================================*/
/*====================================================================================*/

void 		midi_baby_analyze_new_batary_data  	(void);




#endif /*_MIDI_BABY_H*/
