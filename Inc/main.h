
#ifndef __MAIN_H
#define __MAIN_H



//CC user button pins
/*****************************************************/
#define CC_BTN0_Pin 				GPIO_PIN_14
#define CC_BTN0_GPIO_Port 			GPIOC

#define CC_BTN1_Pin 				GPIO_PIN_15
#define CC_BTN1_GPIO_Port			GPIOC
/******************************************************/


//ADC pins
/******************************************************/
#define CC_ROTARY0_Pin 				GPIO_PIN_0
#define CC_ROTARY0_GPIO_Port 		GPIOA

#define CC_ROTARY1_Pin 				GPIO_PIN_1
#define CC_ROTARY1_GPIO_Port 		GPIOA

#define TRANSPORT_BTN_Pin 			GPIO_PIN_2
#define TRANSPORT_BTN_GPIO_Port 	GPIOA

#define BATARY_CHARGE_Pin 			GPIO_PIN_3
#define BATARY_CHARGE_GPIO_Port 	GPIOA
/******************************************************/


//input pin for motion sensor enable
#define MOTION_KEY_Pin 				GPIO_PIN_13
#define MOTION_KEY_GPIO_Port 		GPIOB

//pin goes to the HIGH state when user has inserted usb - cable
#define USB_DETECT_Pin 				GPIO_PIN_8
#define USB_DETECT_GPIO_Port 		GPIOA

//pin goes to the HIGH state when user has inserted usb - cable
#define CHARGER_DETECT_Pin 			GPIO_PIN_8
#define CHARGER_DETECT_GPIO_Port	GPIOA

//interrupt pin for detect that data from MPU-6050 is ready
#define AXYZ_INT1_Pin				GPIO_PIN_0
#define AXYZ_INT1_GPIO_Port			GPIOB

//LED pins
/******************************************************/
//led is enabled if
#define MOTION_LED_Pin				GPIO_PIN_5
#define MOTION_LED_GPIO_Port 		GPIOB

//led is enabled if
#define BAT_LED_Pin 				GPIO_PIN_7
#define BAT_LED_GPIO_Port 			GPIOB

//led is enabled if
#define HEART_LED_Pin 				GPIO_PIN_6
#define HEART_LED_GPIO_Port 		GPIOB
/******************************************************/


//pin for DETECT enable/disable power supply
#define POWER_BUTTON_Pin			GPIO_PIN_3
#define POWER_BUTTON_GPIO_Port 		GPIOB

//pin for enable/disable power supply
#define BATARY_ON_OFF_Pin			GPIO_PIN_4
#define BATARY_ON_OFF_GPIO_Port 	GPIOB

// pin for reset cc2541 chip
#define BLE_RESET_Pin				GPIO_PIN_2
#define BLE_RESET_GPIO_Port 		GPIOB

// enable/disable 1.5KOm from D+ line
#define USB_ENABLE_Pin				GPIO_PIN_9
#define USB_ENABLE_GPIO_Port		GPIOA


// enable/disable battary sensing
#define BAT_SENS_Pin 				GPIO_PIN_12
#define BAT_SENS_GPIO_Port 			GPIOB

#endif /* __MAIN_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
