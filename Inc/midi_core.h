/**
  ******************************************************************************
  * @file    midi_kernel.h
  * @author  Pribore Electronics
  * @version V0.1
  * @date    2017
  * @brief   General defines for the usb midi interface, GM standart.
  ******************************************************************************
  */

#ifndef _MIDI_KERNEL_
#define _MIDI_KERNEL_

#include <stdint.h>




/*It is whole midi packet length */
#define MIDI_PACKET_LENGTH 		     (uint8_t)(4)
#define MIDI_BLE_PACKET_LENGTH     	 (uint8_t)(6)




/*
    Defines for logic message
*/
#define CC_ON                       (uint8_t)(127)
#define CC_OFF                      (uint8_t)(0)





/*****************All possible channels************************************************/
#define CHANNEL_1 	0x00
#define CHANNEL_2 	0x01
#define CHANNEL_3 	0x02
#define CHANNEL_4 	0x03
#define CHANNEL_5 	0x04
#define CHANNEL_6 	0x05
#define CHANNEL_7 	0x06
#define CHANNEL_8 	0x07
#define CHANNEL_9 	0x08
#define CHANNEL_10 	0x09
#define CHANNEL_11 	0x0A
#define CHANNEL_12 	0x0B
#define CHANNEL_13 	0x0C
#define CHANNEL_14 	0x0D
#define CHANNEL_15 	0x0E
#define CHANNEL_16 	0x0F
/**************************************************************************************/



/*
    MIDI message can be either a channel or system message
    The channel message can be addressed to one of sixteen
    channels whereas a system message is addressed to all channels.
    The channel message can operate either on channel voices or on channel modes.
    The system messages provide real-time, common or exclusive control of the MIDI equipment.
*/



/****************************MIDI CHANNEL MESSAGE CODE********************************/
#define MIDI_CIN_NOTE_OFF				     (uint8_t)(0x08)//++
#define MIDI_CIN_NOTE_ON				     (uint8_t)(0x09)//++
#define MIDI_CIN_POLY_KEYPRESS			     (uint8_t)(0x0A)//++
#define MIDI_CIN_CONTROL_CHANGE			     (uint8_t)(0x0B)//++
#define MIDI_CIN_PROGRAM_CHANGE			     (uint8_t)(0x0C)//++
#define MIDI_CIN_CHANNEL_PRESSURE		     (uint8_t)(0x0D)
#define MIDI_CIN_PITCHBEND_CHANGE		     (uint8_t)(0x0E)//++
/**************************************************************************************/







/****************************MIDI SYSTEM MESSAGE CODE********************************/

//System Exclusive*********************************************************************************************************************/

//начало SysEx команды
#define MIDI_CIN_SYSTEM_EXCLUSIVE_BEGIN     (uint8_t)(0xF0)

//конец SysEx команды. Может и не посылаться. Любой другой статус байт будет означать конец SysEx
#define MIDI_CIN_SYSTEM_ENCLUSIVE_END       (uint8_t)(0xF7)
/**********************************************************************************************************************************/

//System Common*********************************************************************************************************************/

//синхронизация секвенсоров с магнитофонами
#define MIDI_CIN_TIME_CODE                  (uint8_t)(0xF1)

//с этой командой можно начать воспроизведение с любого места. В байте данных посылаются
//число бит(Один MIDI-бит равен шести клокам), прошедших от начала песни. Сообщение содержит пару MSB:LSB
//передаётся при перемотке назад/вперёд при остановленном воспроизведении
#define MIDI_CIN_SONG_POSITION_POINTER      (uint8_t)(0xF2)

//выбор песни или паттерна
#define MIDI_CIN_SONG_SELECT                (uint8_t)(0xF3)

//игнорируется
#define MIDI_CIN_SYSTEM_UNDEFINED_1         (uint8_t)(0xF4)

//игнорируется
#define MIDI_CIN_SYSTEM_UNDEFINED_2         (uint8_t)(0xF5)

//used for old analog synthesizer for preset sound generator
#define MIDI_CIN_SONG_TUNE_REQUEST          (uint8_t)(0xF6)
/**********************************************************************************************************************************/



//System Real Time*****************************************************************************************************************/

//MIDI clock. синхронизация двух устройств по скорости
#define MIDI_CIN_TIMING_CLOCK               (uint8_t)(0xF8)

//к ведомому устройству каждые 10 мс. Это вместо TIMING_CLOCK. Тогда ведомый должен считать сам по своим внутренним часам
#define MIDI_CIN_TIMING_TICK                (uint8_t)(0xF9)

//приводит устройство в режим готовности воспроизведения и указывает на то, что воспроизводить надо СНАЧАЛА песни
#define MIDI_CIN_START                      (uint8_t)(0xFA)

//старт устройства с точки, заданной MIDI_CIN_SONG_POSITION_POINTER или от последней точки остановки.
//воспроизведение начинается с приходом следующего клока (внутреннего у управляемого устроства или внешнего к управляемому устройству)
#define MIDI_CIN_CONTINUE                   (uint8_t)(0xFB)

//останавливает воспроизведение на  текущем месте
#define MIDI_CIN_STOP                       (uint8_t)(0xFC)

//игнорируется
#define MIDI_CIN_SYSTEM_UNDEFINED_3         (uint8_t)(0xFD)

//используется при остановке MIDI потока.   (Используется НЕ контроллером)
//Если после посылке синтом этого сообщения нет ответа больше чем 330 мс, то считается что провод выдернут и синт
//выключает всеноты и контроллеры
//Девайся должен ответить (вроде как) такой же командой
#define MIDI_CIN_ACTIVE_SENSING             (uint8_t)(0xFE)


//перевод синта/DAW в первоначальное состояние(все контроллеры = 0, ноты выключены и т.д.)
#define MIDI_CIN_SYSTEM_RESET               (uint8_t)(0xFF)
/**********************************************************************************************************************************/


/**************************************************************************************/









/**********************DEFINE FOR MIDI CONTROLLERS (CC#)*******************************/


/*
    This value(0 - 63) are continuous. From 0 to 127
*/
/***************************************************************************/
#define CC_BANK_SELECT                    (uint8_t)(0)
#define CC_MODULATION_WHEEL               (uint8_t)(1)
#define CC_BREATH                         (uint8_t)(2)
#define CC_UNDEFINED_1                    (uint8_t)(3)
#define CC_FOOT_CONTROLLER                (uint8_t)(4)
#define CC_PORTAMENTO_TIME                (uint8_t)(5)
#define CC_DATA_ENTRY                     (uint8_t)(6)
#define CC_CHANNEL_VOLUME                 (uint8_t)(7)
#define CC_BALANCE_CONTROL                (uint8_t)(8)
#define CC_UNDEFINED_2                    (uint8_t)(9)
#define CC_PAN                            (uint8_t)(10)
#define CC_EXPRESSION                     (uint8_t)(11)
#define CC_EFFECT_1                       (uint8_t)(12)
#define CC_EFFECT_2                       (uint8_t)(13)
#define CC_UNDEFINED_3                    (uint8_t)(14)
#define CC_UNDEFINED_4                    (uint8_t)(15)
#define CC_GENERAL_1                      (uint8_t)(16)
#define CC_GENERAL_2                      (uint8_t)(17)
#define CC_GENERAL_3                      (uint8_t)(18)
#define CC_GENERAL_4                      (uint8_t)(19)
#define CC_UNDEFINED_5                    (uint8_t)(20)//СС 20 LOOP - 7F++
#define CC_UNDEFINED_6                    (uint8_t)(21)//СС 21 E-MU REWIND 7F , КАНАЛ 1++
#define CC_UNDEFINED_7                    (uint8_t)(22)
#define CC_UNDEFINED_8                    (uint8_t)(23)//СС 23 E-MU STOP 7F , КАНАЛ 1++
#define CC_UNDEFINED_9                    (uint8_t)(24)//СС 24 E-MU PLAY 7F , КАНАЛ 1++
#define CC_UNDEFINED_10                   (uint8_t)(25)//СС 25 REC 7F , КАНАЛ 1
#define CC_UNDEFINED_11                   (uint8_t)(26)
#define CC_UNDEFINED_12                   (uint8_t)(27)
#define CC_UNDEFINED_13                   (uint8_t)(28)
#define CC_UNDEFINED_14                   (uint8_t)(29)
#define CC_UNDEFINED_15                   (uint8_t)(30)
#define CC_UNDEFINED_16                   (uint8_t)(31)
/*
    0 - 31, + 32 = LSB for 0 - 31 controllers
    То есть если нужна более точная настройка для конкретного СС из списка выше, то
    присылается ещё одно MIDI СС сообщение с номером контроллера увеличенного на CC_LSB_CONTROLLER
*/
#define CC_LSB_CONTROLLER                 (uint8_t)(32)
/***************************************************************************/

/*
    Logic message!!!
    0   - OFF
    127 - ON
*/
/***************************************************************************/
#define CC_DAMPER_PEDAL                   (uint8_t)(64)
#define CC_PORTAMENTO_SWITCH              (uint8_t)(65)
#define CC_SUSTANUTO                      (uint8_t)(66)
#define CC_SOFT_PEDAL                     (uint8_t)(67)
#define CC_LEGATO                         (uint8_t)(68)
#define CC_HOLD_2_PEDAL                   (uint8_t)(69)
/***************************************************************************/

/*
    Sound controllers
    +ADSR  time value
    and value cutoff for VCF
    This value(70- 95) are continuous. From 0 to 127
*/
#define CC_SOUND_VARIATION                (uint8_t)(70)
#define CC_HARMONICS                      (uint8_t)(71)
#define CC_RELEASE_TIME                   (uint8_t)(72)
#define CC_ATTACK                         (uint8_t)(73)
#define CC_FREQ_CUTOFF                    (uint8_t)(74)
#define CC_GENERETIC_1                    (uint8_t)(75)
#define CC_GENERETIC_2                    (uint8_t)(76)
#define CC_GENERETIC_3                    (uint8_t)(77)
#define CC_GENERETIC_4                    (uint8_t)(78)
#define CC_GENERETIC_5                    (uint8_t)(79)

#define CC_GENERAL_5                      (uint8_t)(80)//off = 0-63, on = 64 - 127
#define CC_GENERAL_6                      (uint8_t)(81)//off = 0-63, on = 64 - 127
#define CC_GENERAL_7                      (uint8_t)(82)//off = 0-63, on = 64 - 127
#define CC_GENERAL_8                      (uint8_t)(83)//off = 0-63, on = 64 - 127

#define CC_PORTAMENTO_CONTROL             (uint8_t)(84)


#define CC_UNDEFINED_17                   (uint8_t)(85)
#define CC_UNDEFINED_18                   (uint8_t)(86)
#define CC_UNDEFINED_19                   (uint8_t)(87)
#define CC_UNDEFINED_20                   (uint8_t)(88)
#define CC_UNDEFINED_21                   (uint8_t)(89)
#define CC_UNDEFINED_22                   (uint8_t)(90)


//91- 101 - ХЗ

//[102 to 119] - is undefined
#define CC_REVERB                         (uint8_t)(91)
#define CC_TREMOLO                        (uint8_t)(92)
#define CC_CHORUS                         (uint8_t)(93)
#define CC_DETUNE                         (uint8_t)(94)
#define CC_PHASER                         (uint8_t)(95)

//******************************120 - 127 сообщения режима канала***********************//

//It's logic message. cc_value or 0, or 127
#define CC_LOCAL_CONTROL_ON_OFF           (uint8_t)(122)

//This value(126) are continuous
#define CC_POLY_MODE_OFF                  (uint8_t)(126)

/*Just command. Without some information byte (data byte = 0!!!)*/
#define CC_ALL_SOUND_OFF                  (uint8_t)(120)
#define CC_ALL_CONTROLLERS_RESER          (uint8_t)(121)
#define CC_ALL_NOTES_OFF                  (uint8_t)(123)
#define CC_OMNI_MODE_OFF                  (uint8_t)(124)
#define CC_OMNI_MODE_ON                   (uint8_t)(125)
#define CC_POLY_MODE_ON                   (uint8_t)(127)
//**************************************************************************************//












/*
   Defines for MIDI Machine Control: loop, record(запись), rewind(перемотка назад)
   То есть для управления звукозаписывающим оборудованием
   Это Real Time SysEx
*/
#define DEVICE_ID                                   (uint8_t)(0x7F)
#define SUB_ID_1_LONG_MTC                           (uint8_t)(0x01)
#define SUB_ID_1_MIDI_SHOW_CONTROL                  (uint8_t)(0x02)
#define SUB_ID_1_NOTATION_INFO                      (uint8_t)(0x03)
#define SUB_ID_1_DEVICE_CONTROL                     (uint8_t)(0x04)
#define SUB_ID_1_REAL_TIME_MTC                      (uint8_t)(0x05)
#define SUB_ID_1_MIDI_MACHINE_CONTROL_CMD           (uint8_t)(0x06)
#define SUB_ID_1_MIDI_MACHINE_CONTROL_RESPONCE      (uint8_t)(0x07)
#define SUB_ID_1_SINGLE_NOTE_RETUNE                 (uint8_t)(0x08)


#define SUB_ID_2_STOP                               (uint8_t)(0x01)
#define SUB_ID_2_PLAY                               (uint8_t)(0x02)
#define SUB_ID_2_FAST_FORWARD                       (uint8_t)(0x04)
#define SUB_ID_2_REWIND                             (uint8_t)(0x05)
#define SUB_ID_2_REC_STROBE                         (uint8_t)(0x06)
#define SUB_ID_2_REC_EXIT                           (uint8_t)(0x07)
#define SUB_ID_2_REC_PAUSE                          (uint8_t)(0x08)


/**USBD_MIDI_Event_Packet: Midi has a fixed packet length = MIDI_PACKET_LENGTH
 You should fill in this structure and call the function midi_transmit_event
 ****************************************************
 **/
typedef struct
{
    uint8_t timestemp_hi;
    uint8_t timestemp_lo;
	uint8_t CIN: 4;  //command identification number
	uint8_t CN:  4;  //channel number
	uint8_t MIDI_0;  //data
	uint8_t MIDI_1;  //data
	uint8_t MIDI_2;  //data
}midi_event_packet_t;







#endif /*_MIDI_KERNEL_*/



