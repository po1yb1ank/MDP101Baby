



#include "software_timer.h"
#include "midi_baby.h"



/*Create some object, which describes timers.
Array index is the number of the task

MAX_COUNT_SW_TIM - maximum number of tasks
*/
static timer_t 		soft_process[MAX_COUNT_SW_TIM] = {0};


/*Arrays for save current mode and state, when used @stop_all_processes@ function
*/
static timer_mode 	temp_mode[MAX_COUNT_SW_TIM] = {IDLE};
static timer_state 	temp_state[MAX_COUNT_SW_TIM] = {INACTIVE};











/************************************************/
uint8_t create_process (timer_t* task)
{
	uint8_t i = 0;

	/*find empty timer*/
	for(i = 0; i <= MAX_COUNT_SW_TIM; ++i)
	{
		if(soft_process[i].state == INACTIVE)
		{
			/*fill in timer structure*/
			soft_process[i].number 					= i;
			soft_process[i].mode 					= task->mode;
			soft_process[i].current_tick 			= 0;
			soft_process[i].cycle_period_ms 		= task->cycle_period_ms;
			soft_process[i].current_repeat			= task->current_repeat;
			soft_process[i].repeat_num			 	= task->repeat_num;
			soft_process[i].time_to_on_ms			= task->time_to_on_ms;
			soft_process[i].time_to_off_ms 			= task->time_to_off_ms;
			soft_process[i].func_process			= task->func_process;
			soft_process[i].state 					= ACTIVE;
			break;
		}
	}
	return i;
}
/************************************************/






/************************************************/
void delete_process (timer_t* task)
{
	soft_process[task->number].state = INACTIVE;
	soft_process[task->number].mode = IDLE;
	return;
}
/************************************************/








/************************************************/
timer_state get_status_process (timer_t* task)
{
	return soft_process[task->number].state;
}
/************************************************/







/************************************************/
timer_mode get_mode_process (timer_t* task)
{
	return soft_process[task->number].mode;
}
/************************************************/










/************************************************/
void process_handler (void)
{
	uint8_t i = 0;

	/*find empty timer*/
	for(i = 0; i <= MAX_COUNT_SW_TIM; ++i)
	{
		if(soft_process[i].state == ACTIVE)
		{
			//increase current time tick for chosen timer
			++soft_process[i].current_tick;

			switch (soft_process[i].mode)
			{
				/**********/
				case ONE_PULSE:
				{
					if(soft_process[i].current_tick >= soft_process[i].cycle_period_ms)
					{
						soft_process[i].current_tick = 0;

						/*calling process with help pointer*/
						(*soft_process[i].func_process)();

						++soft_process[i].current_repeat;

						if(soft_process[i].current_repeat >= soft_process[i].repeat_num)
						{
							soft_process[i].current_tick = 0;
							delete_process(&soft_process[i]);
						}
					}
					break;
				}
				/**********/

				/**********/
				case CYCLE:
				{
					if(soft_process[i].current_tick >= soft_process[i].cycle_period_ms)
					{
					  	soft_process[i].current_tick = 0;

						/*calling process with help pointer*/
						(*soft_process[i].func_process)();
					}
					break;
				}
				/**********/

				/**********/
				case WAIT_TIME_AND_ON:
				{
					if(soft_process[i].current_tick >= soft_process[i].time_to_on_ms)
					{
						soft_process[i].current_tick = 0;

						/*calling process with help pointer*/
						(*soft_process[i].func_process)();

						delete_process(&soft_process[i]);
					}
					break;
				}
				/**********/
			}
		}
	}
	return;
}
/************************************************/








/************************************************/
void stop_all_processes (void)
{
	uint8_t i = 0;

	/*save current mode for all task*/
	for(i = 0; i <= MAX_COUNT_SW_TIM; ++i)
	{
		temp_mode[i] = soft_process[i].mode;
		temp_state[i] = soft_process[i].state;

		soft_process[i].mode = STOP;
		soft_process[i].state = STOP_STATE;
	}
	return;
}
/************************************************/









/************************************************/
void stop_process (timer_t* task)
{
	if(task->state == ACTIVE)
	{
		temp_mode[task->number] = soft_process[task->number].mode;
		temp_state[task->number] = soft_process[task->number].state;

		soft_process[task->number].mode = STOP;
		soft_process[task->number].state = STOP_STATE;
	}
	return;
}
/************************************************/










/************************************************/
void restore_all_processes (void)
{
	uint8_t i = 0;

	/*restore current mode for all task*/
	for(i = 0; i <= MAX_COUNT_SW_TIM; ++i)
	{
		if(soft_process[i].state == STOP_STATE)
		{
			soft_process[i].mode = temp_mode[i];
			soft_process[i].state = temp_state[i];
		}
	}
	return;
}
/************************************************/










/************************************************/
void restore_process (timer_t* task)
{
	if(task->state == STOP_STATE)
	{
		soft_process[task->number].mode = temp_mode[task->number];
		soft_process[task->number].state = temp_state[task->number];
	}
	return;
}
/************************************************/










/************************************************/
void sync_all_processes (void)
{
	uint8_t i = 0;

	stop_all_processes();

	for(i = 0; i <= MAX_COUNT_SW_TIM; ++i)
	{
		soft_process[i].current_tick 	= 0;
		soft_process[i].current_repeat 	= 0;
	}

	restore_all_processes();
	return;
}
/************************************************/




