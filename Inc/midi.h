/**
  ******************************************************************************
  * @file    midi.h
  * @author  Pribore Electronics
  * @version V0.1
  * @date    2017
  * @brief   midi protocol
  ******************************************************************************
  */


#ifndef MIDI_H
#define MIDI_H




#include "queue.h"


/*
	When user pressed note button on midi keyboard,
	this function is called.
	The Note On message also includes data bytes for the key number
	and for the velocity with which the key was released.

*/
void 	midi_note_on					(uint8_t channel, uint8_t note, uint8_t velocity);//++

/*
	When user released note button on midi keyboard,
	this function is called.
	The Note Off message also includes data bytes for the key number
	and for the velocity with which the key was released.
*/
void 	midi_note_off					(uint8_t channel, uint8_t note, uint8_t velocity);//++
void 	midi_all_note_off				(uint8_t channel);//++

/*
	Turn off all controllers for note.
*/
void 	midi_all_sound_off				(uint8_t channel);

/*
	If midi keyboard has pressure sensor, that information about pressure wiil send with help
	this function. This pressure information, commonly called "aftertouch".
*/
//use for NOTE. This command use if there is sensors for each button
void 	midi_polyphonic_aftertouch		(uint8_t channel, uint8_t note, uint8_t presure);//++

//use for CHANNEL. Use if midi keyboard has onle one pressure sensor
void 	midi_channel_aftertouch			(uint8_t channel, uint8_t presure);//++

/*
	MIDI Control Change messages are used to control a wide variety of functions in a synthesizer.
	Control Change messages, like other MIDI Channel messages, should only affect the Channel number indicated in the
	status byte. The Control Change status byte is followed by one data byte indicating the "controller number",
	and a second byte which specifies the "control value". The controller number identifies which function of the
	synthesizer is to be controlled by the message.
*/
void 	midi_control_change				(uint8_t channel, uint8_t num_controller, uint8_t value);//++
void 	midi_control_change_fine		(uint8_t channel, uint8_t num_controller, uint8_t value);//++


/*
	This commant used that change instrument, whitch should play on channel
	All instruments: http://www.electronics.dit.ie/staff/tscarff/Music_technology/midi/program_change.htm
*/
void 	midi_program_change				(uint8_t channel, uint8_t num_instrument);//++



/*
	The Pitch Bend Change message is normally sent
	from a keyboard instrument in response to changes in position of the pitch bend wheel.
	The pitch bend information is used to modify the pitch of sounds.(сдвиг частоты/высоты тона)


    2^14 = 16384 (with zero)
    and because 7 bit in status byte must be logic '1', that receiver understend that it is STATUS byte
    But MIDI data byte - 7 bit MUST be logic '0'

    MUST HAVE: pitch_data < 16383
*/
void 	midi_pitch_bend_fine			(uint8_t channel, uint16_t pitch_data);//++
void 	midi_pitch_bend_coarse			(uint8_t channel, uint8_t pitch_data);//++


/*
	System Real Time message*******************************************************************************
*/
void 	midi_send_clock					(void);//++
void 	midi_play						(void);//++
void  	midi_stop						(void);//++
void    midi_continue 					(void);//++
/*******************************************************************************************************/



/*
	System Common message*******************************************************************************
*/

void 	midi_song_select				(uint8_t num_song);//++

/*
	One midi_beat = * timing clock
	MUST HAVE:  midi_beat < 16383 
*/
void 	midi_set_song_position_ptr		(uint16_t midi_beat);//++
/*******************************************************************************************************/



/*
	System Exclusive message*******************************************************************************
	This message don't put in usb/ble queues, because queues have fixed data lengt (MIDI_PACKET_LENGTH)
	Thats way this message transmit instantly(мгновенно)
*/
/*	FORMAT: "F0 7F <Device-ID> <Sub-ID#1> [<Sub-ID#2> [<parameters>]] F7"
	Device-ID - value 00-7F (7F = all devices)  -
	Sub-ID#1  - 	01 = Long Form MTC
					02 = MIDI Show Control
					03 = Notation Information
					04 = Device Control
					05 = Real Time MTC Cueing
					06 = MIDI Machine Control Command
					07 = MIDI Machine Control Response
					08 = Single Note Retune
	If Sub-ID#1 = 06 (rewind, loop, play, stop, pause), тогда
	Sub-ID#2 -		01 Stop
					02 Play
					03 Deferred Play (play after no longer busy)
					04 Fast Forward
					05 Rewind
					06 Record Strobe (AKA [[Punch in/out|Punch In]])
					07 Record Exit (AKA [[Punch out (music)|Punch out]])
					08 Record Pause
					09 Pause (pause playback)
					0A Eject (disengage media container from MMC device)
					0B Chase
					0D MMC Reset (to default/startup state)
					40 Write (AKA Record Ready, AKA Arm Tracks)
					    parameters: <length1> 4F <length2> <track-bitmap-bytes>
					44 Goto (AKA Locate)
					    parameters: <length>=06 01 <hours> <minutes> <seconds> <frames> <subframes>
					47 Shuttle
					    parameters: <length>=03 <sh> <sm> <sl> (MIDI Standard Speed codes)
*/
void 	midi_send_sysex_without_param	(uint8_t sub_id_1, uint8_t sub_id_2);




void 	midi_rewind						(void);
void 	midi_record						(void);
void 	midi_loop						(void);
/*******************************************************************************************************/



#endif /*MIDI_H*/




