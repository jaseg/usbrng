/*
  This code has been copied from LUFA

  Copyright 2012  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaim all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/

#include "USBTask.h"

volatile bool        USB_IsInitialized;
USB_Request_Header_t USB_ControlRequest;

#if !defined(DEVICE_STATE_AS_GPIOR)
volatile uint8_t     USB_DeviceState;
#endif

void USB_USBTask(void)
{
    USB_DeviceTask();
}

void USB_DeviceTask(void)
{
	if (USB_DeviceState != DEVICE_STATE_Unattached)
	{
		uint8_t PrevEndpoint = Endpoint_GetCurrentEndpoint();

		Endpoint_SelectEndpoint(ENDPOINT_CONTROLEP);

		if (Endpoint_IsSETUPReceived())
		  USB_Device_ProcessControlRequest();

		Endpoint_SelectEndpoint(PrevEndpoint);
	}
}
