#ifndef __USB_MIDI_H
#define __USB_MIDI_H

#ifdef __cplusplus

extern "C" {
#endif

#include <stdint.h>
#include  <usbd_ioreq.h>

#define MIDI_IN_EP                                   0x81  /* EP1 for data IN */
#define MIDI_OUT_EP                                  0x01  /* EP1 for data OUT */

#define MIDI_DATA_MAX_PACKET_SIZE					64 /* Endpoint IN & OUT Packet size */

#define USB_MIDI_CONFIG_DESC_SIZ                    101
#define MIDI_DATA_IN_PACKET_SIZE					MIDI_DATA_MAX_PACKET_SIZE
#define MIDI_DATA_OUT_PACKET_SIZE					MIDI_DATA_MAX_PACKET_SIZE

#define AUDIO_DESCRIPTOR_TYPE                       0x21
#define USB_AUDIO_DESC_SIZ                          0x09

#define MS_DESCRIPTOR_UNDEFINED						0x00
#define MS_HEADER									0x01
#define MIDI_IN_JACK								0x02
#define MIDI_OUT_JACK								0x03
#define ELEMENT										0x04

#define DESCRIPTOR_UNDEFINED							0x00
#define MS_GENERAL									0x01

#define JACK_TYPE_UNDEFINED							0x00
#define EMBEDDED									0x01
#define EXTERNAL									0x02

#define EP_CONTROL_UNDEFINED							0x00
#define ASSOCIATION_CONTROL							0x01

#define AUDIO_REQ_GET_CUR			0x81
#define AUDIO_REQ_SET_CUR			0x01


typedef struct _USBD_MIDI_Itf
{
  int8_t (* Init)          (void);
  int8_t (* DeInit)        (void);
  int8_t (* Control)       (uint8_t, uint8_t * , uint16_t);
  int8_t (* Receive)       (uint8_t *, uint32_t *);
} USBD_MIDI_ItfTypeDef;



typedef struct
{
	uint8_t cmd;
	uint8_t data[USB_MAX_EP0_SIZE];
	uint8_t len;
	uint8_t unit;
} USBD_MIDI_ControlTypeDef;

typedef struct
{
	uint32_t data[MIDI_DATA_MAX_PACKET_SIZE/4];      /* Force 32bits alignment */
	uint8_t  *RxBuffer;
	uint8_t  *TxBuffer;
	uint32_t RxLength;
	uint32_t TxLength;

	__IO uint32_t TxState;
	__IO uint32_t RxState;
	USBD_MIDI_ControlTypeDef control;
	__IO uint32_t alt_setting;
} USBD_MIDI_HandleTypeDef;

extern USBD_ClassTypeDef  USBD_MIDI;
#define USBD_MIDI_CLASS    &USBD_MIDI

uint8_t  USBD_MIDI_RegisterInterface  (USBD_HandleTypeDef   *pdev,
                                      USBD_MIDI_ItfTypeDef *fops);

uint8_t  USBD_MIDI_SetTxBuffer        (USBD_HandleTypeDef   *pdev,
                                      uint8_t  *pbuff,
                                      uint16_t length);

uint8_t  USBD_MIDI_SetRxBuffer        (USBD_HandleTypeDef   *pdev,
                                      uint8_t  *pbuff);

uint8_t  USBD_MIDI_ReceivePacket      (USBD_HandleTypeDef *pdev);

uint8_t  USBD_MIDI_TransmitPacket     (USBD_HandleTypeDef *pdev);

#ifdef __cplusplus
}
#endif

#endif  /* __USB_MIDI_H */
