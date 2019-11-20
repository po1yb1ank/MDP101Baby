/**
  ******************************************************************************
  * @file    queue.c
  * @author  Pribore Electronics
  * @version V0.1
  * @date    2017
  * @brief   ring buffer for midi data
  ******************************************************************************
  */

#include "queue.h"

extern uint16_t midi_timestamp;

/**
* queue_clear: Aligns head and tail
**/
/********************************************/
void midi_queue_clear(midi_queue_t* queue)
{
    //initialization queue
    queue->head = 0;
    queue->tall = 0;
    queue->length = 0;

    return;
}
/********************************************/








/**
* queue_len:	returns current length of queue
*
**/
/********************************************/
uint16_t midi_queue_len(midi_queue_t* queue)
{
    return queue->length;
}
/********************************************/






/**
* is_queue_empty:	returns "0" if queue is empty. It's OK
*					returns "-1" if queue isn't empty(head != tall)
**/
/********************************************/
int8_t is_midi_queue_empty(midi_queue_t* queue)
{
    if(queue->head == queue->tall)
		return 0;
	else
	  return -1;
}
/********************************************/




/**
* put_event_to_queue(where_put, what_put):	returns "0" if all OK
*						returns "-1" if queue is full
**/
/********************************************/
int8_t put_event_to_midi_queue(midi_queue_t* queue, uint8_t* event_buf)
{
    if(midi_queue_len(queue) >= MIDI_QUEUE_LENGHT - 1)
      return -1;

    //timestamp
    queue->midi_event[queue->tall].timestemp_hi = (uint8_t)(0x80 | (midi_timestamp >> 7));
    queue->midi_event[queue->tall].timestemp_lo = (uint8_t)(0x80 | ((uint8_t)((0x7F) & (midi_timestamp))));

    //status byte
    queue->midi_event[queue->tall].CIN = (((event_buf[0]) & 0xF0) >> 4);
    queue->midi_event[queue->tall].CN = ((event_buf[0]) & 0x0F);

    //midi message 1
    queue->midi_event[queue->tall].MIDI_0 = event_buf[1];

    //midi message 2
    queue->midi_event[queue->tall].MIDI_1 = event_buf[2];

    //midi message 3
    queue->midi_event[queue->tall].MIDI_2 = event_buf[3];

    //increase the head pointer and common lenght
    ++queue->tall;

    ++queue->length;

    //закольцовка буфера
    if(queue->tall == MIDI_QUEUE_LENGHT)
      queue->tall = 0;

    return 0;
}
/********************************************/



/**
* returns nearest midi-event
**/
/********************************************/
midi_event_packet_t* get_event_from_midi_queue(midi_queue_t* queue)
{
    --queue->length;

    midi_event_packet_t* event = &queue->midi_event[queue->head++];

    //закольцовка буфера
    if(queue->head == MIDI_QUEUE_LENGHT)
      queue->head = 0;

    return event;
}
/********************************************/


