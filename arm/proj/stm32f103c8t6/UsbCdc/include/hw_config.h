#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#include "platform_config.h"
#include "usb_type.h"
#include <stdbool.h>

#define USART_RX_DATA_SIZE   2048 // Circular buffer size

typedef enum
{
    USBTxState_CheckDataToBeSendAfterSOF = 0,
    USBTxState_SendDataNormal = !USBTxState_CheckDataToBeSendAfterSOF
} USBTxState;

void Set_System(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config(FunctionalState NewState);
void Add_Data_To_Rx_Buffer(uint8_t data);
void Handle_USBAsynchXfer(void);
void Get_SerialNum(void);

#endif  /*__HW_CONFIG_H*/
