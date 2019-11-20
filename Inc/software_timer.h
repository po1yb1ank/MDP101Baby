

#ifndef _SOFT_TIMER
#define _SOFT_TIMER


#include <stdint.h>




#define MAX_COUNT_SW_TIM		(uint8_t)(21)







/*Possible states of timers
*/
typedef enum
{
	//timer has been worked
	INACTIVE,

	//timer works
	ACTIVE,

	//all tasks was stoped
	STOP_STATE

}timer_state;





/*Operations of mode for timers
*/
typedef enum
{
	//not used
	IDLE,

	//Worked once and turns off  @number_of_repetitions@ - number of times
	ONE_PULSE,

	//Works until the stop function(or delete) is called. PERIOD = @cycle_period_ms@
	CYCLE,

	//enable after @time_to_on@ and execute timer-function ONE times
	WAIT_TIME_AND_ON,

	//temprory stop all task
	STOP

}timer_mode;








/*Main structure responsible for one timer
*/
typedef struct
{
	uint8_t 	number;						//№ current soft timer

	timer_state state;						//current status
	timer_mode 	mode;						//operation mode

	uint32_t 	current_tick;				//save current time after start
	uint32_t 	cycle_period_ms;			//period calling task - function
											//important in CYCLE mode

	uint32_t	current_repeat;				//important in ONE_PULSE mode
	uint32_t	repeat_num;					//important in ONE_PULSE mode

	uint32_t 	time_to_on_ms;				//important in WAIT_ON mode
	uint32_t 	time_to_off_ms;				//important in WAIT_OFF mode

	void 		(*func_process)(void);		//task - function
											//It will be call in timer handler with
											//@cycle_period_ms@ period
}timer_t;










/*****************************************************
				FUNCTION_DECLARATION
******************************************************/

/*
	Returns the timer number
*/
uint8_t 		create_process			(timer_t* task);//++

void 			delete_process	 		(timer_t* task);//++

void 			sync_all_processes		(void);//++



/*
	Assigning the STOP status
	If mode = STOP, then nothing task not operate, but 
	systick interrupts are enable
*/
void 			stop_all_processes		(void);//++
void 			stop_process 			(timer_t* task);//++

/*
	Reasrote old status
*/
void 			restore_all_processes	(void);//++
void 			restore_process 		(timer_t* task);


/*
	Returns value from @timer_state@ enum
*/
timer_state		get_status_process		(timer_t* task);//+

/*
	Returns value from @timer_mode@ enum
*/
timer_mode		get_mode_process		(timer_t* task);//+




/*	
	Function is called in systick interrupts or in main function, but
	then in it-handler function you need set FLAG to '1' and reset it
	in main function
*/
void			process_handler			(void);//+



#endif /*_SOFT_TIMER*/





