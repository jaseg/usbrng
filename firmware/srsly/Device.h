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

/** \file
 *  \brief Common USB Device definitions for all architectures.
 *  \copydetails Group_Device
 *
 *  \note This file should not be included directly. It is automatically included as needed by the USB driver
 *        dispatch header located in LUFA/Drivers/USB/USB.h.
 */

/** \ingroup Group_USB
 *  \defgroup Group_Device Device Management
 *  \brief USB Device management definitions for USB device mode.
 *
 *  USB Device mode related definitions common to all architectures. This module contains definitions which
 *  are used when the USB controller is initialized in device mode.
 *
 *  @{
 */

#ifndef __USBDEVICE_H__
#define __USBDEVICE_H__

#include "Common.h"
#include "USBMode.h"
#include "USBInterrupt.h"
#include "USBController.h"
#include "StdDescriptors.h"
#include "Endpoint.h"

/* Public Interface - May be used in end-application: */
/* Enums: */
/** Enum for the various states of the USB Device state machine. Only some states are
 *  implemented in the LUFA library - other states are left to the user to implement.
 *
 *  For information on each possible USB device state, refer to the USB 2.0 specification.
 *
 *  \see \ref USB_DeviceState, which stores the current device state machine state.
 */
enum USB_Device_States_t
{
    DEVICE_STATE_Unattached                   = 0, /**< Internally implemented by the library. This state indicates
                                                    *   that the device is not currently connected to a host.
                                                    */
    DEVICE_STATE_Powered                      = 1, /**< Internally implemented by the library. This state indicates
                                                    *   that the device is connected to a host, but enumeration has not
                                                    *   yet begun.
                                                    */
    DEVICE_STATE_Default                      = 2, /**< Internally implemented by the library. This state indicates
                                                    *   that the device's USB bus has been reset by the host and it is
                                                    *   now waiting for the host to begin the enumeration process.
                                                    */
    DEVICE_STATE_Addressed                    = 3, /**< Internally implemented by the library. This state indicates
                                                    *   that the device has been addressed by the USB Host, but is not
                                                    *   yet configured.
                                                    */
    DEVICE_STATE_Configured                   = 4, /**< May be implemented by the user project. This state indicates
                                                    *   that the device has been enumerated by the host and is ready
                                                    *   for USB communications to begin.
                                                    */
    DEVICE_STATE_Suspended                    = 5, /**< May be implemented by the user project. This state indicates
                                                    *   that the USB bus has been suspended by the host, and the device
                                                    *   should power down to a minimal power level until the bus is
                                                    *   resumed.
                                                    */
};

/* Function Prototypes: */
/** Function to retrieve a given descriptor's size and memory location from the given descriptor type value,
 *  index and language ID. This function MUST be overridden in the user application (added with full, identical
 *  prototype and name so that the library can call it to retrieve descriptor data.
 *
 *  \param[in] wValue                  The type of the descriptor to retrieve in the upper byte, and the index in the
 *                                     lower byte (when more than one descriptor of the given type exists, such as the
 *                                     case of string descriptors). The type may be one of the standard types defined
 *                                     in the DescriptorTypes_t enum, or may be a class-specific descriptor type value.
 *  \param[in] wIndex                  The language ID of the string to return if the \c wValue type indicates
 *                                     \ref DTYPE_String, otherwise zero for standard descriptors, or as defined in a
 *                                     class-specific standards.
 *  \param[out] DescriptorAddress      Pointer to the descriptor in memory. This should be set by the routine to
 *                                     the address of the descriptor.
 *  \param[out] DescriptorMemorySpace  A value from the \ref USB_DescriptorMemorySpaces_t enum to indicate the memory
 *                                     space in which the descriptor is stored. This parameter does not exist when one
 *                                     of the \c USE_*_DESCRIPTORS compile time options is used, or on architectures which
 *                                     use a unified address space.
 *
 *  \note By default, the library expects all descriptors to be located in flash memory via the \c PROGMEM attribute.
 *        If descriptors should be located in RAM or EEPROM instead (to speed up access in the case of RAM, or to
 *        allow the descriptors to be changed dynamically at runtime) either the \c USE_RAM_DESCRIPTORS or the
 *        \c USE_EEPROM_DESCRIPTORS tokens may be defined in the project makefile and passed to the compiler by the -D
 *        switch.
 *
 *  \return Size in bytes of the descriptor if it exists, zero or \ref NO_DESCRIPTOR otherwise.
 */
uint16_t CALLBACK_USB_GetDescriptor(const uint16_t wValue,
                                    const uint8_t wIndex,
                                    const void** const DescriptorAddress);

/** \name USB Device Mode Option Masks */
//@{
#if defined(USB_SERIES_4_AVR) || defined(USB_SERIES_6_AVR) || defined(USB_SERIES_7_AVR) || defined(__DOXYGEN__)
/** Mask for the Options parameter of the \ref USB_Init() function. This indicates that the
 *  USB interface should be initialized in low speed (1.5Mb/s) mode.
 *
 *  \note Low Speed mode is not available on all USB AVR models.
 *        \n
 *
 *  \note Restrictions apply on the number, size and type of endpoints which can be used
 *        when running in low speed mode - please refer to the USB 2.0 specification.
 */
#define USB_DEVICE_OPT_LOWSPEED            0x01
#endif

/** Mask for the Options parameter of the \ref USB_Init() function. This indicates that the
 *  USB interface should be initialized in full speed (12Mb/s) mode.
 */
#define USB_DEVICE_OPT_FULLSPEED               0x00
//@}

#if (!defined(NO_INTERNAL_SERIAL) && \
     (defined(USB_SERIES_7_AVR) || defined(USB_SERIES_6_AVR) || \
      defined(USB_SERIES_4_AVR) || defined(USB_SERIES_2_AVR)))
/** String descriptor index for the device's unique serial number string descriptor within the device.
 *  This unique serial number is used by the host to associate resources to the device (such as drivers or COM port
 *  number allocations) to a device regardless of the port it is plugged in to on the host. Some microcontrollers contain
 *  a unique serial number internally, and setting the device descriptors serial number string index to this value
 *  will cause it to use the internal serial number.
 *
 *  On unsupported devices, this will evaluate to \ref NO_DESCRIPTOR and so will force the host to create a pseudo-serial
 *  number for the device.
 */
#define USE_INTERNAL_SERIAL            0xDC

/** Length of the device's unique internal serial number, in bits, if present on the selected microcontroller
 *  model.
 */
#define INTERNAL_SERIAL_LENGTH_BITS    80

/** Start address of the internal serial number, in the appropriate address space, if present on the selected microcontroller
 *  model.
 */
#define INTERNAL_SERIAL_START_ADDRESS  0x0E
#else
#define USE_INTERNAL_SERIAL            NO_DESCRIPTOR
#define INTERNAL_SERIAL_LENGTH_BITS    0
#define INTERNAL_SERIAL_START_ADDRESS  0
#endif

/* Function Prototypes: */
/** Sends a Remote Wakeup request to the host. This signals to the host that the device should
 *  be taken out of suspended mode, and communications should resume.
 *
 *  Typically, this is implemented so that HID devices (mice, keyboards, etc.) can wake up the
 *  host computer when the host has suspended all USB devices to enter a low power state.
 *
 *  \attention This function should only be used if the device has indicated to the host that it
 *             supports the Remote Wakeup feature in the device descriptors, and should only be
 *             issued if the host is currently allowing remote wakeup events from the device (i.e.,
 *             the \ref USB_Device_RemoteWakeupEnabled flag is set). When the \c NO_DEVICE_REMOTE_WAKEUP
 *             compile time option is used, this function is unavailable.
 *             \n\n
 *
 *  \attention The USB clock must be running for this function to operate. If the stack is initialized with
 *             the \ref USB_OPT_MANUAL_PLL option enabled, the user must ensure that the PLL is running
 *             before attempting to call this function.
 *
 *  \see \ref Group_StdDescriptors for more information on the RMWAKEUP feature and device descriptors.
 */
void USB_Device_SendRemoteWakeup(void);

/* Inline Functions: */
/** Returns the current USB frame number, when in device mode. Every millisecond the USB bus is active (i.e. enumerated to a host)
 *  the frame number is incremented by one.
 *
 *  \return Current USB frame number from the USB controller.
 */
static inline uint16_t USB_Device_GetFrameNumber(void) ATTR_ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT;
static inline uint16_t USB_Device_GetFrameNumber(void)
{
    return UDFNUM;
}

#if !defined(NO_SOF_EVENTS)
/** Enables the device mode Start Of Frame events. When enabled, this causes the
 *  \ref EVENT_USB_Device_StartOfFrame() event to fire once per millisecond, synchronized to the USB bus,
 *  at the start of each USB frame when enumerated in device mode.
 *
 *  \note This function is not available when the \c NO_SOF_EVENTS compile time token is defined.
 */
static inline void USB_Device_EnableSOFEvents(void) ATTR_ALWAYS_INLINE;
static inline void USB_Device_EnableSOFEvents(void)
{
    USB_INT_Enable(USB_INT_SOFI);
}

/** Disables the device mode Start Of Frame events. When disabled, this stops the firing of the
 *  \ref EVENT_USB_Device_StartOfFrame() event when enumerated in device mode.
 *
 *  \note This function is not available when the \c NO_SOF_EVENTS compile time token is defined.
 */
static inline void USB_Device_DisableSOFEvents(void) ATTR_ALWAYS_INLINE;
static inline void USB_Device_DisableSOFEvents(void)
{
    USB_INT_Disable(USB_INT_SOFI);
}
#endif

/* Private Interface - For use in library only: */
/* Inline Functions: */
#if defined(USB_DEVICE_OPT_LOWSPEED)
static inline void USB_Device_SetLowSpeed(void) ATTR_ALWAYS_INLINE;
static inline void USB_Device_SetLowSpeed(void)
{
    UDCON |=  (1 << LSM);
}

static inline void USB_Device_SetFullSpeed(void) ATTR_ALWAYS_INLINE;
static inline void USB_Device_SetFullSpeed(void)
{
    UDCON &= ~(1 << LSM);
}
#endif

static inline void USB_Device_SetDeviceAddress(const uint8_t Address) ATTR_ALWAYS_INLINE;
static inline void USB_Device_SetDeviceAddress(const uint8_t Address)
{
    uint8_t Temp = (UDADDR & (1 << ADDEN)) | (Address & 0x7F);

    UDADDR = Temp;
    UDADDR = Temp | (1 << ADDEN);
}

static inline bool USB_Device_IsAddressSet(void) ATTR_ALWAYS_INLINE ATTR_WARN_UNUSED_RESULT;
static inline bool USB_Device_IsAddressSet(void)
{
    return (UDADDR & (1 << ADDEN));
}

#if (USE_INTERNAL_SERIAL != NO_DESCRIPTOR)
static inline void USB_Device_GetSerialString(uint16_t* const UnicodeString) ATTR_NON_NULL_PTR_ARG(1);
static inline void USB_Device_GetSerialString(uint16_t* const UnicodeString)
{
    uint_reg_t CurrentGlobalInt = GetGlobalInterruptMask();
    GlobalInterruptDisable();

    uint8_t SigReadAddress = INTERNAL_SERIAL_START_ADDRESS;

    for (uint8_t SerialCharNum = 0; SerialCharNum < (INTERNAL_SERIAL_LENGTH_BITS / 4); SerialCharNum++)
    {
        uint8_t SerialByte = boot_signature_byte_get(SigReadAddress);

        if (SerialCharNum & 0x01)
        {
            SerialByte >>= 4;
            SigReadAddress++;
        }

        SerialByte &= 0x0F;

        UnicodeString[SerialCharNum] = cpu_to_le16((SerialByte >= 10) ?
                                                   (('A' - 10) + SerialByte) : ('0' + SerialByte));
    }

    SetGlobalInterruptMask(CurrentGlobalInt);
}
#endif

#endif

/** @} */

