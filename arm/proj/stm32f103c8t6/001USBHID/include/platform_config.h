#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

#include "stm32f10x.h"

// Unique device ID register (96 bits)
// 30.2 Unique device ID register (96 bits) at 1066 page in Reference manual
#define	ID1	(0x1FFFF7E8) // Base address: 0x1FFF F7E8
#define	ID2	(0x1FFFF7EC) // Base address + 0x04
#define	ID3	(0x1FFFF7F0) // Base address + 0x08

//#define MYUSB_Periph	RCC_APB2Periph_GPIOB
//
//#define	MYUSB_Port_Btn	GPIOB
//#define	MYUSB_Pin_Btn1	GPIO_Pin_3
//#define	MYUSB_Pin_Btn2	GPIO_Pin_4
//
//#define	MYUSB_Port_Led	GPIOB
//#define	MYUSB_Pin_Led1	GPIO_Pin_6
//#define	MYUSB_Pin_Led2	GPIO_Pin_7
//#define	MYUSB_Pin_Led3	GPIO_Pin_8

#define	USB_DISCONNECT		GPIOC
#define	USB_DISCONNECT_PIN	GPIO_Pin_13

#endif /* __PLATFORM_CONFIG_H */
