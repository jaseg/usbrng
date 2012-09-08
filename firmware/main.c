

#include <avr/io.h>
#include <avr/wdt.h>
#include <avr/power.h>
#include "srsly/USB.h"
#include "srsly/Descriptors.h"
#include "main.h"

/** LUFA CDC Class driver interface configuration and state information. This structure is
 *  passed to all CDC Class driver functions, so that multiple instances of the same class
 *  within a device can be differentiated from one another.
 */
USB_ClassInfo_CDC_Device_t cdcif =
	{
		.Config =
			{
				.ControlInterfaceNumber   = 0,
				.DataINEndpoint           =
					{
						.Address          = CDC_TX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.DataOUTEndpoint =
					{
						.Address          = CDC_RX_EPADDR,
						.Size             = CDC_TXRX_EPSIZE,
						.Banks            = 1,
					},
				.NotificationEndpoint =
					{
						.Address          = CDC_NOTIFICATION_EPADDR,
						.Size             = CDC_NOTIFICATION_EPSIZE,
						.Banks            = 1,
					},
			},
	};

uint8_t random_data[8];

void readBitsAndWhiten(){
    static uint8_t j = 0;
    for(uint16_t i=0; i<sizeof(random_data); i++){
        random_data[i] = j;//PORTB&1;
        //TODO add whitening here
        j++;
        //_delay_us(10);
    }
}

void sendData(){
    for(uint16_t i=0; i<sizeof(random_data); i++){
        //CDC_Device_SendByte(&cdcif, random_data[i]);
    }
    if(CDC_Device_SendString(&cdcif, "Fnord!\n") == ENDPOINT_RWSTREAM_NoError){
        PORTD |= 0x30;
    }else{
        PORTD &= 0xCF;
    }
    CDC_Device_Flush(&cdcif);
}

void setup(){
    MCUSR &= ~(1 << WDRF);
    wdt_disable();

    clock_prescale_set(clock_div_1);

    DDRD |= 0x30;
    PORTD &= 0xCF;

    USB_Init();
    sei();
}

void loop(){
    readBitsAndWhiten();
    sendData();
    CDC_Device_USBTask(&cdcif);
    USB_USBTask();
}

int main(void){
   setup();
   for(;;){
       loop();
   }
}

void EVENT_CDC_Device_LineEncodingChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo){
    //ignore
}

void EVENT_CDC_Device_ControLineStateChanged(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo){
    //ignore
}

void EVENT_CDC_Device_BreakSent(USB_ClassInfo_CDC_Device_t* const CDCInterfaceInfo, const uint8_t Duration){
    //ignore
}
