
#include "adc.h"
#include "stm32f1xx_hal.h"


DMA_HandleTypeDef 						hdma_adc1;
ADC_HandleTypeDef 						hadc1;
ADC_HandleTypeDef 						hadc2;











/**
	Init DMA channel for ADC and itself ADC module
*/
/*-------------------------------------------------------------------------*/
void adc1_init(void)
{
	/*DMA Init */
	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

    hdma_adc1.Instance 					= DMA1_Channel1;
    hdma_adc1.Init.Direction 			= DMA_PERIPH_TO_MEMORY;
    hdma_adc1.Init.PeriphInc 			= DMA_PINC_DISABLE;
    hdma_adc1.Init.MemInc 				= DMA_MINC_ENABLE;
    hdma_adc1.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_HALFWORD;
    hdma_adc1.Init.MemDataAlignment 	= DMA_MDATAALIGN_HALFWORD;
    hdma_adc1.Init.Mode 				= DMA_NORMAL;
    hdma_adc1.Init.Priority 			= DMA_PRIORITY_MEDIUM;
    HAL_DMA_Init(&hdma_adc1);

	/*link*/
	__HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);

	/* DMA2_Stream0_IRQn interrupt configuration */
  	HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  	HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);

	/*GPIO init*/
	GPIO_InitTypeDef GPIO_InitStruct;


    /**ADC1 GPIO Configuration
    PA0-WKUP     ------> ADC1_IN0 - ROTATRY_0
    PA1     ------> ADC1_IN1 - ROTATRY_1
    PA2     ------> ADC1_IN2 - TRANSPORT
    */
    GPIO_InitStruct.Pin = CC_ROTARY0_Pin|CC_ROTARY1_Pin|TRANSPORT_BTN_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);


	/* ADC1 Init */
	ADC_ChannelConfTypeDef sConfig;
	__HAL_RCC_ADC1_CLK_ENABLE();

	hadc1.Instance 						= ADC1;
	hadc1.Init.ScanConvMode 			= ADC_SCAN_ENABLE;
	hadc1.Init.ContinuousConvMode 		= DISABLE;
	hadc1.Init.DiscontinuousConvMode 	= DISABLE;
	hadc1.Init.ExternalTrigConv 		= ADC_SOFTWARE_START;
	hadc1.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
	hadc1.Init.NbrOfConversion 			= 3;
	HAL_ADC_Init(&hadc1);


	/*Рангом задаётся последовательность преобразования*/
	sConfig.Channel = ADC_CHANNEL_0;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_2;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);

	sConfig.Channel = ADC_CHANNEL_2;
	sConfig.Rank = ADC_REGULAR_RANK_3;
	HAL_ADC_ConfigChannel(&hadc1, &sConfig);
	return;
}
/*-------------------------------------------------------------------------*/








/*-------------------------------------------------------------------------*/
void adc2_init (void)
{
	/*GPIO init*/
	GPIO_InitTypeDef GPIO_InitStruct;

    /**ADC1 GPIO Configuration
		PA3     ------> ADC1_IN3 - Batary
    */
    GPIO_InitStruct.Pin = BATARY_CHARGE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* ADC2 Init */
	ADC_ChannelConfTypeDef sConfig;
	__HAL_RCC_ADC2_CLK_ENABLE();

	hadc2.Instance 						= ADC2;
	hadc2.Init.ScanConvMode 			= ADC_SCAN_DISABLE;
	hadc2.Init.ContinuousConvMode 		= DISABLE;
	hadc2.Init.DiscontinuousConvMode 	= DISABLE;
	hadc2.Init.ExternalTrigConv 		= ADC_SOFTWARE_START;
	hadc2.Init.DataAlign 				= ADC_DATAALIGN_RIGHT;
	hadc2.Init.NbrOfConversion 			= 1;
	HAL_ADC_Init(&hadc2);

	sConfig.Channel = ADC_CHANNEL_3;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
	HAL_ADC_ConfigChannel(&hadc2, &sConfig);

	//SET IT
	HAL_NVIC_SetPriority(ADC1_2_IRQn, 0, 0);
  	HAL_NVIC_EnableIRQ(ADC1_2_IRQn);
	return;
}
/*-------------------------------------------------------------------------*/

