#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#include "platform_config.h"
#include "usb_type.h"

void Set_System(void);
void Set_USBClock(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config(FunctionalState NewState);
void GPIO_Configuration(void);
void EXTI_Configuration(void);
void ADC_Configuration(void);
void Get_SerialNum(void);

#endif  /*__HW_CONFIG_H*/
