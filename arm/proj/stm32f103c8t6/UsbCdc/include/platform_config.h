#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

#include "stm32f10x.h"

/*Unique Devices IDs register set*/
#define         ID1          (0x1FFFF7E8)
#define         ID2          (0x1FFFF7EC)
#define         ID3          (0x1FFFF7F0)

#define USB_DISCONNECT                      GPIOD
#define USB_DISCONNECT_PIN                  GPIO_Pin_9
#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOD

#endif /* __PLATFORM_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
