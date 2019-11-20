
#include "usbd_midi_if.h"




/* Define size for the receive and transmit buffer over MIDI */
/* It's up to user to redefine and/or remove those define */
#define APP_RX_DATA_SIZE  64

#define APP_TX_DATA_SIZE  64






/* Create buffer for reception and transmission           */
/* It's up to user to redefine and/or remove those define */
/* Received Data over USB are stored in this buffer       */
uint8_t UserRxBufferFS[APP_RX_DATA_SIZE];

/* Send Data over USB MIDI are stored in this buffer       */
uint8_t UserTxBufferFS[APP_TX_DATA_SIZE];





extern USBD_HandleTypeDef hUsbDeviceFS;






static int8_t MIDI_Init_FS     (void);
static int8_t MIDI_DeInit_FS   (void);
static int8_t MIDI_Control_FS  (uint8_t cmd, uint8_t* pbuf, uint16_t length);
static int8_t MIDI_Receive_FS  (uint8_t* pbuf, uint32_t *Len);







USBD_MIDI_ItfTypeDef USBD_Interface_fops_FS =
{
    MIDI_Init_FS,
    MIDI_DeInit_FS,
    MIDI_Control_FS,
    MIDI_Receive_FS
};















/**
  * @brief  MIDI_Init_FS
  *         Initializes the MIDI media low layer over the USB HS IP
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MIDI_Init_FS(void)
{
  /* Set Application Buffers */
  USBD_MIDI_SetTxBuffer(&hUsbDeviceFS, UserTxBufferFS, 0);
  USBD_MIDI_SetRxBuffer(&hUsbDeviceFS, UserRxBufferFS);
  return (USBD_OK);
}











/**
  * @brief  MIDI_DeInit_FS
  *         DeInitializes the MIDI media low layer
  * @param  None
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MIDI_DeInit_FS(void)
{

  return (USBD_OK);
}












/**
  * @brief  MIDI_Control_FS
  *         Manage the MIDI class requests
  * @param  cmd: Command code
  * @param  pbuf: Buffer containing command data (request parameters)
  * @param  length: Number of data to be sent (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MIDI_Control_FS  (uint8_t cmd, uint8_t* pbuf, uint16_t length)
{
  return (USBD_OK);
}










/**
  * @brief  MIDI_Receive_FS
  *         Data received over USB OUT endpoint are sent over MIDI interface
  *         through this function.
  *
  *         @note
  *         This function will block any OUT packet reception on USB endpoint
  *         untill exiting this function. If you exit this function before transfer
  *         is complete on MIDI interface (ie. using DMA controller) it will result
  *         in receiving more data while previous ones are still not sent.
  *
  * @param  Buf: Buffer of data to be received
  * @param  Len: Number of data received (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL
  */
static int8_t MIDI_Receive_FS (uint8_t* Buf, uint32_t *Len)
{
	USBD_MIDI_ReceivePacket(&hUsbDeviceFS);

  return(USBD_OK);
}





/**
  * @brief  MIDI_Transmit_FS
  *         Data send over USB IN endpoint are sent over MIDI interface
  *         through this function.
  *         @note
  *
  *
  * @param  Buf: Buffer of data to be send
  * @param  Len: Number of data to be send (in bytes)
  * @retval Result of the operation: USBD_OK if all operations are OK else USBD_FAIL or USBD_BUSY
  */
uint8_t MIDI_Transmit_FS(uint8_t* Buf, uint16_t Len)
{
  uint8_t result = USBD_OK;
  USBD_MIDI_HandleTypeDef *hMIDI = (USBD_MIDI_HandleTypeDef*)hUsbDeviceFS.pClassData;

  if (hMIDI->TxState != 0)
  {
    return USBD_BUSY;
  }

  USBD_MIDI_SetTxBuffer(&hUsbDeviceFS, Buf, Len);
  result = USBD_MIDI_TransmitPacket(&hUsbDeviceFS);

  return result;
}




























