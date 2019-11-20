#ifndef MPU6050_LL_SELF_TEST_H
#define MPU6050_LL_SELF_TEST_H










/*===============================================================================*/
/*===============================SELF-TEST FUNCTIONS=============================*/
/*===============================================================================*/
/*
	Self-test enable/disable dor GYRO
*/
void 			mpu6050_self_test_gyro_x_enable		(void);
void 			mpu6050_self_test_gyro_x_disable	(void);

void 			mpu6050_self_test_gyro_y_enable		(void);
void 			mpu6050_self_test_gyro_y_disable	(void);

void 			mpu6050_self_test_gyro_z_enable		(void);
void 			mpu6050_self_test_gyro_z_disable	(void);


/*
	Self-test enable/disable dor ACCEL
*/
void 			mpu6050_self_test_accel_x_enable	(void);
void 			mpu6050_self_test_accel_x_disable	(void);

void 			mpu6050_self_test_accel_y_enable	(void);
void 			mpu6050_self_test_accel_y_disable	(void);

void 			mpu6050_self_test_accel_z_enable	(void);
void 			mpu6050_self_test_accel_z_disable	(void);






















#endif /*MPU6050_LL_SELF_TEST_H*/

