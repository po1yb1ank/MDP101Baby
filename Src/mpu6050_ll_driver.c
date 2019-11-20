

#include "main.h"
#include "mpu6050_ll_driver.h"
#include "stm32f1xx_hal.h"




I2C_HandleTypeDef 				hi2c1;
DMA_HandleTypeDef               hdma_i2c1_tx;
DMA_HandleTypeDef               hdma_i2c1_rx;



/*
*/
static uint16_t accel_full_scale = 0;



/*
*/
static uint16_t gyro_full_scale = 0;





/*
	Clear interrupts flag for any IT.
	returns value if INT_STATUS register.
	bit 6 = 1 -> motion detection interrupts has generated.
				 It means that accelerometer value exceeded 'motion_max_value' in threshold register

	bit 4 = 1 -> FIFO overflow interrupts
	bit 3 = 1 -> I2C master interrupts
	bit 0 = 1 -> Data ready interrupts
*/
static uint8_t	mpu6050_clear_interrupt_flag		(void);



/*
	ext_sync = 0 - input disable
	ext_sync = 1 - temp_out_l
	ext_sync = 2 - gyro_xout_l
	ext_sync = 3 - gyro_yout_l
	ext_sync = 4 - gyro_zout_l
	ext_sync = 5 - accel_xout_l
	ext_sync = 6 - accel_yout_l
	ext_sync = 7 - accel_zout_l

*/
static void 	mpu6050_set_ext_sync				(uint8_t ext_sync);


/*
*/
static void 	mpu6050_define_full_scale			(void);















/*----------------------------------------------------------------*/
void mpu6050_init (void)
{
	uint8_t tmp_reg = 0;
	uint8_t tmp[2] = {0};

	// initialization I2C for data transfer
	mpu6050_ll_hardware_init();
	HAL_Delay(10);

	// reset whole device
	mpu6050_reset();

	// set PLL with Z axis gyroscope reference(default: internal 8 MHz oscillator)
	tmp[0] = PWR_MGMT_1;
	tmp[1] = 0x03;
	mpu6050_ll_write_data(tmp, 2);

	//DLPF_CFG = 1: Fs = 1 kHz; bandwidth = 42 Hz
	mpu6050_set_lowpass_filter(MPU6050_DIGITAL_LOWPASS_CONF);

	//	SAMPLE RATE
	//	Set gyro output sample rate.
	//	For accelerometer output sample rate - 1kHz
	//	gyro_output_sample_rate = 8kHz, when DLPF(low pass) is disabled and 1kHz, when it is enabled
	//	sample rate = gyro_output_sample_rate/(1 + divider)
	tmp[0] = SMPRT_DIV;
	tmp[1] = ((8000/MPU6050_SAMPLE_RATE_FOR_NEW_DATA) - 1);
	mpu6050_ll_write_data(tmp, 2);

	// gyro full scale setting
	mpu6050_gyro_set_full_scale(MPU6050_GYRO_FULL_SCALE);

	// accel full scale setting
	mpu6050_accel_set_full_scale(MPU6050_ACCEL_FULL_SCALE);

	// interrupt status bits are cleared by any read operation
	#if (MPU6050_CLEAR_INT_WHEN_ANY_READ == 1)
	{
		tmp_reg = mpu6050_ll_read_byte(INT_PIN_CONFIG);

		tmp[0] = INT_PIN_CONFIG;
		tmp[1] = (tmp_reg | 0x10);

		mpu6050_ll_write_data(tmp, 2);
	}
	// interrupt status bits are cleared by only read status register
	#else
	{
		tmp_reg = mpu6050_ll_read_byte(INT_PIN_CONFIG);

		tmp[0] = INT_PIN_CONFIG;
		tmp[1] = (tmp_reg & 0xEF);

		mpu6050_ll_write_data(tmp, 2);
	}
	#endif /*CLEAR_WHEN_ANY_READ*/

	// Interrupt PIN settings
	#if ((MPU6050_FIFO_INT_ENABLE == 1) || (MPU6050_DATA_RDY_INT_ENABLE == 1) || (MPU6050_MOTION_THRS_INT_ENABLE == 1))
	{
		//ACTIVE LEVEL
		#if (MPU6050_INT_PIN_ACTIVE_LEVEL_HI == 1)
		{
			tmp_reg = mpu6050_ll_read_byte(INT_PIN_CONFIG);

			tmp[0] = INT_PIN_CONFIG;
			tmp[1] = (tmp_reg & 0x7F);

			mpu6050_ll_write_data(tmp, 2);
		}
		#elif (MPU6050_INT_PIN_ACTIVE_LEVEL_LO == 1)
		{
			tmp_reg = mpu6050_ll_read_byte(INT_PIN_CONFIG);

			tmp[0] = INT_PIN_CONFIG;
			tmp[1] = (tmp_reg | 0x80);

			mpu6050_ll_write_data(tmp, 2);
		}
		#endif/**/

		//PIN MODE
		#if (MPU6050_INT_PIN_PUSH_PULL == 1)
		{
			tmp_reg = mpu6050_ll_read_byte(INT_PIN_CONFIG);

			tmp[0] = INT_PIN_CONFIG;
			tmp[1] = (tmp_reg & 0xBF);

			mpu6050_ll_write_data(tmp, 2);
		}
		#elif (MPU6050_INT_PIN_OPEN_DRAIN == 1)
		{
			tmp_reg = mpu6050_ll_read_byte(INT_PIN_CONFIG);

			tmp[0] = INT_PIN_CONFIG;
			tmp[1] = (tmp_reg | 0x40);

			mpu6050_ll_write_data(tmp, 2);
		}
		#endif/**/

		//INTERRUPT MODE
		#if (MPU6050_INT_PIN_PULSE == 1)
		{
			tmp_reg = mpu6050_ll_read_byte(INT_PIN_CONFIG);

			tmp[0] = INT_PIN_CONFIG;
			tmp[1] = (tmp_reg & 0xDF);

			mpu6050_ll_write_data(tmp, 2);
		}
		#elif (MPU6050_INT_PIN_HOLD_LEVEL == 1)
		{
			tmp_reg = mpu6050_ll_read_byte(INT_PIN_CONFIG);

			tmp[0] = INT_PIN_CONFIG;
			tmp[1] = (tmp_reg | 0x20);

			mpu6050_ll_write_data(tmp, 2);
		}
		#endif/**/
	}
	#endif

	#if (MPU6050_FIFO_INT_ENABLE == 1)
	{
		mpu6050_fifo_overflow_int_enable();
	}
	#endif

	#if (MPU6050_DATA_RDY_INT_ENABLE == 1)
	{
		mpu6050_data_ready_int_enable();
	}
	#endif

	#if (MPU6050_MOTION_THRS_INT_ENABLE == 1)
	{
		mpu6050_motion_int_enable();
		mpu6050_threshold_value_int_config(MPU6050_MOTION_THRESHOLD);
	}
	#endif

	//disable input
	mpu6050_set_ext_sync(0);

	// reset gyro, accelerometer and temperature sensor
	tmp[0] = SIGNAL_PATH_RESET;
	tmp[1] = 0x07;
	mpu6050_ll_write_data(tmp, 2);

	// receive bytes from MPU6050 with help DMA
	mpu6050_ll_dma_hardware_init();

	//
	mpu6050_define_full_scale();
	return;
}
/*----------------------------------------------------------------*/




/*----------------------------------------------------------------*/
void mpu6050_reset (void)
{
	uint8_t tmp[2] = {0};

	tmp[0] = PWR_MGMT_1;
	tmp[1] = 1 << 7;

	mpu6050_ll_write_data(tmp, 2);
	HAL_Delay(10);

	return;
}
/*----------------------------------------------------------------*/





/*----------------------------------------------------------------*/
void mpu6050_go_sleep_mode (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(PWR_MGMT_1);

	//
	data_to_reg[0] = PWR_MGMT_1;
	data_to_reg[1] = (tmp_reg | 0x40);

	mpu6050_ll_write_data(data_to_reg, 2);

	return;
}
/*----------------------------------------------------------------*/





/*----------------------------------------------------------------*/
void mpu6050_go_cycle_mode (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(PWR_MGMT_1);

	//
	data_to_reg[0] = PWR_MGMT_1;
	data_to_reg[1] = (tmp_reg | 0x20);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_set_wakeup_freq (uint8_t freq)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(PWR_MGMT_2);

	//
	data_to_reg[0] = PWR_MGMT_2;
	data_to_reg[1] = ((tmp_reg & 0x3F) | (freq << 6));

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_temp_sens_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(PWR_MGMT_1);

	//
	data_to_reg[0] = PWR_MGMT_1;
	data_to_reg[1] = (tmp_reg & 0xF7);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/





/*----------------------------------------------------------------*/
void mpu6050_temp_sens_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(PWR_MGMT_1);

	//
	data_to_reg[0] = PWR_MGMT_1;
	data_to_reg[1] = (tmp_reg | 0x08);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*===============================================================================*/
/*===============================SCALE SET FUNCTIONS=============================*/

/*----------------------------------------------------------------*/
void mpu6050_gyro_set_full_scale (gyro_scale_range_t range)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(GYRO_CONFIG);

	//
	data_to_reg[0] = GYRO_CONFIG;
	data_to_reg[1] = (((tmp_reg) & (0xE7)) | (range << 3));

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_accel_set_full_scale (accel_scale_range_t range)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(ACCEL_CONFIG);

	//
	data_to_reg[0] = ACCEL_CONFIG;
	data_to_reg[1] = (((tmp_reg) & (0xE7)) | (range << 3));

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/

/*===============================================================================*/
/*===============================FIFO FUNCTIONS==================================*/

/*----------------------------------------------------------------*/
void mpu6050_fifo_temp_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(FIFO_EN);

	//
	data_to_reg[0] = FIFO_EN;
	data_to_reg[1] = (tmp_reg | 0x80);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}


void mpu6050_fifo_temp_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(FIFO_EN);

	//
	data_to_reg[0] = FIFO_EN;
	data_to_reg[1] = (tmp_reg & 0x7F);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_fifo_x_gyro_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(FIFO_EN);

	//
	data_to_reg[0] = FIFO_EN;
	data_to_reg[1] = (tmp_reg | 0x40);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}


void mpu6050_fifo_x_gyro_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(FIFO_EN);

	//
	data_to_reg[0] = FIFO_EN;
	data_to_reg[1] = (tmp_reg & 0xBF);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_fifo_y_gyro_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(FIFO_EN);

	//
	data_to_reg[0] = FIFO_EN;
	data_to_reg[1] = (tmp_reg | 0x20);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}


void mpu6050_fifo_y_gyro_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(FIFO_EN);

	//
	data_to_reg[0] = FIFO_EN;
	data_to_reg[1] = (tmp_reg & 0xDF);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_fifo_z_gyro_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(FIFO_EN);

	//
	data_to_reg[0] = FIFO_EN;
	data_to_reg[1] = (tmp_reg | 0x10);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}


void mpu6050_fifo_z_gyro_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(FIFO_EN);

	//
	data_to_reg[0] = FIFO_EN;
	data_to_reg[1] = (tmp_reg & 0xEF);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_fifo_accel_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(FIFO_EN);

	//
	data_to_reg[0] = FIFO_EN;
	data_to_reg[1] = (tmp_reg | 0x08);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}


void mpu6050_fifo_accel_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(FIFO_EN);

	//
	data_to_reg[0] = FIFO_EN;
	data_to_reg[1] = (tmp_reg & 0xF7);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_fifo_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(USER_CTRL);

	//
	data_to_reg[0] = USER_CTRL;
	data_to_reg[1] = (tmp_reg | 0x40);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_fifo_reset (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(USER_CTRL);

	//
	data_to_reg[0] = USER_CTRL;
	data_to_reg[1] = (tmp_reg | 0x04);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
uint16_t mpu6050_fifo_get_available_bytes (void)
{
	uint8_t data[2] = {0};

	//data[0] - high byte, data[1] - low byte
	mpu6050_ll_read_nbytes(data, FIFO_COUNTS_H, 2);

	return (uint16_t)((((uint16_t)data[0]) << 8) | data[1]);
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
uint8_t mpu6050_fifo_read_byte (void)
{
	return (mpu6050_ll_read_byte(FIFO_R_W));
}
/*----------------------------------------------------------------*/


/*===============================================================================*/
/*============================INTERRUPTS CONFIG==================================*/

/*----------------------------------------------------------------*/
uint8_t mpu6050_is_motion_int_occurred (void)
{
	uint8_t int_status = mpu6050_clear_interrupt_flag();

	if (int_status & 0x40)
		return 1;

	return 0;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
uint8_t mpu6050_is_fifo_overflow_int_occurred (void)
{
	uint8_t int_status = mpu6050_clear_interrupt_flag();

	if (int_status & 0x10)
		return 1;

	return 0;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
uint8_t mpu6050_is_data_ready_int_occurred (void)
{
	uint8_t int_status = mpu6050_clear_interrupt_flag();

	if (int_status & 0x01)
		return 1;

	return 0;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_motion_int_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(INT_ENABLE);

	//
	data_to_reg[0] = INT_ENABLE;
	data_to_reg[1] = (tmp_reg | 0x40);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}

void mpu6050_motion_int_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(INT_ENABLE);

	//
	data_to_reg[0] = INT_ENABLE;
	data_to_reg[1] = (tmp_reg & 0xBF);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_fifo_overflow_int_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(INT_ENABLE);

	//
	data_to_reg[0] = INT_ENABLE;
	data_to_reg[1] = (tmp_reg | 0x10);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}

void mpu6050_fifo_overflow_int_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(INT_ENABLE);

	//
	data_to_reg[0] = INT_ENABLE;
	data_to_reg[1] = (tmp_reg & 0xEF);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_data_ready_int_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(INT_ENABLE);

	//
	data_to_reg[0] = INT_ENABLE;
	data_to_reg[1] = (tmp_reg | 0x01);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}

void mpu6050_data_ready_int_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(INT_ENABLE);

	//
	data_to_reg[0] = INT_ENABLE;
	data_to_reg[1] = (tmp_reg & 0xFE);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_threshold_value_int_config (uint8_t motion_max_value)
{
	uint8_t data_to_reg[2] = {0};

	data_to_reg[0] = MOT_THR;
	data_to_reg[1] = motion_max_value;

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*===============================================================================*/
/*============================LOW LEVEL FUNCTION(without DMA)====================*/

/*----------------------------------------------------------------*/
static void mpu6050_i2c_analog_magic (void)
{
	GPIO_PinState scl_state = GPIO_PIN_RESET;
	GPIO_PinState sda_state = GPIO_PIN_RESET;

	//1. Disable the I2C peripheral by clearing the PE bit in I2Cx_CR1 register.
	__HAL_I2C_DISABLE(&hi2c1);

	//2. Configure the SCL and SDA I/Os as General Purpose Output Open-Drain, High level(Write 1 to GPIOx_ODR).
	/**I2C1 GPIO Configuration
	PB8     ------> I2C1_SCL
	PB9     ------> I2C1_SDA
	*/
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8|GPIO_PIN_9, GPIO_PIN_SET);
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//3. Check SCL and SDA High level in GPIOx_IDR.
	scl_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);
	sda_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);

	//4. Configure the SDA I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_RESET);

	//5. Check SDA Low level in GPIOx_IDR.
	sda_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);

	//6. Configure the SCL I/O as General Purpose Output Open-Drain, Low level (Write 0 to GPIOx_ODR).
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);

	//7. Check SCL Low level in GPIOx_IDR.
	scl_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);

	//8. Configure the SCL I/O as General Purpose Output Open-Drain, High level (Write 1 to GPIOx_ODR).
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);

	//9. Check SCL High level in GPIOx_IDR.
	scl_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_8);

	//10. Configure the SDA I/O as General Purpose Output Open-Drain , High level (Write 1 to GPIOx_ODR).
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_9, GPIO_PIN_SET);

	//11. Check SDA High level in GPIOx_IDR.
	sda_state = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9);

	//12. Configure the SCL and SDA I/Os as Alternate function Open-Drain.
	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	//13. Set SWRST bit in I2Cx_CR1 register.
	hi2c1.Instance->CR1 |= I2C_CR1_SWRST;

	//14. Clear SWRST bit in I2Cx_CR1 register.
	hi2c1.Instance->CR1 &= ~(I2C_CR1_SWRST);

	//15. Enable the I2C peripheral by setting the PE bit in I2Cx_CR1 register
	__HAL_I2C_ENABLE(&hi2c1);

	(void)sda_state;
	(void)scl_state;

	return;
}


void mpu6050_ll_hardware_init (void)
{
	/*i2c configuration*/
	hi2c1.Instance 				= I2C1;
	hi2c1.Init.ClockSpeed 		= 100000;
	hi2c1.Init.DutyCycle 		= I2C_DUTYCYCLE_2;
	hi2c1.Init.OwnAddress1 		= 0;
	hi2c1.Init.AddressingMode 	= I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode 	= I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 		= 0;
	hi2c1.Init.GeneralCallMode 	= I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode 	= I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&hi2c1);

	//mpu6050_i2c_analog_magic();

	HAL_NVIC_SetPriority(I2C1_ER_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(I2C1_ER_IRQn);
	return;
}

void HAL_I2C_MspInit(I2C_HandleTypeDef* hi2c)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	if(hi2c->Instance==I2C1)
	{
		/**I2C1 GPIO Configuration
		PB8     ------> I2C1_SCL
		PB9     ------> I2C1_SDA
		*/
		__HAL_RCC_GPIOB_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		// Interrupt PIN settings
		#if ((MPU6050_FIFO_INT_ENABLE == 1) || (MPU6050_DATA_RDY_INT_ENABLE == 1) || (MPU6050_MOTION_THRS_INT_ENABLE == 1))
		{
			/*Interrupt pin for detect that data from MPU-6050 is ready*/
			GPIO_InitStruct.Pin = AXYZ_INT1_Pin;
			GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
			GPIO_InitStruct.Pull = GPIO_NOPULL;
			HAL_GPIO_Init(AXYZ_INT1_GPIO_Port, &GPIO_InitStruct);

			/*pin it config*/
			HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
			HAL_NVIC_EnableIRQ(EXTI0_IRQn);
		}
		#endif
		__HAL_AFIO_REMAP_I2C1_ENABLE();

		__HAL_RCC_I2C1_CLK_ENABLE();
		__HAL_RCC_I2C1_FORCE_RESET();
		__HAL_RCC_I2C1_RELEASE_RESET();
	}
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_ll_write_data (uint8_t* data, uint16_t len)
{
	//set write bit - 0
	HAL_I2C_Master_Transmit(&hi2c1, (MPU6050_DEV_ADR << 1), data, len, 5);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
uint8_t mpu6050_ll_read_byte (uint8_t register_for_read)
{
  	uint8_t reg_data = 0;

	//set register address which we will read
	HAL_I2C_Master_Transmit(&hi2c1, (MPU6050_DEV_ADR << 1), &register_for_read, 1, 5);

	//request for reading (read bit = 1). Reading process ongoing
	HAL_I2C_Master_Receive(&hi2c1, ((MPU6050_DEV_ADR << 1) | 0x01), &reg_data, 1, 5);

	return reg_data;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_ll_read_nbytes (uint8_t* destination_buf, uint8_t begin_reg_for_read, uint8_t counts_read)
{
	//set register address which we will read
	HAL_I2C_Master_Transmit(&hi2c1, (MPU6050_DEV_ADR << 1), &begin_reg_for_read, 1, 10);
	HAL_I2C_Master_Receive(&hi2c1, ((MPU6050_DEV_ADR << 1) | 0x01), destination_buf, counts_read, 10);
	return;
}
/*----------------------------------------------------------------*/





/*===============================================================================*/
/*============================LOW LEVEL FUNCTION(WITH DMA)=======================*/

/*----------------------------------------------------------------*/
void mpu6050_ll_dma_hardware_init (void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	/* DMA1_Channel6_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel6_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel6_IRQn);

	/* DMA1_Channel7_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Channel7_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Channel7_IRQn);

    /**I2C1 GPIO Configuration
    PB8     ------> I2C1_SCL
    PB9     ------> I2C1_SDA
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	// Interrupt PIN settings
	#if ((MPU6050_FIFO_INT_ENABLE == 1) || (MPU6050_DATA_RDY_INT_ENABLE == 1) || (MPU6050_MOTION_THRS_INT_ENABLE == 1))
	{
		/*Interrupt pin for detect that data from MPU-6050 is ready*/
		GPIO_InitStruct.Pin = AXYZ_INT1_Pin;
		GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(AXYZ_INT1_GPIO_Port, &GPIO_InitStruct);

		/*pin it config*/
		HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
		HAL_NVIC_EnableIRQ(EXTI0_IRQn);
	}
	#endif

    __HAL_AFIO_REMAP_I2C1_ENABLE();

	/* Peripheral clock enable */
	__HAL_RCC_I2C1_CLK_ENABLE();
	__HAL_RCC_I2C1_FORCE_RESET();
	__HAL_RCC_I2C1_RELEASE_RESET();

    /* I2C1_TX Init */
    hdma_i2c1_tx.Instance 					= DMA1_Channel6;
    hdma_i2c1_tx.Init.Direction 			= DMA_MEMORY_TO_PERIPH;
    hdma_i2c1_tx.Init.PeriphInc 			= DMA_PINC_DISABLE;
    hdma_i2c1_tx.Init.MemInc 				= DMA_MINC_ENABLE;
    hdma_i2c1_tx.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.MemDataAlignment 		= DMA_MDATAALIGN_BYTE;
    hdma_i2c1_tx.Init.Mode 					= DMA_NORMAL;
    hdma_i2c1_tx.Init.Priority 				= DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma_i2c1_tx);

    __HAL_LINKDMA(&hi2c1,hdmatx,hdma_i2c1_tx);

    /* I2C1_RX Init */
    hdma_i2c1_rx.Instance 					= DMA1_Channel7;
    hdma_i2c1_rx.Init.Direction 			= DMA_PERIPH_TO_MEMORY;
    hdma_i2c1_rx.Init.PeriphInc 			= DMA_PINC_DISABLE;
    hdma_i2c1_rx.Init.MemInc 				= DMA_MINC_ENABLE;
    hdma_i2c1_rx.Init.PeriphDataAlignment 	= DMA_PDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.MemDataAlignment 		= DMA_MDATAALIGN_BYTE;
    hdma_i2c1_rx.Init.Mode 					= DMA_NORMAL;
    hdma_i2c1_rx.Init.Priority 				= DMA_PRIORITY_LOW;
    HAL_DMA_Init(&hdma_i2c1_rx);

    __HAL_LINKDMA(&hi2c1,hdmarx,hdma_i2c1_rx);

 	//I2C inti
	hi2c1.Instance 				= I2C1;
	hi2c1.Init.ClockSpeed 		= 400000;
	hi2c1.Init.DutyCycle 		= I2C_DUTYCYCLE_16_9;
	hi2c1.Init.OwnAddress1 		= 0;
	hi2c1.Init.AddressingMode 	= I2C_ADDRESSINGMODE_7BIT;
	hi2c1.Init.DualAddressMode 	= I2C_DUALADDRESS_DISABLE;
	hi2c1.Init.OwnAddress2 		= 0;
	hi2c1.Init.GeneralCallMode 	= I2C_GENERALCALL_DISABLE;
	hi2c1.Init.NoStretchMode 	= I2C_NOSTRETCH_DISABLE;
	HAL_I2C_Init(&hi2c1);

	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_ll_dma_read_nbytes (uint8_t* destination_buf, uint8_t begin_reg_for_read, uint8_t counts_read)
{
	//set register address which we will read
	HAL_I2C_Master_Transmit(&hi2c1, (MPU6050_DEV_ADR << 1), &begin_reg_for_read, 1, 10);
	HAL_I2C_Master_Receive_DMA(&hi2c1, ((MPU6050_DEV_ADR << 1) | 0x01), destination_buf, counts_read);

	return;
}
/*----------------------------------------------------------------*/






/*----------------------------------------------------------------*/
static void mpu6050_set_lowpass_filter (uint8_t filter_val)
{
	if (filter_val >= 7)
		return;

	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(CONFIG_DLPF_OR_EXT_SYNC);

	//
	data_to_reg[0] = CONFIG_DLPF_OR_EXT_SYNC;
	data_to_reg[1] = (((tmp_reg) & (0xF8)) | filter_val);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
static void mpu6050_set_ext_sync (uint8_t ext_sync)
{
	if (ext_sync > 7)
		return;

	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(CONFIG_DLPF_OR_EXT_SYNC);

	//
	data_to_reg[0] = CONFIG_DLPF_OR_EXT_SYNC;
	data_to_reg[1] = (((tmp_reg) & (0xC7)) | (ext_sync << 3));

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
static uint8_t mpu6050_clear_interrupt_flag (void)
{
	return (mpu6050_ll_read_byte(INT_STATUS));
}
/*----------------------------------------------------------------*/



/*----------------------------------------------------------------*/
static void mpu6050_define_full_scale (void)
{
	/*-------------------------------*/
	if (MPU6050_GYRO_FULL_SCALE == RANGE_250_RADIAN_PER_SEC)
		gyro_full_scale = 250;

	else if (MPU6050_GYRO_FULL_SCALE == RANGE_500_RADIAN_PER_SEC)
		gyro_full_scale = 500;

	else if (MPU6050_GYRO_FULL_SCALE == RANGE_1000_RADIAN_PER_SEC)
		gyro_full_scale = 1000;

	else if (MPU6050_GYRO_FULL_SCALE == RANGE_2000_RADIAN_PER_SEC)
		gyro_full_scale = 2000;

	/*-------------------------------*/

	/*-------------------------------*/
	if (MPU6050_ACCEL_FULL_SCALE == RANGE_2G)
		accel_full_scale = 2;

	else if (MPU6050_ACCEL_FULL_SCALE == RANGE_4G)
		accel_full_scale = 4;

	else if (MPU6050_ACCEL_FULL_SCALE == RANGE_8G)
		accel_full_scale = 8;

	else if (MPU6050_ACCEL_FULL_SCALE == RANGE_16G)
		accel_full_scale = 16;
	/*-------------------------------*/

	return;
}
/*----------------------------------------------------------------*/




/*----------------------------------------------------------------*/
uint16_t mpu6050_get_accel_full_scale_val (void)
{
	return accel_full_scale;
}
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
uint16_t mpu6050_get_gyro_full_scale_val (void)
{
	return gyro_full_scale;
}
/*----------------------------------------------------------------*/


