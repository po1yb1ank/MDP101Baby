

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F1xx_IT_H
#define __STM32F1xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);


/*ADC DMA conversion complete interrupts*/
void DMA1_Channel1_IRQHandler (void);

/*USB MIDI Interrupts*/
void USB_LP_CAN1_RX0_IRQHandler (void);

/* connect with cc2541 chip */
void USART3_IRQHandler (void);

/* transmit to MPU-6050 */
void DMA1_Channel6_IRQHandler (void);

/* recive FROM mpu-6050 */
void DMA1_Channel7_IRQHandler (void);

/**/
void ADC1_2_IRQHandler (void);

/**/
void TIM4_IRQHandler (void);
#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_IT_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
