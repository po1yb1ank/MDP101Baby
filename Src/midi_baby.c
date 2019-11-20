


#include "stm32f1xx_hal.h"
#include "main.h"
#include "midi_baby.h"
#include "software_timer.h"
#include "mpu6050_ll_driver.h"
#include "usb_device.h"
#include "usbd_midi_if.h"
#include "usbd_midi.h"
#include "adc.h"
#include "bootloader.h"
#include "usr_led_blnk.h"

extern DMA_HandleTypeDef    hdma_adc1;
extern ADC_HandleTypeDef    hadc1;
extern ADC_HandleTypeDef    hadc2;
extern TIM_HandleTypeDef    htim4;
/*
    unions with all flags
*/
extern common_flags_t		common_flags;
extern midi_flags_t         midi_flags;


/*
    Structure contain information about cuttern midi_baby state and info
    for flash memory(for pribore electronics configurator)
*/
extern midi_device_t        midi_baby_device;



/*-------------------------------------------------------------*/
void midi_baby_init(void)
{
    /*============================*/
    #if (USB_MIDI_SUPPORT == 1)
        USB_DEVICE_init();
    #endif /**/
    /*============================*/


    /*============================*/
    #if (BLE_MIDI_SUPPORT == 1)
        midi_ble_hardware_init();
    #endif /**/
    /*============================*/


    /*============================*/
    #if (UART_MIDI_IN_OUT_SUPPORT == 1)
    #endif /**/
    /*============================*/


    /*
        MIDI BABY DEVICE SETUP.
        FUTURE: READ IT FROM LAST FLASH PAGE
    */
    midi_baby_device.cc_rotary_0_ch             = DEFAULT_CHANNEL_ROTARY_0;
    midi_baby_device.cc_rotary_0_id             = DEFAULT_CC_ID_ROTARY_0;
    midi_baby_device.cc_rotary_0_val            = 0;

    midi_baby_device.cc_rotary_1_ch             = DEFAULT_CHANNEL_ROTARY_1;
    midi_baby_device.cc_rotary_1_id             = DEFAULT_CC_ID_ROTARY_1;
    midi_baby_device.cc_rotary_1_val            = 0;

    midi_baby_device.cc_left_usr_button_ch      = DEFAULT_CHANNEL_LEFT_BUTTON;
    midi_baby_device.cc_left_usr_button_id      = DEFAULT_CC_ID_LEFT_BUTTON;
    midi_baby_device.cc_left_usr_button_val     = 0x7F;

    midi_baby_device.cc_right_usr_button_ch     = DEFAULT_CHANNEL_RIGHT_BUTTON;
    midi_baby_device.cc_right_usr_button_id     = DEFAULT_CC_ID_RIGHT_BUTTON;
    midi_baby_device.cc_right_usr_button_val    = 0x7F;

    midi_baby_device.cc_rew_ch                  = DEFAULT_CHANNEL_REW;
    midi_baby_device.cc_rew_id                  = DEFAULT_CC_ID_REW;
    midi_baby_device.cc_rew_val                 = DEFAULT_CC_VAL_REW;

    midi_baby_device.cc_stop_ch                 = DEFAULT_CHANNEL_STOP;
    midi_baby_device.cc_stop_id                 = DEFAULT_CC_ID_STOP;
    midi_baby_device.cc_stop_val                = DEFAULT_CC_VAL_STOP;

    midi_baby_device.cc_play_ch                 = DEFAULT_CHANNEL_PLAY;
    midi_baby_device.cc_play_id                 = DEFAULT_CC_ID_PLAY;
    midi_baby_device.cc_play_val                = DEFAULT_CC_VAL_PLAY;

    midi_baby_device.cc_rec_ch                  = DEFAULT_CHANNEL_REC;
    midi_baby_device.cc_rec_id                  = DEFAULT_CC_ID_REC;
    midi_baby_device.cc_rec_val                 = DEFAULT_CC_VAL_REC;

    midi_baby_device.cc_loop_ch                 = DEFAULT_CHANNEL_LOOP;
    midi_baby_device.cc_loop_id                 = DEFAULT_CC_ID_LOOP;
    midi_baby_device.cc_loop_val                = DEFAULT_CC_VAL_LOOP;

    midi_baby_device.cc_gyro_x_ch               = DEFAULT_CHANNEL_GYRO_X;
    midi_baby_device.cc_gyro_x_id               = DEFAULT_CC_ID_GYRO_X;
    midi_baby_device.cc_gyro_x_val              = DEFAULT_CC_VAL_GYRO_X;

    midi_baby_device.cc_gyro_y_ch               = DEFAULT_CHANNEL_GYRO_Y;
    midi_baby_device.cc_gyro_y_id               = DEFAULT_CC_ID_GYRO_Y;
    midi_baby_device.cc_gyro_y_val              = DEFAULT_CC_VAL_GYRO_Y;

    midi_baby_device.cc_gyro_z_ch               = DEFAULT_CHANNEL_GYRO_Z;
    midi_baby_device.cc_gyro_z_id               = DEFAULT_CC_ID_GYRO_Z;
    midi_baby_device.cc_gyro_z_val              = DEFAULT_CC_VAL_GYRO_Z;

    midi_baby_device.cc_acceleromtr_x_ch        = DEFAULT_CHANNEL_ACCEL_X;
    midi_baby_device.cc_acceleromtr_x_id        = DEFAULT_CC_ID_ACCEL_X;
    midi_baby_device.cc_acceleromtr_x_val       = DEFAULT_CC_VAL_ACCEL_X;

    midi_baby_device.cc_acceleromtr_y_ch        = DEFAULT_CHANNEL_ACCEL_Y;
    midi_baby_device.cc_acceleromtr_y_id        = DEFAULT_CC_ID_ACCEL_Y;
    midi_baby_device.cc_acceleromtr_y_val       = DEFAULT_CC_VAL_ACCEL_Y;

    midi_baby_device.cc_acceleromtr_z_ch        = DEFAULT_CHANNEL_ACCEL_Z;
    midi_baby_device.cc_acceleromtr_z_id        = DEFAULT_CC_ID_ACCEL_Z;
    midi_baby_device.cc_acceleromtr_z_val       = DEFAULT_CC_VAL_ACCEL_Z;

    midi_baby_device.mpu_temprature             = 0;
    midi_baby_device.battary_charge             = 0;

    /**/
    hardware_crc_init();

    /**/
    hardware_flash_init();

    common_flags.bit.enable_x_axis = 1;
    common_flags.bit.enable_y_axis = 1;
    common_flags.bit.enable_z_axis = 0;
    return;
}
/*-------------------------------------------------------------*/






/**********************************************************/
void midi_baby_transmit_event(midi_event_packet_t* event)
{
    /*-------------------------------------------------------------*/
    #if (USB_MIDI_SUPPORT == 1)
  	{
        if (common_flags.bit.usb_connect == 1)
        {
            uint16_t i = 0;
            uint8_t  tx_usb_buf[MIDI_PACKET_LENGTH] = {0};

            tx_usb_buf[0] = event->CIN;
            tx_usb_buf[1] = ((((uint8_t)(event->CIN)) << 4) | (event->CN));
            tx_usb_buf[2] = event->MIDI_0;
            tx_usb_buf[3] = event->MIDI_1;

            while((MIDI_Transmit_FS((uint8_t *)(tx_usb_buf), MIDI_PACKET_LENGTH)) != USBD_OK)
            {
                ++i;

                /*if USB DISconnect -> RETURN*/
                if(i == 1200)
                {
                    common_flags.bit.usb_connect = 0;
                    break;
                }
            }
        }
	}
    #endif /*USB_MIDI_SUPPORT*/
    /*-------------------------------------------------------------*/



    /*-------------------------------------------------------------*/
    #if (BLE_MIDI_SUPPORT == 1)
	{
        if (common_flags.bit.ble_connect == 1)
        {

            uint8_t tx_ble_buf[MIDI_BLE_PACKET_LENGTH] = {0};

            tx_ble_buf[0] = event->timestemp_hi;
            tx_ble_buf[1] = event->timestemp_lo;
            tx_ble_buf[2] = ((((uint8_t)(event->CIN)) << 4) | (event->CN));
            tx_ble_buf[3] = event->MIDI_0;
            tx_ble_buf[4] = event->MIDI_1;
            tx_ble_buf[5] = event->MIDI_2;
            midi_ble_ll_transmit(tx_ble_buf, MIDI_BLE_PACKET_LENGTH);
/*
		  	uint8_t tx_ble_buf[4] = {0};
            tx_ble_buf[0] = ((((uint8_t)(event->CIN)) << 4) | (event->CN));
            tx_ble_buf[1] = event->MIDI_0;
            tx_ble_buf[2] = event->MIDI_1;
            tx_ble_buf[3] = event->MIDI_2;
            midi_ble_ll_transmit(tx_ble_buf, 4);
*/
        }
	}
    #endif /*BLE_MIDI_SUPPORT*/
    /*-------------------------------------------------------------*/



    /*-------------------------------------------------------------*/
    #if (UART_MIDI_IN_OUT_SUPPORT == 1)
    #endif /*UART_MIDI_IN_OUT_SUPPORT*/
    /*-------------------------------------------------------------*/

    return;
}
/**********************************************************/












/*==============================INTERFACE WITH CURCUIT================================*/
/*====================================================================================*/

/*---------------------------------------------------------------------------*/
void power_supply_disable (void)
{
  	extern void tim4_pwm_init(void);
    stop_all_processes();

    heart_led_off();
    bat_led_off();
    motion_led_off();

	HAL_GPIO_WritePin(BATARY_ON_OFF_GPIO_Port, BATARY_ON_OFF_Pin, GPIO_PIN_RESET);

	//выключить pwm
	tim4_pwm_init();
	HAL_TIM_PWM_Stop_IT(&htim4, TIM_CHANNEL_1);
	while (1);
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void power_supply_enable (void)
{
    uint16_t test = 0;
    while (1)
    {
        //if POWER BUTTON IS PRESSED
        if ((HAL_GPIO_ReadPin(POWER_BUTTON_GPIO_Port, POWER_BUTTON_Pin)) == GPIO_PIN_RESET)
        {
            HAL_Delay(1);
            ++test;

            //if user don't release the power button even time > ENABLE_DISABLE_TIME
            if (test >  ENABLE_DISABLE_TIME)
                break;
        }
        else
        {
            if (test < ENABLE_DISABLE_TIME)
                power_supply_disable();
            else
                break;
        }
    }
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void heart_led_on (void)
{
  	HAL_GPIO_WritePin(HEART_LED_GPIO_Port, HEART_LED_Pin, GPIO_PIN_SET);
  	return;
}

void heart_led_off (void)
{
  	HAL_GPIO_WritePin(HEART_LED_GPIO_Port, HEART_LED_Pin, GPIO_PIN_RESET);
  	return;
}

void heart_led_toggle (void)
{
  	HAL_GPIO_TogglePin(HEART_LED_GPIO_Port, HEART_LED_Pin);
  	return;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void motion_led_on (void)
{
  	HAL_GPIO_WritePin(MOTION_LED_GPIO_Port, MOTION_LED_Pin, GPIO_PIN_SET);
  	return;
}

void motion_led_off (void)
{
  	HAL_GPIO_WritePin(MOTION_LED_GPIO_Port, MOTION_LED_Pin, GPIO_PIN_RESET);
  	return;
}

void motion_led_toggle (void)
{
  	HAL_GPIO_TogglePin(MOTION_LED_GPIO_Port, MOTION_LED_Pin);
  	return;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void bat_led_on (void)
{
  	HAL_GPIO_WritePin(BAT_LED_GPIO_Port, BAT_LED_Pin, GPIO_PIN_SET);
  	return;
}

void bat_led_off (void)
{
  	HAL_GPIO_WritePin(BAT_LED_GPIO_Port, BAT_LED_Pin, GPIO_PIN_RESET);
  	return;
}

void bat_led_toggle (void)
{
  	HAL_GPIO_TogglePin(BAT_LED_GPIO_Port, BAT_LED_Pin);
  	return;
}
/*---------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*/
void ble_chip_reset (void)
{
    ble_chip_reset_pin_to_low();
	HAL_Delay(100);

    ble_chip_reset_pin_to_high();
    HAL_Delay(100);

	return;
}

void ble_chip_reset_pin_to_low (void)
{
    HAL_GPIO_WritePin(BLE_RESET_GPIO_Port, BLE_RESET_Pin, GPIO_PIN_RESET);
    return;
}

void ble_chip_reset_pin_to_high (void)
{
    HAL_GPIO_WritePin(BLE_RESET_GPIO_Port, BLE_RESET_Pin, GPIO_PIN_SET);
}
/*---------------------------------------------------------------------------*/





/*---------------------------------------------------------------------------*/
void usb_pin_enable (void)
{
    HAL_GPIO_WritePin(USB_ENABLE_GPIO_Port, USB_ENABLE_Pin, GPIO_PIN_SET);
    return;
}


void usb_pin_disable (void)
{
    HAL_GPIO_WritePin(USB_ENABLE_GPIO_Port, USB_ENABLE_Pin, GPIO_PIN_RESET);
    return;
}
/*---------------------------------------------------------------------------*/




/*---------------------------------------------------------------------------*/
void bat_sens_enable (void)
{
    HAL_GPIO_WritePin(BAT_SENS_GPIO_Port, BAT_SENS_Pin, GPIO_PIN_SET);
    return;
}


void bat_sens_disable (void)
{
    HAL_GPIO_WritePin(BAT_SENS_GPIO_Port, BAT_SENS_Pin, GPIO_PIN_RESET);
    return;
}
/*---------------------------------------------------------------------------*/















/*==============================Analyze new ADC data==================================*/
/*====================================================================================*/

/*variable for data from ADC*****************
adc_current_measurement[0] - rotor 0(left)    - current data
adc_current_measurement[1] - rotor 1(right)   - current data
adc_current_measurement[2] - transport buttor - current data

*/
/*current value received from ADC DMA*/
static uint16_t adc_current_measurement[ADC1_COUNT_CHANNEL]      = {0};



/*  variable for data from ADC:
    smooth_data[0] - rotor 0(left)
    smooth_data[1] - rotor 1(right)
    smooth_data[2] - transport buttor
*/
static uint8_t old_adc_data[ADC1_COUNT_CHANNEL]                  = {0};
static uint8_t smooth_data[ADC1_COUNT_CHANNEL]                   = {0};


/********************************************/
/**
    This fuction is called by system timer
*/
/*---------------------------------------------------------------------------*/
static void adc_dma_start (void)
{
    HAL_ADC_Start_DMA(&hadc1, (uint32_t* )(adc_current_measurement), ADC1_COUNT_CHANNEL);
    return;
}
/*---------------------------------------------------------------------------*/


/*Variable shows the number of process
*/
static uint8_t adc_task_heandle                             = 0;

/**
    This function create task in ONE_PULSE mode
*/
/*---------------------------------------------------------------------------*/
void midi_baby_start_adc_conversion (void)
{
    timer_t temp_process = {0};

    /*fill in info about task*/
    temp_process.mode               = ONE_PULSE;
    temp_process.cycle_period_ms    = PERIOD_ADC_CALL;
    temp_process.repeat_num         = 1;
    temp_process.func_process       = adc_dma_start;

    /*create task*/
    adc_task_heandle = create_process(&temp_process);
    return;
}
/*---------------------------------------------------------------------------*/

/**
    This function delete task
*/
/*---------------------------------------------------------------------------*/
void midi_baby_stop_adc_conversion (void)
{
    timer_t temp_process = {0};

    /*delete task*/
    temp_process.number = adc_task_heandle;
    delete_process(&temp_process);

    HAL_ADC_Stop_DMA(&hadc1);
    return;
}
/*---------------------------------------------------------------------------*/

/*data processing*/
/*---------------------------------------------------------------------------*/
void midi_baby_analyze_new_adc_data (void)
{
    /*Scaling data to 0 - 127*********************************/
    uint8_t scaling_data[ADC1_COUNT_CHANNEL] = {0};

    /*current measurement in true scale*/
    scaling_data[0] = (uint8_t)((adc_current_measurement[0] * 127)/ROTATRY_MAX_VALUE);
    scaling_data[1] = (uint8_t)((adc_current_measurement[1] * 127)/ROTATRY_MAX_VALUE);
    scaling_data[2] = (uint8_t)((adc_current_measurement[2] * 127)/ROTATRY_MAX_VALUE);
    /**********************************************************/

    /*rotary 0 (left)***********************************/
    old_adc_data[0] = smooth_data[0];

    /*new data with smoothing*/
    smooth_data[0] = ((old_adc_data[0]*(ADC_AVERAGE_FACTOR - 1) + scaling_data[0])/ADC_AVERAGE_FACTOR);

    /*magic*/
    if((scaling_data[0] > 0) && (scaling_data[0] != 127))
        ++smooth_data[0];

    if(smooth_data[0] != old_adc_data[0])
        midi_flags.bit.change_rotor_0_value = 1;
    /***************************************************/

    /*rotary 1 (right)**********************************/
    old_adc_data[1] = smooth_data[1];

    /*new data with smoothing*/
    smooth_data[1] = ((old_adc_data[1]*(ADC_AVERAGE_FACTOR - 1) + scaling_data[1])/ADC_AVERAGE_FACTOR);

    /*magic*/
    if((scaling_data[1] > 0) && (scaling_data[1] != 127))
        ++smooth_data[1];

    if(smooth_data[1] != old_adc_data[1])
        midi_flags.bit.change_rotor_1_value = 1;
    /***************************************************/

    /*TRANSPORT BUTTON**********************************/
    old_adc_data[2] = smooth_data[2];

    /*new data with smoothing*/
    smooth_data[2] = ((old_adc_data[2]*(ADC_AVERAGE_FACTOR - 1) + scaling_data[2])/ADC_AVERAGE_FACTOR);

    static uint8_t count_detect_pressed[COUNT_TRANSPORT_BUTTONS] = {0};

    /*magic*/
    if((scaling_data[2] > 0) && (scaling_data[2] != 127))
        ++smooth_data[2];

    if((smooth_data[2] != old_adc_data[2]) && \
        (smooth_data[2] != (old_adc_data[2] + 1)) && \
        (smooth_data[2] != (old_adc_data[2] + 2)) && \
        (smooth_data[2] != (old_adc_data[2] + 3)) && \
        (smooth_data[2] != (old_adc_data[2] - 1)) && \
        (smooth_data[2] != (old_adc_data[2] - 2)) && \
        (smooth_data[2] != (old_adc_data[2] - 3)))
        common_flags.bit.enable_detect_transport = 1;

    /*detect transport button if old value != new value*/
    if(common_flags.bit.enable_detect_transport == 1)
    {
        //REWIND BUTTON=================================================================================
        if((smooth_data[2] <= (REWIND_ADC_VALUE + ADC_ERROR_OFFSET)) && (smooth_data[2] >= (REWIND_ADC_VALUE - ADC_ERROR_OFFSET)))
        {
            ++count_detect_pressed[0];

            if(count_detect_pressed[0] >= ADC_SENSITIVITY)
            {
                midi_flags.bit.rew = 1;
                common_flags.bit.enable_detect_transport = 0;
                count_detect_pressed[0] = 0;
            }
        }
        else
        {
            count_detect_pressed[0] = 0;
        }
        //=============================================================================================

        //STOP BUTTON=================================================================================
        if((smooth_data[2] <= (STOP_ADC_VALUE + ADC_ERROR_OFFSET)) && (smooth_data[2] >= (STOP_ADC_VALUE - ADC_ERROR_OFFSET)))
        {
            ++count_detect_pressed[1];

            if(count_detect_pressed[1] >= ADC_SENSITIVITY)
            {
                midi_flags.bit.stop = 1;
                common_flags.bit.enable_detect_transport = 0;
                count_detect_pressed[1] = 0;
            }
        }
        else
        {
            count_detect_pressed[1] = 0;
        }
        //=============================================================================================

        //PLAY BUTTON=================================================================================
        if(smooth_data[2] <= (PLAY_ADC_VALUE + ADC_ERROR_OFFSET))
        {
            ++count_detect_pressed[2];

            if(count_detect_pressed[2] >= ADC_SENSITIVITY)
            {
                midi_flags.bit.play = 1;
                common_flags.bit.enable_detect_transport = 0;
                count_detect_pressed[2] = 0;
            }
        }
        else
        {
            count_detect_pressed[2] = 0;
        }
        //=============================================================================================

        //REC BUTTON=================================================================================
        if((smooth_data[2] <= (REC_ADC_VALUE + ADC_ERROR_OFFSET)) && (smooth_data[2] >= (REC_ADC_VALUE - ADC_ERROR_OFFSET)))
        {
            ++count_detect_pressed[3];

            if(count_detect_pressed[3] >= ADC_SENSITIVITY)
            {
                midi_flags.bit.rec = 1;
                common_flags.bit.enable_detect_transport = 0;
                count_detect_pressed[3] = 0;
            }
        }
        else
        {
            count_detect_pressed[3] = 0;
        }
        //=============================================================================================

        //LOOP BUTTON=================================================================================
        if((smooth_data[2] <= (LOOP_ADC_VALUE + ADC_ERROR_OFFSET)) && (smooth_data[2] >= (LOOP_ADC_VALUE - ADC_ERROR_OFFSET)))
        {
            ++count_detect_pressed[4];

            if(count_detect_pressed[4] >= ADC_SENSITIVITY)
            {
                midi_flags.bit.loop = 1;
                common_flags.bit.enable_detect_transport = 0;
                count_detect_pressed[4] = 0;
            }
        }
        else
        {
            count_detect_pressed[4] = 0;
        }
        //=============================================================================================
    }

    //
    midi_baby_device.cc_rotary_0_val = (uint8_t)smooth_data[0];
    midi_baby_device.cc_rotary_1_val = (uint8_t)smooth_data[1];
    return;
}
/*---------------------------------------------------------------------------*/














/*==============================Analyze Batary data===================================*/
/*====================================================================================*/
//create this task, if batary disharged
extern void batary_is_discharged (void);
uint8_t batary_discharge_process_heandle = MAX_COUNT_SW_TIM + 2;
/*
*/
void midi_baby_analyze_new_batary_data (void)
{
    static uint32_t counts_detect_that_bat_discharge = 0;
    uint32_t tmp = HAL_ADC_GetValue(&hadc2);

    //get value in mili-volt
    tmp = ((tmp * VDD_VALUE)/4095);

    //
    if (common_flags.bit.charger_insert == 0)
    {
        if (tmp >= MAX_BATARY_VOLTAGE)
            midi_baby_device.battary_charge = 100;//100%
        else
            midi_baby_device.battary_charge = (100 - ((uint8_t)(((MAX_BATARY_VOLTAGE - tmp) * 100)/(MAX_BATARY_VOLTAGE - MIN_BATARY_VOLTAGE))));

        //
        if (midi_baby_device.battary_charge <= PERSENT_WHEN_DEVICE_DISCHARGED)
        {
            //если кабель зарядки не вставлен. То есть девайс продолжает разряжаться
            //и если это не первое прерывание
            if (common_flags.bit.charger_insert == 0)
            {
                if (((counts_detect_that_bat_discharge == 1) && (common_flags.bit.batary_discharged == 0)) || \
                    (common_flags.bit.first_batary_measurement == 1))
                {
                    //create task for toggle batary led
                    timer_t temp_process = {0};

                    temp_process.mode                   = CYCLE;
                    temp_process.cycle_period_ms        = PERIOD_TOGGLE_BAT_LED_WHEN_DISCHARGE;
                    temp_process.func_process           = batary_is_discharged;
                    batary_discharge_process_heandle    = create_process(&temp_process);

                    //set flag that device batary discharged
                    //it means that toggle led process is created!!!
                    //ФЛАГ, ЧТО ДЕВАЙСУ НУЖНА ЗАРЯДКА
                    common_flags.bit.batary_discharged = 1;

                    //сбросить, чтобы данный процесс не создавался заново
                    counts_detect_that_bat_discharge = 0;
                }
                ++counts_detect_that_bat_discharge;
            }
            else
                counts_detect_that_bat_discharge = 0;
        }
        else
            counts_detect_that_bat_discharge = 0;
    }
    //если провод зарядки вставлен
    else
    {
        //tmp - значение напряжения в миливольтах
        //если девайсу в данный момент нужна зарядка
        //то добавляем к tmp (значение в миливольтах) добавочное напряжение, которое появляется, когда вставляю usb провод
        //и проводим измерения уже с учетом добавочных данных
        if (common_flags.bit.batary_discharged == 1)
        {
            //только если два раза подряд мы убедились, что девайс заряжен - сообщаем об этом пользователю
            static uint32_t count_detect_that_device_is_charging = 0;

            //если обнаружили, что девайс заряжен
            if (((tmp >= (MAX_BATARY_VOLTAGE + ADDITIONAL_VOLTAGE_WHEN_USB_INSERTED))) || \
                 ((MAX_BATARY_VOLTAGE - tmp) >= ((MAX_BATARY_VOLTAGE + ADDITIONAL_VOLTAGE_WHEN_USB_INSERTED) - MIN_BATARY_VOLTAGE)))
            {
                //если на второй раз обнаружили, что девайс уже заряжен
                if (count_detect_that_device_is_charging == 1)
                {
                    //БАТАРЕЙКА ЗАРЯЖЕНА
                    common_flags.bit.batary_discharged  = 0;
                    common_flags.bit.batary_charging_finished = 1;

                    //enable bat_led_pwm
                }
                ++count_detect_that_device_is_charging;
            }
            else
                count_detect_that_device_is_charging = 0;
        }
    }
    bat_sens_disable();
    return;
}












/*==============================Analyze new gyro/accelerometer data===================*/
/*====================================================================================*/

/*
    motion_mpu6050_data[0-1] - accel x
    motion_mpu6050_data[2-3] - accel y
    motion_mpu6050_data[4-5] - accel z

    motion_mpu6050_data[6-7] - tempreture

    motion_mpu6050_data[8-9] = gyro x
    motion_mpu6050_data[10-11] = gyro y
    motion_mpu6050_data[12-13] = gyro z
*/
//
static uint8_t     motion_mpu6050_data[14] = {0};



/*---------------------------------------------------------------------------*/
void midi_baby_start_read_motion (void)
{
    mpu6050_ll_dma_read_nbytes(motion_mpu6050_data, ACCEL_XOUT_H, 14);
    return;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void midi_baby_analyze_new_motion_data (void)
{
    static uint32_t    old_accel_x_g_value = 0;
    static uint32_t    old_accel_y_g_value = 0;
    //static uint32_t    old_accel_z_g_value = 0;

    int16_t            current_accel_x_value = 0;
    int16_t            current_accel_y_value = 0;
    //int16_t            current_accel_z_value = 0;

    uint32_t           average_accel_x_value = 0;
    uint32_t           average_accel_y_value = 0;
    //uint32_t           average_accel_z_value = 0;

    //Calculate x acceleration
    /*-------------------------------------------------------------------------------------------------------------*/
    //get current value from MPU register
    current_accel_x_value = (int16_t)((((uint16_t)motion_mpu6050_data[0]) << 8) | motion_mpu6050_data[1]);

    //get scalling data
    if (current_accel_x_value <= -16384) {current_accel_x_value = -16350;}
    if (current_accel_x_value >=  16384) {current_accel_x_value =  16350;}

    //define rotate direction
    if (current_accel_x_value <= -200) //right rotate
    {
        //delete minus
        current_accel_x_value = (-1) * current_accel_x_value;

        //scaling data and average
        uint8_t accel_x_cc = (uint8_t)((uint8_t)((float)((current_accel_x_value) * 65)/16000) + 64);
        average_accel_x_value = (uint8_t)(((float)(GAIN_FILTER*accel_x_cc)) + ((float)((1 - GAIN_FILTER)*old_accel_x_g_value)));

        if (midi_baby_device.cc_acceleromtr_x_val != average_accel_x_value)
        {
            midi_baby_device.cc_acceleromtr_x_val = average_accel_x_value;

            if ((common_flags.bit.motion_button_pressed == 1) && (common_flags.bit.enable_x_axis == 1))
                midi_baby_accel_x_changed();
        }
        old_accel_x_g_value = average_accel_x_value;
    }
    else if (current_accel_x_value >= 200) //left rotate
    {
        //scaling data and average
        uint8_t accel_x_cc = (uint8_t)((uint8_t)(64 - (float)((current_accel_x_value) * 63)/16000));
        average_accel_x_value = (uint8_t)(((float)(GAIN_FILTER*accel_x_cc)) + ((float)((1 - GAIN_FILTER)*old_accel_x_g_value)));

        if (midi_baby_device.cc_acceleromtr_x_val != average_accel_x_value)
        {
            midi_baby_device.cc_acceleromtr_x_val = average_accel_x_value;

            if ((common_flags.bit.motion_button_pressed == 1) && (common_flags.bit.enable_x_axis == 1))
                midi_baby_accel_x_changed();
        }
        old_accel_x_g_value = average_accel_x_value;
    }
    /*-------------------------------------------------------------------------------------------------------------*/


    //Calculate y acceleration
    /*-------------------------------------------------------------------------------------------------------------*/
    //get current value from MPU register
    current_accel_y_value = (int16_t)((((uint16_t)motion_mpu6050_data[2]) << 8) | motion_mpu6050_data[3]);

    //get scalling data
    if (current_accel_y_value <= -15384) {current_accel_y_value = -15350;}
    if (current_accel_y_value >=  15384) {current_accel_y_value =  15350;}

    //define rotate direction
    if (current_accel_y_value >= 250) //up rotate
    {
        //scaling data and average
        uint8_t accel_y_cc = (uint8_t)((uint8_t)((float)((current_accel_y_value) * 64)/15350) + 64);
        average_accel_y_value = (uint8_t)(((float)(GAIN_FILTER*accel_y_cc)) + ((float)((1 - GAIN_FILTER)*old_accel_y_g_value)));

        if (midi_baby_device.cc_acceleromtr_y_val != average_accel_y_value)
        {
            midi_baby_device.cc_acceleromtr_y_val = average_accel_y_value;

            if ((common_flags.bit.motion_button_pressed == 1) && (common_flags.bit.enable_y_axis == 1))
                midi_baby_accel_y_changed();
        }
        old_accel_y_g_value = average_accel_y_value;
    }
    else if (current_accel_y_value < -250) //down rotate
    {
        //delete minus
        current_accel_y_value = (-1) * current_accel_y_value;
        //scaling data and average
        uint8_t accel_y_cc = (uint8_t)((uint8_t)(64 - (float)((current_accel_y_value) * 64)/15350));
        average_accel_y_value = (uint8_t)(((float)(GAIN_FILTER*accel_y_cc)) + ((float)((1 - GAIN_FILTER)*old_accel_y_g_value)));

        if (midi_baby_device.cc_acceleromtr_y_val != average_accel_y_value)
        {
            midi_baby_device.cc_acceleromtr_y_val = average_accel_y_value;

            if ((common_flags.bit.motion_button_pressed == 1) && (common_flags.bit.enable_y_axis == 1))
                midi_baby_accel_y_changed();
        }
        old_accel_y_g_value = average_accel_y_value;
    }
    /*-------------------------------------------------------------------------------------------------------------*/


    //Calculate z acceleration
    /*-------------------------------------------------------------------------------------------------------------*/
    /*-------------------------------------------------------------------------------------------------------------*/



    //Calculate angle
    /*-------------------------------------------------------------------------------------------------------------*/
    //int16_t gyro_x_reg = (int16_t)((((uint16_t)motion_mpu6050_data[8]) << 8) | motion_mpu6050_data[9]);
    //int16_t gyro_y_reg = (int16_t)((((uint16_t)motion_mpu6050_data[10]) << 8) | motion_mpu6050_data[11]);
    //int16_t gyro_z_reg = (int16_t)((((uint16_t)motion_mpu6050_data[12]) << 8) | motion_mpu6050_data[13]);
    /*-------------------------------------------------------------------------------------------------------------*/


    //Calculate temperature
    /*-------------------------------------------------------------------------------------------------------------*/
    uint16_t temperature = ((int16_t)((((uint16_t)motion_mpu6050_data[6]) << 8) | motion_mpu6050_data[7])/340) + 36;
	midi_baby_device.mpu_temprature = (uint8_t)temperature;
    /*-------------------------------------------------------------------------------------------------------------*/

    return;
}
/*---------------------------------------------------------------------------*/















/*==============================Reaction to changes device parameters=================*/
/*====================================================================================*/

/*---------------------------------------------------------------------------*/
void midi_baby_rotary_0_changed (void)
{
    midi_control_change(midi_baby_device.cc_rotary_0_ch, midi_baby_device.cc_rotary_0_id, midi_baby_device.cc_rotary_0_val);
    return;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void midi_baby_rotary_1_changed (void)
{
    midi_control_change(midi_baby_device.cc_rotary_1_ch, midi_baby_device.cc_rotary_1_id, midi_baby_device.cc_rotary_1_val);
    return;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void midi_baby_stop_but_changed (void)
{
    midi_control_change(midi_baby_device.cc_stop_ch, midi_baby_device.cc_stop_id, midi_baby_device.cc_stop_val);
    return;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void midi_baby_play_but_changed (void)
{
    midi_control_change(midi_baby_device.cc_play_ch, midi_baby_device.cc_play_id, midi_baby_device.cc_play_val);
    return;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void midi_baby_rewind_but_changed (void)
{
    midi_control_change(midi_baby_device.cc_rew_ch, midi_baby_device.cc_rew_id, midi_baby_device.cc_rew_val);
    return;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void midi_baby_rec_but_changed (void)
{
    midi_control_change(midi_baby_device.cc_rec_ch, midi_baby_device.cc_rec_id, midi_baby_device.cc_rec_val);
    return;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void midi_baby_loop_but_changed (void)
{
    static uint8_t flag = 0;

    if (flag == 0)
    {
        midi_control_change(midi_baby_device.cc_loop_ch, midi_baby_device.cc_loop_id, midi_baby_device.cc_loop_val);
        midi_baby_device.cc_loop_val = 0x00;

        flag = 1;
    }
    else
    {
        midi_control_change(midi_baby_device.cc_loop_ch, midi_baby_device.cc_loop_id, midi_baby_device.cc_loop_val);
        midi_baby_device.cc_loop_val = 0x7F;

        flag = 0;
    }

    return;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void midi_baby_accel_x_changed (void)
{
    midi_control_change(midi_baby_device.cc_acceleromtr_x_ch, midi_baby_device.cc_acceleromtr_x_id, midi_baby_device.cc_acceleromtr_x_val);
    return;
}
/*---------------------------------------------------------------------------*/



/*---------------------------------------------------------------------------*/
void midi_baby_accel_y_changed (void)
{
    midi_control_change(midi_baby_device.cc_acceleromtr_y_ch, midi_baby_device.cc_acceleromtr_y_id, midi_baby_device.cc_acceleromtr_y_val);
    return;
}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
void midi_baby_accel_z_changed (void)
{
    midi_control_change(midi_baby_device.cc_acceleromtr_z_ch, midi_baby_device.cc_acceleromtr_z_id, midi_baby_device.cc_acceleromtr_z_val);
    return;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void midi_baby_gyro_x_changed (void)
{
    midi_control_change(midi_baby_device.cc_gyro_x_ch, midi_baby_device.cc_gyro_x_id, midi_baby_device.cc_gyro_x_val);
    return;
}
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
void midi_baby_gyro_y_changed (void)
{
    midi_control_change(midi_baby_device.cc_gyro_y_ch, midi_baby_device.cc_gyro_y_id, midi_baby_device.cc_gyro_y_val);
    return;
}
/*---------------------------------------------------------------------------*/


/*---------------------------------------------------------------------------*/
void midi_baby_gyro_z_changed (void)
{
    midi_control_change(midi_baby_device.cc_gyro_z_ch, midi_baby_device.cc_gyro_z_id, midi_baby_device.cc_gyro_z_val);
    return;
}
/*---------------------------------------------------------------------------*/
