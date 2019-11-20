/**
  ******************************************************************************
  * @file    midi_ble.h
  * @author  Pribore Electronics
  * @version V0.1
  * @date    2017
  * @brief   midi ble protocol header
  ******************************************************************************
  */


#ifndef _MIDI_BLE
#define _MIDI_BLE

#include <stdint.h>



void 		midi_ble_hardware_init 			(void);
void 		midi_ble_timestamp_inc		    (void);
void        midi_ble_ll_transmit            (uint8_t* midi_data, uint16_t len);
void 		midi_ble_parce_data  			(void);


#endif /*_MIDI_BLE*/























