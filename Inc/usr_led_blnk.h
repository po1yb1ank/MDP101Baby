

#ifndef USR_LED_BLNK_H
#define USR_LED_BLNK_H
#include <stdint.h>



//heart beat
/*--------------------------------------------------------------------*/
//
#define HEART_PWM_PERIOD 						1000

//
#define HEART_PWM_MAX_VAL						960
#define HEART_PWM_MIN_VAL						0

//incremented value after each pwm period
#define HEART_BEAT_STEP							40

//
#define HEART_RISE_1_PHASE						1
#define HEART_SHORT_SILENCE_PHASE				2
#define HEART_RISE_2_PHASE						3
#define HEART_LONG_SILENCE_PHASE				4

//
#define HEART_SHORT_SILENT_TIME 				((uint16_t)7)
#define HEART_LONG_SILENT_TIME 					((uint16_t)100)
/*--------------------------------------------------------------------*/







void 		pwm_long_silent_set_val			(uint16_t val);//
void 		inc_pwm_long_silent_time 		(uint16_t val);
void 		dec_pwm_long_silent_time 		(uint16_t val);
uint16_t 	get_pwm_long_silent_time 		(void);




void 		pwm_short_silent_set_val		(uint16_t val);//
void		inc_pwm_short_silemt_time 		(uint16_t val);
void 		dec_pwm_short_silemt_time 		(uint16_t val);
uint16_t 	get_pwm_short_silent_time 		(void);



void		pwm_step_set_val				(uint16_t val);//
void 		inc_pwm_step 					(uint16_t val);
void 		dec_pwm_step 					(uint16_t val);
uint16_t 	get_pwm_step 					(void);




void 		pwm_calculate_difference 		(void);

#endif /*USR_LED_BLNK_H*/