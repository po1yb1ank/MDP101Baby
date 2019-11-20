/**
  ******************************************************************************
  * @file    midi.c
  * @author  Pribore Electronics
  * @version V0.1
  * @date    2017
  * @brief   midi protocol
  ******************************************************************************
  */

#include "midi_core.h"
#include "midi.h"
#include "queue.h"



//This is the queue of midi events
extern midi_queue_t 			      midi_event_queue;






/*-------------------------------------------------------------------*/
void midi_note_on(uint8_t channel, uint8_t note, uint8_t velocity)
{
    uint8_t temp_buf[MIDI_PACKET_LENGTH] = {0};

    temp_buf[0] = (((MIDI_CIN_NOTE_ON << 4) | (channel)) | (0x80));
    temp_buf[1] = note;
    temp_buf[2] = velocity;
    temp_buf[3] = 0x00;

    /*add midi event to usb or/and uart queue*/
    put_event_to_midi_queue(&midi_event_queue, temp_buf);
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_note_off(uint8_t channel, uint8_t note, uint8_t velocity)
{
    uint8_t temp_buf[MIDI_PACKET_LENGTH] = {0};

    temp_buf[0] = (((MIDI_CIN_NOTE_OFF << 4) | (channel)) | (0x80));
    temp_buf[1] = note;
    temp_buf[2] = velocity;
    temp_buf[3] = 0x00;

    put_event_to_midi_queue(&midi_event_queue, temp_buf);
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_all_note_off (uint8_t channel)
{
    midi_control_change(channel, CC_ALL_NOTES_OFF, 0x00);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_all_sound_off (uint8_t channel)
{
    midi_control_change(channel, CC_ALL_SOUND_OFF, 0x00);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_control_change(uint8_t channel, uint8_t num_controller, uint8_t value)
{
    uint8_t temp_buf[MIDI_PACKET_LENGTH] = {0};

    temp_buf[0] = (((MIDI_CIN_CONTROL_CHANGE << 4) | (channel)) | (0x80));
    temp_buf[1] = num_controller;
    temp_buf[2] = value;
    temp_buf[3] = 0x00;

    put_event_to_midi_queue(&midi_event_queue, temp_buf);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_control_change_fine (uint8_t channel, uint8_t num_controller, uint8_t value)
{
    uint8_t temp_buf[MIDI_PACKET_LENGTH] = {0};

    temp_buf[0] = (((MIDI_CIN_CONTROL_CHANGE << 4) | (channel)) | (0x80));
    temp_buf[1] = num_controller + CC_LSB_CONTROLLER;
    temp_buf[2] = value;
    temp_buf[3] = 0x00;

    put_event_to_midi_queue(&midi_event_queue, temp_buf);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_send_clock (void)
{
    uint8_t temp_buf[MIDI_PACKET_LENGTH] = {0};

    temp_buf[0] = MIDI_CIN_TIMING_CLOCK;
    temp_buf[1] = 0x00;
    temp_buf[2] = 0x00;
    temp_buf[3] = 0x00;

    put_event_to_midi_queue(&midi_event_queue, temp_buf);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_play (void)
{
  	midi_control_change(0, CC_UNDEFINED_9, 127);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_stop (void)
{
  	midi_control_change(0, CC_UNDEFINED_8, 127);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_continue (void)
{
    uint8_t temp_buf[MIDI_PACKET_LENGTH] = {0};

    temp_buf[0] = MIDI_CIN_CONTINUE;
    temp_buf[1] = 0x00;
    temp_buf[2] = 0x00;
    temp_buf[3] = 0x00;

    put_event_to_midi_queue(&midi_event_queue, temp_buf);

    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_rewind (void)
{
  	midi_control_change(0, CC_UNDEFINED_6, 127);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void  midi_record (void)
{
    midi_control_change(0, CC_UNDEFINED_10, 0x7F);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_loop (void)
{
    static uint8_t flag = 0;

    if(flag == 0)
    {
        midi_control_change(0, CC_UNDEFINED_5, 0x7F);
        flag = 1;
    }
    else
    {
        midi_control_change(0, CC_UNDEFINED_5, 0x00);
        flag = 0;
    }

    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_polyphonic_aftertouch (uint8_t channel, uint8_t note, uint8_t presure)
{
    uint8_t temp_buf[MIDI_PACKET_LENGTH] = {0};

    temp_buf[0] = (((MIDI_CIN_POLY_KEYPRESS << 4) | (channel)) | (0x80));
    temp_buf[1] = note;
    temp_buf[2] = presure;
    temp_buf[3] = 0x00;

    put_event_to_midi_queue(&midi_event_queue, temp_buf);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_channel_aftertouch (uint8_t channel, uint8_t presure)
{
    uint8_t temp_buf[MIDI_PACKET_LENGTH] = {0};

    temp_buf[0] = (((MIDI_CIN_CHANNEL_PRESSURE << 4) | (channel)) | (0x80));
    temp_buf[1] = presure;
    temp_buf[2] = 0x00;
    temp_buf[3] = 0x00;

    put_event_to_midi_queue(&midi_event_queue, temp_buf);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_program_change (uint8_t channel, uint8_t num_instrument)
{
    uint8_t temp_buf[MIDI_PACKET_LENGTH] = {0};

    temp_buf[0] = (((MIDI_CIN_PROGRAM_CHANGE << 4) | (channel)) | (0x80));
    temp_buf[1] = num_instrument;
    temp_buf[2] = 0x00;
    temp_buf[3] = 0x00;

    put_event_to_midi_queue(&midi_event_queue, temp_buf);
	return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_pitch_bend_fine (uint8_t channel, uint16_t pitch_data)
{
    uint8_t temp_buf[MIDI_PACKET_LENGTH] = {0};

    temp_buf[0] = (((MIDI_CIN_PITCHBEND_CHANGE << 4) | (channel)) | (0x80));

    //MSB
    temp_buf[1] = ((uint8_t)(pitch_data >> 7) & (0x7F));

    //LSB
    temp_buf[2] = (((uint8_t)(pitch_data)) & (0x7F));

    temp_buf[3] = 0x00;

    put_event_to_midi_queue(&midi_event_queue, temp_buf);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_pitch_bend_coarse (uint8_t channel, uint8_t pitch_data)
{
    uint8_t temp_buf[MIDI_PACKET_LENGTH] = {0};

    temp_buf[0] = (((MIDI_CIN_PITCHBEND_CHANGE << 4) | (channel)) | (0x80));
    temp_buf[1] = pitch_data;
    temp_buf[2] = 0x00;
    temp_buf[3] = 0x00;

    put_event_to_midi_queue(&midi_event_queue, temp_buf);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_song_select (uint8_t num_song)
{
    uint8_t temp_buf[MIDI_PACKET_LENGTH] = {0};

    temp_buf[0] = MIDI_CIN_SONG_SELECT;
    temp_buf[1] = num_song;
    temp_buf[2] = 0x00;
    temp_buf[3] = 0x00;

    put_event_to_midi_queue(&midi_event_queue, temp_buf);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_set_song_position_ptr (uint16_t midi_beat)
{
    uint8_t temp_buf[MIDI_PACKET_LENGTH] = {0};

    temp_buf[0] = MIDI_CIN_SONG_POSITION_POINTER;

    //MSB
    temp_buf[1] = ((uint8_t)(midi_beat >> 7) & (0x7F));

    //LSB
    temp_buf[2] = (((uint8_t)(midi_beat)) & (0x7F));
    temp_buf[3] = 0x00;

    put_event_to_midi_queue(&midi_event_queue, temp_buf);
    return;
}
/*-------------------------------------------------------------------*/

/*-------------------------------------------------------------------*/
void midi_send_sysex_without_param (uint8_t sub_id_1, uint8_t sub_id_2)
{
//    uint8_t temp_buf[6] = {0};
//
//    temp_buf[0] = MIDI_CIN_SYSTEM_EXCLUSIVE_BEGIN;
//    temp_buf[1] = 0x7F;
//    temp_buf[2] = DEVICE_ID;
//    temp_buf[3] = sub_id_1;
//    temp_buf[4] = sub_id_2;
//	temp_buf[5] = 0xF7;
}
/*-------------------------------------------------------------------*/


