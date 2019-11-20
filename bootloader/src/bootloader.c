





#include "bootloader.h"
#include "stm32f1xx_hal.h"



CRC_HandleTypeDef 		hcrc;





/*-------------------------------------------------------------*/
void hardware_crc_init (void)
{
	__HAL_RCC_CRC_CLK_ENABLE();

	hcrc.Instance = CRC;
	HAL_CRC_Init(&hcrc);
	
	return;
}
/*-------------------------------------------------------------*/





/*-------------------------------------------------------------*/
void hardware_flash_init (void)
{
	return;
}
/*-------------------------------------------------------------*/





