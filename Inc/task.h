



#ifndef _TASK_H
#define _TASK_H







/*
	Get batary charging every 'PERIOD_FOR_MEASUREMENT_BAT_CHARGE' milisecond
*/
void 		batary_measurement 				(void);
void 		batary_is_discharged 			(void);

/**
	blink led for START device
*/
void 		led_blink_when_device_starts	(void);


/**
	When power pin = '0' - we disable device with help BAT_ON_OFF_PIN
*/
void 		power_supply_detect				(void);


/**
	When usb cabble is inserted - enable led and set flag, that usb is connected
*/
void 		usb_connect_detect				(void);
void 		charger_connect_detect			(void);


/**
*/
void 		motion_button_detect 			(void);

/**
	Initialization all tasks in the MIDI BABY system
*/
void 		system_tasks_init				(void);


#endif /*_TASK_H*/


