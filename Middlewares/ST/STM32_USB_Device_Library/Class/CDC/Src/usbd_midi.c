

#include <usbd_ctlreq.h>

#include "usbd_midi.h"
#include "usbd_desc.h"


static uint8_t  USBD_MIDI_Init (USBD_HandleTypeDef *pdev,
                               uint8_t cfgidx);

static uint8_t  USBD_MIDI_DeInit (USBD_HandleTypeDef *pdev,
                                 uint8_t cfgidx);

static uint8_t  USBD_MIDI_Setup (USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req);

static uint8_t  USBD_MIDI_DataIn (USBD_HandleTypeDef *pdev,
                                 uint8_t epnum);

static uint8_t  USBD_MIDI_DataOut (USBD_HandleTypeDef *pdev,
                                 uint8_t epnum);

static uint8_t  USBD_MIDI_EP0_TxReady (USBD_HandleTypeDef *pdev);
static uint8_t  USBD_MIDI_EP0_RxReady (USBD_HandleTypeDef *pdev);
static uint8_t USBD_MIDI_SOF (USBD_HandleTypeDef *pdev);
static uint8_t USBD_MIDI_IsoINIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);
static uint8_t USBD_MIDI_IsoOUTIncomplete (USBD_HandleTypeDef *pdev, uint8_t epnum);

static uint8_t  *USBD_MIDI_GetCfgDesc (uint16_t *length);

uint8_t  *USBD_MIDI_GetDeviceQualifierDescriptor (uint16_t *length);

static void MIDI_REQ_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);
static void MIDI_REQ_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req);

/* USB Standard Device Descriptor */
__ALIGN_BEGIN static uint8_t USBD_MIDI_DeviceQualifierDesc[USB_LEN_DEV_QUALIFIER_DESC] __ALIGN_END =
{
		USB_LEN_DEV_QUALIFIER_DESC,
		USB_DESC_TYPE_DEVICE_QUALIFIER,

		0x00,
		0x02,

		0x01,
		0x03,

		0x00,

		0x08,
		0x01,
		0x00,
};

USBD_ClassTypeDef  USBD_MIDI =
{
		USBD_MIDI_Init,
		USBD_MIDI_DeInit,
		USBD_MIDI_Setup,
		USBD_MIDI_EP0_TxReady,
		USBD_MIDI_EP0_RxReady,
		USBD_MIDI_DataIn,
		USBD_MIDI_DataOut,
		USBD_MIDI_SOF,
		USBD_MIDI_IsoINIncomplete,
		USBD_MIDI_IsoOUTIncomplete,
		USBD_MIDI_GetCfgDesc,
		USBD_MIDI_GetCfgDesc,
		USBD_MIDI_GetCfgDesc,
		USBD_MIDI_GetDeviceQualifierDescriptor,
};

/* USB MIDI device Configuration Descriptor */
__ALIGN_BEGIN uint8_t USBD_MIDI_CfgDesc[USB_MIDI_CONFIG_DESC_SIZ] __ALIGN_END =
{
		/*MIDI Adapter Configuration Descriptor*/
		USB_LEN_CFG_DESC,								/* bLength: Configuration Descriptor size */
		USB_DESC_TYPE_CONFIGURATION,			/* bDescriptorType: Configuration */
		LOBYTE(USB_MIDI_CONFIG_DESC_SIZ),	/* wTotalLength:no of returned bytes */
		HIBYTE(USB_MIDI_CONFIG_DESC_SIZ),
		0x02,								/* bNumInterfaces: 2 interface */
		0x01,								/* bConfigurationValue: - ID of this configuration */
		0x00,								/* iConfiguration: Index of string descriptor describing the configuration */
		0x80,								/* bmAttributes: bus powered  - not Self Powered, no Remote wakeup capability*/
		0xFA,								/* MaxPower 500 mA */
		/* 9 */

 		/*---------------------------------------------------------------------------*/

		/*MIDI Adapter Standard Audio Control Interface Descripto*/
		USB_LEN_IF_DESC,					/* bLength: Interface Descriptor size */
		USB_DESC_TYPE_INTERFACE,			/* bDescriptorType: INTERFACE descriptor */
		0x00,								/* bInterfaceNumber: Index of this interface */
		0x00,								/* bAlternateSetting: Alternate setting */
		0x00,								/* bNumEndpoints: 0 endpoints, потому что device не имеет аудио функций */
		0x01,								/* bInterfaceClass: Audio */
		0x01,								/* bInterfaceSubClass: Audio Control */
		0x00,								/* bInterfaceProtocol: */
		0x00,								/* iInterface: */
		/* 18 */

		/* MIDI Adapter Class-specific Audio Control Interface Descriptor */
		USB_LEN_IF_DESC,					/* bLength: */
		0x24,								/* bDescriptorType: CS_INTERFACE */
		0x01,								/* bDescriptorSubtype: Header Func Desc */

		0x00,								/* bcdADC: spec release number */
		0x01,

		0x09,								/* wTotalLength: */
		0x00,

		0x01,								/* bInCollection */
		0x01,								/* baInterfaceNr(1) */
		/* 27 */

		/*  Standard MidiStreamig Interface Descriptor */
		USB_LEN_IF_DESC,					/* bLength */
		USB_DESC_TYPE_INTERFACE,			/* bDescriptorType */
		0x01,								/* bInterfaceNumber */
		0x00,								/* bAlternateSetting */
		0x02,								/* bNumEndpoints */
		0x01,								/* bInterfaceClass  - AUDIO*/
		0x03,								/* bInterfaceSubClass - MIDISTREAMING */
		0x00,								/* bInterfaceProtocol */
		0x00,								/* iInterface */
		/* 36 */

		/*   MIDI Adapter Class-specific MidiStreamig Interface Descriptor*/
		0x07,								/* bLength */
		0x24,								/* bDescriptorType */
		0x01,								/* bDescriptorSubtype */

		0x00, 								/* bcdADC */
		0x01,

		0x41, 								/* wTotalLength */
		0x00,
		/* 43 */

		/*  MIDI IN Jack Descriptor (Embedded) */
		0x06,								/* bLength */
		0x24,								/* bDescriptorType */
		0x02,								/* bDescriptorSubtype */
		0x01,								/* bJackType - Embedded*/
		0x01,								/* bJackID  - ID of this Jack*/
		0x00,								/* iJack(none) - string index for this jack */
		/* 49 */

		/*  MIDI IN Jack Descriptor (External) */
		0x06,								/* bLength */
		0x24,								/* bDescriptorType */
		0x02,								/* bDescriptorSubtype */
		0x02,								/* bJackType - External*/
		0x02,								/* bJackID */
		0x00,								/* iJack(none) - string index for this jack*/
		/* 55 */

		/* MIDI OUT Jack Descriptor (Embedded) */
		0x09,								/* bLength */
		0x24,								/* bDescriptorType */
		0x03,								/* bDescriptorSubtype */
		0x01,								/* bJackType - EMBEDDED*/
		0x03,								/* bJackID - ID of this Jack.*/
		0x01,								/* bNrInputPins - Number of Input Pins of this Jack*/
		0x02,								/* BaSourceID(1) - ID of the Entity to which this Pin isconnected */
		0x01,								/* BaSourcePin(1) Output Pin number of the Entity to whichthis Input Pin is connected.*/
		0x00,								/* iJack(none) - string index for this jack */
		/* 64 */

		/*  MIDI OUT Jack Descriptor (External) */
		0x09,								/* bLength */
		0x24,								/* bDescriptorType */
		0x03,								/* bDescriptorSubtype */
		0x02,								/* bJackType */
		0x04,								/* bJackID */
		0x01,								/* bNrInputPins */
		0x01,								/* BaSourceID(1) */
		0x01,								/* BaSourcePin(1) */
		0x00,								/* iJack */
		/* 73 */

		/*  Standard Bulk OUT Endpoint Descriptor */
		0x09,								/* bLength */
		USB_DESC_TYPE_ENDPOINT,				/* bDescriptorType */
		MIDI_OUT_EP,							/* bEndpointAddress */
		USBD_EP_TYPE_BULK,					/* bmAttributes */
		LOBYTE(MIDI_DATA_OUT_PACKET_SIZE),	/* wMaxPacketSize */
		HIBYTE(MIDI_DATA_OUT_PACKET_SIZE),

		0x00,								/* bInterval */
		0x00,								/* bRefresh */
		0x00,								/* bSynchAddress */
		/* 82 */

		/*  Class-specific MS Bulk OUT Endpoint Descriptor */
		0x05,								/* bLength */
		0x25,								/* bDescriptorType - CS_ENDPOINT descriptor */
		0x01,								/* bDescriptorSubtype - MS_GENERAL subtype. */
		0x01,								/* bNumEmbMIDIJack - Number of embedded MIDI IN Jacks. */
		0x01,								/* BaAssocJackID(1) - ID of the Embedded MIDI IN Jack.*/
		/* 87 */

		/*  Standard Bulk IN Endpoint Descriptor */
		0x09,								/* bLength */
		USB_DESC_TYPE_ENDPOINT,				/* bDescriptorType */
		MIDI_IN_EP,							/* bEndpointAddress */
		USBD_EP_TYPE_BULK,					/* bmAttributes */
		LOBYTE(MIDI_DATA_IN_PACKET_SIZE),	/* wMaxPacketSize */
		HIBYTE(MIDI_DATA_IN_PACKET_SIZE),
		0x00,								/* bInterval */
		0x00,								/* bRefresh */
		0x00,								/* bSynchAddress */
		/* 96 */

		/*  Class-specific MS Bulk IN Endpoint Descriptor */
		0x05,								/* bLength */
		0x25,								/* bDescriptorType - CS_ENDPOINT descriptor */
		0x01,								/* bDescriptorSubtype - MS_GENERAL subtype.*/
		0x01,								/* bNumEmbMIDIJack - Number of embedded MIDI OUT Jacks*/
		0x03,								/* BaAssocJackID(1) - ID of the Embedded MIDI OUT Jack.*/
		/* 101 */
};





static uint8_t  USBD_MIDI_Init (USBD_HandleTypeDef *pdev, uint8_t cfgidx)
{
	uint8_t ret = 0;
	USBD_MIDI_HandleTypeDef   *hmidi;

	//only FS mode

	/* Open EP IN */
	USBD_LL_OpenEP(pdev,
			MIDI_IN_EP,
			USBD_EP_TYPE_BULK,
			MIDI_DATA_IN_PACKET_SIZE);
	/* Open EP OUT */
	USBD_LL_OpenEP(pdev,
			MIDI_OUT_EP,
			USBD_EP_TYPE_BULK,
			MIDI_DATA_OUT_PACKET_SIZE);
	pdev->pClassData = USBD_malloc(sizeof (USBD_MIDI_HandleTypeDef));

	if(pdev->pClassData == NULL)
	{
		ret = USBD_FAIL;
	}
	else
	{
		hmidi = (USBD_MIDI_HandleTypeDef*) pdev->pClassData;
		hmidi->alt_setting = 0;

		/* Init  physical Interface components */
		((USBD_MIDI_ItfTypeDef *)pdev->pUserData)->Init();

		/* Init Xfer states */
		hmidi->TxState =0;
		hmidi->RxState =0;


		USBD_LL_PrepareReceive(pdev,
				MIDI_OUT_EP,
				hmidi->RxBuffer,
				MIDI_DATA_OUT_PACKET_SIZE);
	}
	return ret;
}

static uint8_t  USBD_MIDI_DeInit (USBD_HandleTypeDef *pdev,
                                 uint8_t cfgidx)
{
	uint8_t ret = 0;

	/* Close EP IN */
	USBD_LL_CloseEP(pdev,
			MIDI_IN_EP);

	/* Close EP OUT */
	USBD_LL_CloseEP(pdev,
			MIDI_OUT_EP);

	/* DeInit  physical Interface components */
	if(pdev->pClassData != NULL)
	{
		((USBD_MIDI_ItfTypeDef *)pdev->pUserData)->DeInit();
		USBD_free(pdev->pClassData);
		pdev->pClassData = NULL;
	}
	return ret;
}

static uint8_t  USBD_MIDI_Setup (USBD_HandleTypeDef *pdev,
                                USBD_SetupReqTypedef *req)
{
	USBD_MIDI_HandleTypeDef *hmidi = (USBD_MIDI_HandleTypeDef*) pdev->pClassData;
	uint16_t len;
	uint8_t *pbuf;
	uint8_t ret = USBD_OK;

	switch (req->bmRequest & USB_REQ_TYPE_MASK)
	{
	case USB_REQ_TYPE_CLASS:
		switch (req->bRequest)
		{
		case AUDIO_REQ_GET_CUR:
			MIDI_REQ_GetCurrent(pdev, req);
			break;
		case AUDIO_REQ_SET_CUR:
			MIDI_REQ_SetCurrent(pdev, req);
			break;
		default:
			USBD_CtlError(pdev, req);
			ret = USBD_FAIL;
			break;
		}
		break;

	case USB_REQ_TYPE_STANDARD:
		switch (req->bRequest)
		{
		case USB_REQ_GET_DESCRIPTOR:
			if ((req->wValue >> 8) == AUDIO_DESCRIPTOR_TYPE)
			{
				pbuf = USBD_MIDI_CfgDesc + 18;
				len = MIN(USB_AUDIO_DESC_SIZ, req->wLength);
				USBD_CtlSendData(pdev, pbuf, len);
			}
			break;
		case USB_REQ_GET_INTERFACE:
			USBD_CtlSendData(pdev,
								(uint8_t*)&(hmidi->alt_setting),
								1);
			break;
		case USB_REQ_SET_INTERFACE:
			if ((uint8_t)(req->wValue) <= USBD_MAX_NUM_INTERFACES)
			{
				hmidi->alt_setting = (uint8_t)(req->wValue);
			}
			else
			{
				USBD_CtlError(pdev, req);
			}
			break;
		default:
			USBD_CtlError(pdev, req);
			ret = USBD_FAIL;
			break;
		}
		break;
	default:
		break;
	}

	return ret;
}

static uint8_t  USBD_MIDI_DataIn (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	USBD_MIDI_HandleTypeDef   *hcdc = (USBD_MIDI_HandleTypeDef*) pdev->pClassData;
	if(pdev->pClassData != NULL)
	{
		hcdc->TxState = 0;
		return USBD_OK;
	}
	else
	{
		return USBD_FAIL;
	}
}

static uint8_t  USBD_MIDI_DataOut (USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	USBD_MIDI_HandleTypeDef   *hmidi = (USBD_MIDI_HandleTypeDef*) pdev->pClassData;

	if (epnum == MIDI_OUT_EP)
	{
		hmidi->RxLength = USBD_LL_GetRxDataSize(pdev, epnum);

		if (pdev->pClassData != NULL)
		{
			((USBD_MIDI_ItfTypeDef*)pdev->pUserData)->Receive(hmidi->RxBuffer, &hmidi->RxLength);
		}
		USBD_LL_PrepareReceive(pdev,
				MIDI_OUT_EP,
				hmidi->RxBuffer,
				MIDI_DATA_OUT_PACKET_SIZE);
		return USBD_OK;
	}
	else
	{
		return USBD_FAIL;
	}
}

static uint8_t USBD_MIDI_SOF(USBD_HandleTypeDef *pdev)
{
	return USBD_OK;
}

static uint8_t USBD_MIDI_IsoINIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	return USBD_OK;
}

static uint8_t USBD_MIDI_IsoOUTIncomplete(USBD_HandleTypeDef *pdev, uint8_t epnum)
{
	return USBD_OK;
}

static uint8_t USBD_MIDI_EP0_TxReady(USBD_HandleTypeDef *pdev)
{
	return USBD_OK;
}


static uint8_t  USBD_MIDI_EP0_RxReady (USBD_HandleTypeDef *pdev)
{
	USBD_MIDI_HandleTypeDef   *hmidi = (USBD_MIDI_HandleTypeDef*) pdev->pClassData;

	if((pdev->pUserData != NULL) && (hmidi->control.cmd != 0))
	{
		((USBD_MIDI_ItfTypeDef *)pdev->pUserData)->Control(hmidi->control.cmd,
															(uint8_t *)hmidi->data,
															hmidi->control.len);
		hmidi->control.cmd = 0;
	}
	return USBD_OK;
}

static uint8_t  *USBD_MIDI_GetCfgDesc (uint16_t *length)
{
	*length = sizeof (USBD_MIDI_CfgDesc);
	return USBD_MIDI_CfgDesc;
}

uint8_t  *USBD_MIDI_GetDeviceQualifierDescriptor (uint16_t *length)
{
  *length = sizeof (USBD_MIDI_DeviceQualifierDesc);
  return USBD_MIDI_DeviceQualifierDesc;
}

uint8_t  USBD_MIDI_RegisterInterface  (USBD_HandleTypeDef   *pdev,
		USBD_MIDI_ItfTypeDef *fops)
{
	uint8_t  ret = USBD_FAIL;

	if(fops != NULL)
	{
		pdev->pUserData = fops;
		ret = USBD_OK;
	}

	return ret;
}

uint8_t  USBD_MIDI_SetTxBuffer  (USBD_HandleTypeDef   *pdev,
		uint8_t  *pbuff,
		uint16_t length)
{
	USBD_MIDI_HandleTypeDef   *hmidi = (USBD_MIDI_HandleTypeDef*) pdev->pClassData;

	hmidi->TxBuffer = pbuff;
	hmidi->TxLength = length;

	return USBD_OK;
}

uint8_t  USBD_MIDI_SetRxBuffer  (USBD_HandleTypeDef   *pdev,
		uint8_t  *pbuff)
{
	USBD_MIDI_HandleTypeDef   *hmidi = (USBD_MIDI_HandleTypeDef*) pdev->pClassData;
	hmidi->RxBuffer = pbuff;

	return USBD_OK;
}

uint8_t  USBD_MIDI_TransmitPacket(USBD_HandleTypeDef *pdev)
{
	USBD_MIDI_HandleTypeDef   *hmidi = (USBD_MIDI_HandleTypeDef*) pdev->pClassData;

	if(pdev->pClassData != NULL)
	{
		if(hmidi->TxState == 0)
		{
			/* Tx Transfer in progress */
			hmidi->TxState = 1;
			/* Transmit next packet */
			USBD_LL_Transmit(pdev,
							 MIDI_IN_EP,
							 hmidi->TxBuffer,
							 hmidi->TxLength);
			return USBD_OK;
		}
		else
		{
			return USBD_BUSY;
		}
	}
	else
	{
		return USBD_FAIL;
	}
}

uint8_t  USBD_MIDI_ReceivePacket(USBD_HandleTypeDef *pdev)
{
	USBD_MIDI_HandleTypeDef   *hmidi = (USBD_MIDI_HandleTypeDef*) pdev->pClassData;

	/* Suspend or Resume USB Out process */
	if(pdev->pClassData != NULL)
	{
		USBD_LL_PrepareReceive(pdev,
								MIDI_OUT_EP,
								hmidi->RxBuffer,
								MIDI_DATA_OUT_PACKET_SIZE);
		return USBD_OK;
	}
	else
	{
		return USBD_FAIL;
	}
}




static void MIDI_REQ_GetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
	USBD_MIDI_HandleTypeDef *hmidi;
	hmidi = (USBD_MIDI_HandleTypeDef*)pdev->pClassData;

	memset(hmidi->control.data, 0, USB_MAX_EP0_SIZE);
	USBD_CtlSendData(pdev,
					 hmidi->control.data,
					 req->wLength);
}

static void MIDI_REQ_SetCurrent(USBD_HandleTypeDef *pdev, USBD_SetupReqTypedef *req)
{
	USBD_MIDI_HandleTypeDef *hmidi;
	hmidi = (USBD_MIDI_HandleTypeDef*)pdev->pClassData;

	if (req->wLength)
	{
		USBD_CtlPrepareRx(pdev,
						  hmidi->control.data,
						  req->wLength);

		hmidi->control.cmd = AUDIO_REQ_SET_CUR;
		hmidi->control.len = req->wLength;
		hmidi->control.unit = HIBYTE(req->wIndex);
	}
}

