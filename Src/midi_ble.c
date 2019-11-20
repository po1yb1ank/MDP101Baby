/**
  ******************************************************************************
  * @file    midi_ble.c
  * @author  Pribore Electronics
  * @version V0.1
  * @date    2017
  * @brief   functions for midi ble protocol
  ******************************************************************************
  */


#include <stdint.h>
#include "stm32f1xx_hal.h"
#include "midi_baby.h"


//uart for communication with cc2541 chip
UART_HandleTypeDef                      huart3;

uint16_t midi_timestamp = 0;
uint8_t  byte_from_cc2541 = 0;


extern common_flags_t                   common_flags;


/*
    Calculate timestamp: step = 1ms
*/
/******************************************************************************/
void midi_ble_timestamp_inc (void)
{
	++midi_timestamp;

	if (midi_timestamp == 8192)
		midi_timestamp = 0;

	return;
}
/******************************************************************************/




/******************************************************************************/
void midi_ble_hardware_init (void)
{
    huart3.Instance                 = USART3;
    huart3.Init.BaudRate            = 115200;
    huart3.Init.WordLength          = UART_WORDLENGTH_8B;
    huart3.Init.StopBits            = UART_STOPBITS_1;
    huart3.Init.Parity              = UART_PARITY_NONE;
    huart3.Init.Mode                = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl           = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling        = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart3);

    //start detect receiving data from cc2541
    HAL_UART_Receive_IT(&huart3, &byte_from_cc2541, 1);
    return;
}

void HAL_UART_MspInit(UART_HandleTypeDef* huart)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    if (huart->Instance == USART3)
    {
        /* Peripheral clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();

        /**USART3 GPIO Configuration
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		GPIO_InitStruct.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* USART3 interrupt Init */
        HAL_NVIC_SetPriority(USART3_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(USART3_IRQn);
    }
}
/**********************************************************/



/******************************************************************************/
void midi_ble_ll_transmit (uint8_t* midi_data, uint16_t len)
{
    uint16_t i = 0;
    while((HAL_UART_Transmit(&huart3, midi_data, len, 100)) != HAL_OK)
    {
        ++i;

        if (i == 200)
            break;
    }
}
/******************************************************************************/








/******************************************************************************/
void midi_ble_parce_data  (void)
{
    //device connected
    if (byte_from_cc2541 == 0xFF)
        common_flags.bit.ble_connect = 1;

    //device disconnected
    else if (byte_from_cc2541 == 0x00)
    {
        common_flags.bit.ble_connect = 0;
        common_flags.bit.reset_ble = 1;
    }

    HAL_UART_Receive_IT(&huart3, &byte_from_cc2541, 1);
}
/******************************************************************************/



