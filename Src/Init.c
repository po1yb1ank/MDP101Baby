
#include "Init.h"
#include "stm32f1xx_hal.h"


extern PCD_HandleTypeDef 				hpcd_USB_FS;
extern DMA_HandleTypeDef 				hdma_adc1;
extern ADC_HandleTypeDef 				hadc1;




/********************************************************************/
void SystemClock_Config(void)
{
    #if (EXTERNAL_OSC == 1)
    {
        RCC_OscInitTypeDef 		       RCC_OscInitStruct;
        RCC_ClkInitTypeDef 		       RCC_ClkInitStruct;
        RCC_PeriphCLKInitTypeDef 	   PeriphClkInit;


        //Initializes the CPU, AHB and APB busses clocks
        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
        RCC_OscInitStruct.HSEState = RCC_HSE_ON;
        RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV2;
        RCC_OscInitStruct.LSEState = RCC_LSE_OFF;
    	// The Flash memory programming interface clock (FLITFCLK) is always the HSI clock.
        RCC_OscInitStruct.HSIState = RCC_HSI_ON;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
        HAL_RCC_OscConfig(&RCC_OscInitStruct);

        //Initializes the CPU, AHB and APB busses clocks
        RCC_ClkInitStruct.ClockType 		= RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  		|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
        RCC_ClkInitStruct.SYSCLKSource 	= RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider 	= RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider 	= RCC_HCLK_DIV2;
        RCC_ClkInitStruct.APB2CLKDivider 	= RCC_HCLK_DIV8;

        HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
        PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
        PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
        HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);

        //Configure the Systick interrupt time
        HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

        //Configure the Systick
        HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);

        //SisTick interrupt
        HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    }
    #else//internal
    {
        RCC_OscInitTypeDef RCC_OscInitStruct;
        RCC_ClkInitTypeDef RCC_ClkInitStruct;
        RCC_PeriphCLKInitTypeDef PeriphClkInit;

        RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
        RCC_OscInitStruct.HSIState = RCC_HSI_ON;
        RCC_OscInitStruct.HSICalibrationValue = 16;
        RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
        RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
        RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
        HAL_RCC_OscConfig(&RCC_OscInitStruct);

        RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                                  |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
        RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
        RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
        RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
        RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV8;

        HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);

        PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
        PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
		PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;

        HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit);
        HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

        HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
        HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);
    }
    #endif /*EXTERNAL_OSC*/
	SystemCoreClockUpdate();
    return;
}
/********************************************************************/



/********************************************************************/
void GPIO_init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOB, MOTION_LED_Pin|BAT_LED_Pin|HEART_LED_Pin, GPIO_PIN_RESET);


    /*БОКОВЫЕ КНОПКИ : CC_BTN0_Pin CC_BTN1_Pin ++++++++++*/
    GPIO_InitStruct.Pin = CC_BTN0_Pin|CC_BTN1_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*power button*/
    GPIO_InitStruct.Pin = POWER_BUTTON_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(POWER_BUTTON_GPIO_Port, &GPIO_InitStruct);

    /*batary on/off pin*/
    GPIO_InitStruct.Pin = BATARY_ON_OFF_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    //HAL_GPIO_Init(BATARY_ON_OFF_GPIO_Port, &GPIO_InitStruct);

	/*usb instert detect pin*/
    GPIO_InitStruct.Pin = USB_DETECT_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(USB_DETECT_GPIO_Port, &GPIO_InitStruct);

    /* MOTION_KEY_Pin ++++++++++*/
    GPIO_InitStruct.Pin = MOTION_KEY_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(MOTION_KEY_GPIO_Port, &GPIO_InitStruct);

    /*LEDs : BT_LED_Pin USB_LED_Pin BAT_LED_Pin ++++++++++*/
    GPIO_InitStruct.Pin = MOTION_LED_Pin|BAT_LED_Pin|HEART_LED_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /*pin for reset cc2541 chip*/
    GPIO_InitStruct.Pin = BLE_RESET_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BLE_RESET_GPIO_Port, &GPIO_InitStruct);

	/*usb enable/disable*/
    GPIO_InitStruct.Pin = USB_ENABLE_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(USB_ENABLE_GPIO_Port, &GPIO_InitStruct);

    /* enable/disable battary sensing */
    GPIO_InitStruct.Pin = BAT_SENS_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(BAT_SENS_GPIO_Port, &GPIO_InitStruct);
}
/********************************************************************/








/********************************************************************/
void HAL_MspInit(void)
{
    __HAL_RCC_AFIO_CLK_ENABLE();

    HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

    /* System interrupt init*/
    /* MemoryManagement_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);
    /* BusFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);
    /* UsageFault_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);
    /* SVCall_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SVCall_IRQn, 0, 0);
    /* DebugMonitor_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(DebugMonitor_IRQn, 0, 0);
    /* PendSV_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(PendSV_IRQn, 0, 0);
    /* SysTick_IRQn interrupt configuration */
    HAL_NVIC_SetPriority(SysTick_IRQn, 0, 0);

    /**NOJTAG: JTAG-DP Disabled and SW-DP Enabled
    */
    __HAL_AFIO_REMAP_SWJ_NOJTAG();
}
/********************************************************************/
