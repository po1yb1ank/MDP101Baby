


#include "tim.h"
#include "stm32f1xx_hal.h"
#include "usr_led_blnk.h"


TIM_HandleTypeDef htim4;
static void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim);


/* TIM4 init function */
void tim4_pwm_init(void)
{
	TIM_ClockConfigTypeDef 	sClockSourceConfig;
	TIM_OC_InitTypeDef 		sConfigOC;

	__HAL_RCC_TIM4_CLK_ENABLE();
	htim4.Instance = TIM4;

	#if (EXTERNAL_OSC == 1)
	{
		htim4.Init.Prescaler = 720 - 1;//100 KHg >> 1 tick = 10 us
	}
	#else//internal
	{
		htim4.Init.Prescaler = 480 - 1;//100 KHg >> 1 tick = 10 us
	}
	#endif /*EXTERNAL_OSC*/

	htim4.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim4.Init.Period = HEART_PWM_PERIOD; //это значение * на 0,1 = количество реальных милисекунд
	htim4.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	HAL_TIM_PWM_Init(&htim4);

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&htim4, &sClockSourceConfig);

	sConfigOC.OCMode = TIM_OCMODE_PWM1;
	sConfigOC.Pulse = HEART_PWM_MIN_VAL;
	sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
	sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigOC, TIM_CHANNEL_1);
	HAL_TIM_MspPostInit(&htim4);

	HAL_NVIC_SetPriority(TIM4_IRQn, 0, 0);
  	HAL_NVIC_EnableIRQ(TIM4_IRQn);

	HAL_TIM_PWM_Start_IT(&htim4, TIM_CHANNEL_1);
	
}





static void HAL_TIM_MspPostInit(TIM_HandleTypeDef* htim)
{

  GPIO_InitTypeDef GPIO_InitStruct;
  if(htim->Instance==TIM4)
  {
    /**TIM4 GPIO Configuration
    PB6     ------> TIM4_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  }

}