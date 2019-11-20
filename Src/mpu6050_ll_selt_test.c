

#include "mpu6050_ll_driver.h"
#include "mpu6050_ll_self_test.h"




/*===============================================================================*/
/*===============================SELF-TEST FUNCTIONS=============================*/

/*----------------------------------------------------------------*/
void mpu6050_self_test_gyro_x_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(GYRO_CONFIG);

	//
	data_to_reg[0] = GYRO_CONFIG;
	data_to_reg[1] = (tmp_reg | 0x80);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}


void mpu6050_self_test_gyro_x_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(GYRO_CONFIG);

	//
	data_to_reg[0] = GYRO_CONFIG;
	data_to_reg[1] = (tmp_reg & 0x7F);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_self_test_gyro_y_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(GYRO_CONFIG);

	//
	data_to_reg[0] = GYRO_CONFIG;
	data_to_reg[1] = (tmp_reg | 0x40);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}


void mpu6050_self_test_gyro_y_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(GYRO_CONFIG);

	//
	data_to_reg[0] = GYRO_CONFIG;
	data_to_reg[1] = (tmp_reg & 0xBF);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_self_test_gyro_z_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(GYRO_CONFIG);

	//
	data_to_reg[0] = GYRO_CONFIG;
	data_to_reg[1] = (tmp_reg | 0x20);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}


void mpu6050_self_test_gyro_z_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(GYRO_CONFIG);

	//
	data_to_reg[0] = GYRO_CONFIG;
	data_to_reg[1] = (tmp_reg & 0xDF);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/



/*----------------------------------------------------------------*/
void mpu6050_self_test_accel_x_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(ACCEL_CONFIG);

	//
	data_to_reg[0] = ACCEL_CONFIG;
	data_to_reg[1] = (tmp_reg | 0x80);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}


void mpu6050_self_test_accel_x_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(ACCEL_CONFIG);

	//
	data_to_reg[0] = ACCEL_CONFIG;
	data_to_reg[1] = (tmp_reg & 0x7F);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_self_test_accel_y_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(ACCEL_CONFIG);

	//
	data_to_reg[0] = ACCEL_CONFIG;
	data_to_reg[1] = (tmp_reg | 0x40);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}


void mpu6050_self_test_accel_y_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(ACCEL_CONFIG);

	//
	data_to_reg[0] = ACCEL_CONFIG;
	data_to_reg[1] = (tmp_reg & 0xBF);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


/*----------------------------------------------------------------*/
void mpu6050_self_test_accel_z_enable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(ACCEL_CONFIG);

	//
	data_to_reg[0] = ACCEL_CONFIG;
	data_to_reg[1] = (tmp_reg | 0x20);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}


void mpu6050_self_test_accel_z_disable (void)
{
	uint8_t tmp_reg = 0;
	uint8_t data_to_reg[2] = {0};

	//read old value
	tmp_reg = mpu6050_ll_read_byte(ACCEL_CONFIG);

	//
	data_to_reg[0] = ACCEL_CONFIG;
	data_to_reg[1] = (tmp_reg & 0xDF);

	mpu6050_ll_write_data(data_to_reg, 2);
	return;
}
/*----------------------------------------------------------------*/


























