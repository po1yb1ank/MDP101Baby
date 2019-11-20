
#include "usr_led_blnk.h"
#include "midi_baby.h"
#include <stdlib.h>


//
extern midi_device_t            		midi_baby_device;
extern common_flags_t					common_flags;


//
static uint16_t pwm_step = HEART_BEAT_STEP;
static uint16_t long_silent_time = HEART_LONG_SILENT_TIME;
static uint16_t short_silent_time = HEART_SHORT_SILENT_TIME;


/*-------------------------------------*/
void inc_pwm_long_silent_time (uint16_t val)
{
	long_silent_time += val;
}

void dec_pwm_long_silent_time (uint16_t val)
{
	if (long_silent_time <= val)
	{
		long_silent_time = HEART_SHORT_SILENT_TIME;
		return;
	}

	long_silent_time -= val;
}

void pwm_long_silent_set_val (uint16_t val)
{
	long_silent_time = val;
}

uint16_t get_pwm_long_silent_time (void)
{
	return long_silent_time;
}
/*-------------------------------------*/



/*-------------------------------------*/
void inc_pwm_short_silemt_time (uint16_t val)
{
	short_silent_time += val;
}

void dec_pwm_short_silemt_time (uint16_t val)
{
	short_silent_time -= val;
}

void pwm_short_silent_set_val (uint16_t val)
{
	short_silent_time = val;
}

uint16_t get_pwm_short_silent_time (void)
{
	return short_silent_time;
}
/*-------------------------------------*/




/*-------------------------------------*/
void pwm_step_set_val (uint16_t val)
{
	pwm_step = val;
}

void inc_pwm_step (uint16_t val)
{
	pwm_step += val;
}

void dec_pwm_step (uint16_t val)
{
	pwm_step -= val;
}

uint16_t get_pwm_step (void)
{
	return pwm_step;
}
/*-------------------------------------*/







/*-------------------------------------*/
void pwm_calculate_difference (void)
{
	//static uint32_t old_midi_events = 0;
	static uint32_t cur_time = 0;

	cur_time += PERIOD_FOR_POLL_CALC_DIFFERENT;

	//
	if (midi_baby_device.midi_events_counts > 7)
	{
		//old_midi_events = midi_baby_device.midi_events_counts;
		midi_baby_device.midi_events_counts = 0;

		//gain PWM
		if ((get_pwm_long_silent_time()) >= 11)
		{
			pwm_step_set_val(HEART_BEAT_STEP);
			pwm_long_silent_set_val(get_pwm_long_silent_time() - 10);
		}
		else
		{
			pwm_step_set_val(HEART_BEAT_STEP + 10);
		  	pwm_long_silent_set_val(5);
		}
	}
	else if (midi_baby_device.midi_events_counts <= 7)
	{
		//old_midi_events = midi_baby_device.midi_events_counts;
		midi_baby_device.midi_events_counts = 0;

		//gain PWM
		if ((get_pwm_long_silent_time()) <= (HEART_LONG_SILENT_TIME + 50))
		{
			pwm_step_set_val(HEART_BEAT_STEP);
			pwm_long_silent_set_val(get_pwm_long_silent_time() + 10);
		}
		else
		{
			pwm_step_set_val(HEART_BEAT_STEP - 10);
		}
	}

	//
	if (cur_time >= 3600000)
	{
		cur_time = 0;
		power_supply_disable();
	}

	return;
}
/*-------------------------------------*/












