#ifndef MPU6050_LL_DRIVER_H
#define MPU6050_LL_DRIVER_H

#include <stdint.h>


/*
	Below you will find the defines with which you can configure MPU-6050
	'0' value is disabled corresponding target
	'1' value is enabled corresponding target
*/

//sample rate for internal ADC in Herz
#define MPU6050_SAMPLE_RATE_FOR_NEW_DATA		(uint16_t)(500)

//DIGITAL_LOWPASS_CONF = 0x00 - accel delay = 0.0 ms;  gyro delay = 0.98 ms;   Fs = 8 kHz;
//DIGITAL_LOWPASS_CONF = 0x01 - accel delay = 2.0 ms;  gyro delay = 1.90 ms;   Fs = 1 kHz;
//DIGITAL_LOWPASS_CONF = 0x02 - accel delay = 3.0 ms;  gyro delay = 2.80 ms;   Fs = 1 kHz;
//DIGITAL_LOWPASS_CONF = 0x03 - accel delay = 4.9 ms;  gyro delay = 4.80 ms;   Fs = 1 kHz;
//DIGITAL_LOWPASS_CONF = 0x04 - accel delay = 8.5 ms;  gyro delay = 8.30 ms;   Fs = 1 kHz;
//DIGITAL_LOWPASS_CONF = 0x05 - accel delay = 13.8 ms; gyro delay = 13.4 ms;   Fs = 1 kHz;
//DIGITAL_LOWPASS_CONF = 0x06 - accel delay = 19.0 ms; gyro delay = 18.6 ms;   Fs = 1 kHz;
#define MPU6050_DIGITAL_LOWPASS_CONF			(uint8_t)(0x03)

// gyro full scale setting
#define MPU6050_GYRO_FULL_SCALE 				(gyro_scale_range_t)(RANGE_2000_RADIAN_PER_SEC)

// accel full scale setting
#define MPU6050_ACCEL_FULL_SCALE 				(accel_scale_range_t)(RANGE_2G)

// clearing interrupts flag, when any read operations is occured
// or only read INT_STATUS register
#define MPU6050_CLEAR_INT_WHEN_ANY_READ			1

// if use it(MPU6050_CLEAR_INT_WHEN_READ_STATUS defined as 1), you must call one of this functions:
// 'mpu6050_is_data_ready_int_occurred', 'mpu6050_is_motion_int_occurred', 'mpu6050_is_fifo_overflow_int_occurred'
// that clearing interrupts flag into MPU-6050.
// This function clears the interrupts flag and return '1', is correscponding INT is occured.
// But in the begining you must enable corresponding interrupts
// if 'MPU6050_CLEAR_INT_WHEN_ANY_READ' defined as 1 interrupts flag will clear any read operations
#define MPU6050_CLEAR_INT_WHEN_READ_STATUS 		0

// Interrupts settings
// All it interrupts will show in INT_STATUS register, but
// If you enable only ONE interrupt, then it will output a signal to the GPIO pin.
#define MPU6050_FIFO_INT_ENABLE 				0
#define MPU6050_DATA_RDY_INT_ENABLE 			0
#define MPU6050_MOTION_THRS_INT_ENABLE 			0

// When acceleromenter data (any axis) will more than it define, mpu6050 will generate motion interrupts
// only if 'MPU6050_MOTION_THRS_INT_ENABLE' defined as 1
#define MPU6050_MOTION_THRESHOLD				(uint8_t)(0)

// Interrupt PIN settings
// Pin in your central processor (stm) you should preset yourself
#if ((MPU6050_FIFO_INT_ENABLE==1) || (MPU6050_DATA_RDY_INT_ENABLE==1) || (MPU6050_MOTION_THRS_INT_ENABLE==1))
	//ACTIVE LEVEL
	#define MPU6050_INT_PIN_ACTIVE_LEVEL_HI		1
	#define MPU6050_INT_PIN_ACTIVE_LEVEL_LO 	0

	//PIN MODE
	#define MPU6050_INT_PIN_PUSH_PULL			1
	#define MPU6050_INT_PIN_OPEN_DRAIN			0

	//INTERRUPTS MODE
	//When INT is happened int pin will emit a 50 us pulse
	#define MPU6050_INT_PIN_PULSE				1
	//The INT pin is held your level until the interrupt is cleared
	#define MPU6050_INT_PIN_HOLD_LEVEL			0
#endif










/*
*/
typedef enum
{
	RANGE_250_RADIAN_PER_SEC = 0,
	RANGE_500_RADIAN_PER_SEC = 1,
	RANGE_1000_RADIAN_PER_SEC = 2,
	RANGE_2000_RADIAN_PER_SEC = 3
}gyro_scale_range_t;


/*
*/
typedef enum
{
	RANGE_2G = 0,
	RANGE_4G = 1,
	RANGE_8G = 2,
	RANGE_16G = 3
}accel_scale_range_t;





/*
	Address for communication with MPU-6050
*/
#define 		MPU6050_DEV_ADR 				(uint16_t)(0x68)

/*
	REGISTERS MAP
*/
#define 		WHO_AM_I						(uint8_t)(0x75)
#define 		SMPRT_DIV						(uint8_t)(0x19)
#define 		CONFIG_DLPF_OR_EXT_SYNC			(uint8_t)(0x1A)
#define 		GYRO_CONFIG						(uint8_t)(0x1B)
#define 		ACCEL_CONFIG 					(uint8_t)(0x1C)
#define 		MOT_THR 						(uint8_t)(0x1F)
#define 		FIFO_EN							(uint8_t)(0x23)
#define 		INT_PIN_CONFIG 					(uint8_t)(0x37)
#define 		INT_ENABLE 						(uint8_t)(0x38)
#define 		INT_STATUS 						(uint8_t)(0x3A)

#define 		ACCEL_XOUT_H 					(uint8_t)(0x3B)
#define 		ACCEL_XOUT_L 					(uint8_t)(0x3C)
#define 		ACCEL_YOUT_H 					(uint8_t)(0x3D)
#define 		ACCEL_YOUT_L 					(uint8_t)(0x3E)
#define 		ACCEL_ZOUT_H 					(uint8_t)(0x3F)
#define 		ACCEL_ZOUT_L 					(uint8_t)(0x40)

#define 		TEMP_OUT_H 						(uint8_t)(0x41)
#define 		TEMP_OUT_L 						(uint8_t)(0x42)

#define 		GYRO_XOUT_H 					(uint8_t)(0x43)
#define 		GYRO_XOUT_L 					(uint8_t)(0x44)
#define 		GYRO_YOUT_H 					(uint8_t)(0x45)
#define 		GYRO_YOUT_L 					(uint8_t)(0x46)
#define 		GYRO_ZOUT_H 					(uint8_t)(0x47)
#define 		GYRO_ZOUT_L 					(uint8_t)(0x48)

#define 		SIGNAL_PATH_RESET				(uint8_t)(0x68)
#define 		MOT_DETECT_CTRL 				(uint8_t)(0x69)
#define 		USER_CTRL						(uint8_t)(0x6A)
#define 		PWR_MGMT_1 						(uint8_t)(0x6B)
#define 		PWR_MGMT_2 						(uint8_t)(0x6C)

#define 		FIFO_COUNTS_H					(uint8_t)(0x72)
#define 		FIFO_COUNTS_L					(uint8_t)(0x73)
#define 		FIFO_R_W 						(uint8_t)(0x74)

/*===============================================================================*/
/*========================INITIALIZATION AND POWER FUNCTIONS=====================*/

void 		mpu6050_init 						(void);
void 		mpu6050_deinit 						(void);
void 		mpu6050_reset 	 					(void);

/*
	Returns maximum measurement value
	(full scale value)
*/
uint16_t 	mpu6050_get_gyro_full_scale_val		(void);
uint16_t 	mpu6050_get_accel_full_scale_val	(void);

/*
*/
void 		mpu6050_go_sleep_mode				(void);

/*
	Device cycles between sleep and wake up with the frequency specified
	in register PWR_MGMT_2: bit 7 & 6 and do measurement.

	Use ONLY when sleep mode is disable
*/
void 		mpu6050_go_cycle_mode 				(void);


/*
	Set cycle period for cycle mode.
	Device wake up with 'freq', do sample and go to the sleep mode.
	freq = 0 -> sample 1.25Hz
	freq = 1 -> sample 5.00Hz
	freq = 2 -> sample 20.0Hz
	freq = 3 -> sample 40.0Hz

	This function will be usefull only CYCLE mode
*/
void 		mpu6050_set_wakeup_freq				(uint8_t freq);

/*
	Enable/disable tempreture sensor
*/
void 		mpu6050_temp_sens_enable			(void);
void 		mpu6050_temp_sens_disable			(void);

/*
	Configuration Digital Low Pass Filter(dlpf)
	Accelerometer and gyroscope are filtered according to 'filter_val'
	(Internal ADC will wait some time before next sampling)
*/
void 		mpu6050_set_lowpass_filter			(uint8_t filter_val);

/*
	Gyroscope/accelerometer full scale range settings
*/
void 		mpu6050_gyro_set_full_scale 		(gyro_scale_range_t range);
void 		mpu6050_accel_set_full_scale		(accel_scale_range_t range);



/*===============================================================================*/
/*===============================FIFO FUNCTIONS==================================*/

/*
	What is sensor loaded into FIFO buffer?
	The sensors are sampled at the Sample Rte as defined  in SMPRT_DIV register.
*/
void 		mpu6050_fifo_temp_enable 			(void);//TEMP_OT_H and TEMP_OUT_L writen to FIFO
void 		mpu6050_fifo_temp_disable 			(void);//

void 		mpu6050_fifo_x_gyro_enable 			(void);//GYRO_XOUT_H and GYRO_XOUT_L writen to FIFO
void 		mpu6050_fifo_x_gyro_disable			(void);//

void 		mpu6050_fifo_y_gyro_enable 			(void);//GYRO_YOUT_H and GYRO_YOUT_H writen to FIFO
void 		mpu6050_fifo_y_gyro_disable			(void);//

void 		mpu6050_fifo_z_gyro_enable 			(void);//GYRO_ZOUT_H and GYRO_ZOUT_Lwriten to FIFO
void 		mpu6050_fifo_z_gyro_disable			(void);//

void 		mpu6050_fifo_accel_enable			(void);//from ACCEL_XOUT_H to ACCEL_ZOUT_L (6 bytesn)
void 		mpu6050_fifo_accel_disable			(void);//

/*
*/
void 		mpu6050_fifo_enable 				(void);
void 		mpu6050_fifo_reset  				(void);

/*
	Returns count bytes in internal FIFO buffer
*/
uint16_t	mpu6050_fifo_get_available_bytes	(void);

/*
	Returns one byte from fifo(if corresponding FIFO enable)
	FIFO is written with sample rate
*/
uint8_t 	mpu6050_fifo_read_byte				(void);


/*===============================================================================*/
/*============================INTERRUPTS CONFIG==================================*/

/*
	Enable/disable interrupt generator in MPU-6050 circuit
*/
void 		mpu6050_motion_int_enable 			(void);
void 		mpu6050_motion_int_disable 			(void);

void 		mpu6050_fifo_overflow_int_enable	(void);
void 		mpu6050_fifo_overflow_int_disable	(void);

void  		mpu6050_data_ready_int_enable 		(void);
void  		mpu6050_data_ready_int_disable 		(void);


/*===============================================================================*/
/*============================MOTION DETECTION INTERRUPTS========================*/

// Interrupt bits are automatically set into the MPU-6050 circuit
/*
	Function clear INT flag
	Returns '1' if motion detection interrupts is happend.
	And return '0' if motion detection interrupts isn't happend

	Motion INT is occured when accelerometer value (any axis) exeeds the 'motion_max_value'

	You should call this function only after gpio pin iinterrupts from mpu-6050 is occured.
	And this function is not suitable for use in pin interrupts heandler
*/
uint8_t		mpu6050_is_motion_int_occurred		(void);


/*
	Configuration the detection threshold for motion interrupt generator. Thar's why
	in the begining you should make settings for interrupts from MPU-6050.

	Interrupts will indicated when value from any of the accelerometers exceeds the 'motion_max_value'.
*/
void 		mpu6050_threshold_value_int_config 	(uint8_t motion_max_value);



/*===============================================================================*/
/*===============================DATA READY INTERRUPTS===========================*/

// Interrupt bits are automatically set into the MPU-6050 circuit
/*
	Function clear INT flag
	Returns '1' if data ready interrupts is occurred.
	And return '0' if data ready interrupts isn't occurred

	You should call this function only after gpio pin iinterrupts from mpu-6050 is occured.
	And this function is not suitable for use in pin interrupts heandler
*/
uint8_t		mpu6050_is_data_ready_int_occurred	(void);


/*===============================================================================*/
/*===============================FIFO OVERFLOW INTERRUPTS========================*/

// Interrupt bits are automatically set into the MPU-6050 circuit
/*
	Function clear INT flag
	Returns '1' if fifo overflow interrupts is occurred.
	And return '0' if fifo overflow interrupts isn't occurred

	You should call this function only after gpio pin iinterrupts from mpu-6050 is occured.
	And this function is not suitable for use in pin interrupts heandler
*/
uint8_t		mpu6050_is_fifo_overflow_int_occurred	(void);





/*===============================================================================*/
/*===============================LOW LEVEL FUNCTIONS=============================*/

/*
	Hardware initialization for communication with slave - gyro and accelerometer
	Init NON blocking mode I2C
*/
void 		mpu6050_ll_dma_hardware_init 		(void);


/*
	'destination_buf' - into this buffer data will write from MPU-6050
	NON Blocking mode (DMA)
*/
void		mpu6050_ll_dma_read_nbytes			(uint8_t* destination_buf, uint8_t begin_reg_for_read, uint8_t counts_read);




/*
	Hardware initialization for communication with slave - gyro and accelerometer
	Init blocking mode I2C (without DMA)
*/
void 		mpu6050_ll_hardware_init 			(void);

/*
	Run low level I2C module for transmitting data for gyroscope or accelerometer
	Blocking mode (without DMA)
*/
void 		mpu6050_ll_write_data 				(uint8_t* data, uint16_t len);


/*
	Run low level I2C module for receiving data for gyroscope or accelerometer
	Blocking mode (without DMA)
*/
uint8_t		mpu6050_ll_read_byte				(uint8_t register_for_read);

/*
	'destination_buf' - into this buffer data will write from MPU-6050
	Blocking mode (without DMA)
*/
void		mpu6050_ll_read_nbytes				(uint8_t* destination_buf, uint8_t begin_reg_for_read, uint8_t counts_read);


#endif /*MPU6050_LL_DRIVER_H*/

