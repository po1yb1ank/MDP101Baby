/**
	******************************************************************************
	* @file    queue.h
	* @author  Pribore Electronics
	* @version V0.1
	* @date    2017
	* @brief   ring buffer for midi data
	******************************************************************************
	*/



#ifndef _QUEUE_H
#define _QUEUE_H


#include <stdint.h>
#include "midi_core.h"
#include "midi_ble.h"
#include "usbd_midi.h"


#define MIDI_QUEUE_LENGHT       500




/**
* MIDI_Event_PacketTypeDef: this is the midi unit of data
* Create the array of structure from midi unit of data.
* It will be queue, which saves all musician actions
 **/
typedef struct
{
	midi_event_packet_t			midi_event[MIDI_QUEUE_LENGHT];

	uint16_t 				    head;
	uint16_t 				    tall;
	uint16_t   			        length;

}midi_queue_t;






/*************************************************************************************
				function declaration FOR WORK WITH USB QUEUE
*/
void                      midi_queue_clear             (midi_queue_t* queue);
uint16_t                  midi_queue_len               (midi_queue_t* queue);
int8_t                    is_midi_queue_empty          (midi_queue_t* queue);


int8_t 				      put_event_to_midi_queue      (midi_queue_t* queue, uint8_t* event_buf);
midi_event_packet_t*   	  get_event_from_midi_queue    (midi_queue_t* queue);
/*************************************************************************************/




#endif /*_QUEUE_H*/



