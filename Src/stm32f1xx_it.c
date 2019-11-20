
#include "stm32f1xx_hal.h"
#include "stm32f1xx.h"
#include "stm32f1xx_it.h"
#include "midi_baby.h"


/* External variables --------------------------------------------------------*/
extern PCD_HandleTypeDef        hpcd_USB_FS;
extern DMA_HandleTypeDef        hdma_adc1;
extern ADC_HandleTypeDef        hadc2;
extern UART_HandleTypeDef       huart3;
extern TIM_HandleTypeDef        htim4;
extern DMA_HandleTypeDef        hdma_i2c1_tx;
extern DMA_HandleTypeDef        hdma_i2c1_rx;
extern common_flags_t           common_flags;
extern I2C_HandleTypeDef        hi2c1;


/******************************************************************************/
/*            Cortex-M3 Processor Interruption and Exception Handlers         */
/******************************************************************************/

/**
* @brief This function handles Non maskable interrupt.
*/
void NMI_Handler(void)
{
}

/**
* @brief This function handles Hard fault interrupt.
*/
void HardFault_Handler(void)
{
  	HAL_GPIO_WritePin(BATARY_ON_OFF_GPIO_Port, BATARY_ON_OFF_Pin, GPIO_PIN_RESET);
    while (1)
    {
    }
}

/**
* @brief This function handles Memory management fault.
*/
void MemManage_Handler(void)
{
    while (1)
    {
    }
}

/**
* @brief This function handles Prefetch fault, memory access fault.
*/
void BusFault_Handler(void)
{
    while (1)
    {
    }
}

/**
* @brief This function handles Undefined instruction or illegal state.
*/
void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

/**
* @brief This function handles System service call via SWI instruction.
*/
void SVC_Handler(void)
{
}

/**
* @brief This function handles Debug monitor.
*/
void DebugMon_Handler(void)
{
}

/**
* @brief This function handles Pendable request for system service.
*/
void PendSV_Handler(void)
{
}

/**
* @brief This function handles System tick timer.
*/
void SysTick_Handler(void)
{
    HAL_IncTick();
    HAL_SYSTICK_IRQHandler();
}

// USB IT
void USB_LP_CAN1_RX0_IRQHandler (void)
{
    HAL_PCD_IRQHandler(&hpcd_USB_FS);
}

// ADC conversion for rotary and buttons
void DMA1_Channel1_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_adc1);
}

//adc 2 charging measuremnt
void ADC1_2_IRQHandler (void)
{
    HAL_ADC_IRQHandler(&hadc2);
}

// transmit to MPU-6050
void DMA1_Channel6_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_i2c1_tx);
}

// recive FROM mpu-6050
void DMA1_Channel7_IRQHandler(void)
{
    HAL_DMA_IRQHandler(&hdma_i2c1_rx);
}

// data from CC2541 ble chip
void USART3_IRQHandler(void)
{
    HAL_UART_IRQHandler(&huart3);
}

//
void TIM4_IRQHandler (void)
{
    HAL_TIM_IRQHandler(&htim4);
}

//
void I2C1_ER_IRQHandler (void)
{
    HAL_I2C_ER_IRQHandler(&hi2c1);
}


















