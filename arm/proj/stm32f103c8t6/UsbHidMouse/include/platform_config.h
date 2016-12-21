#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

#include "stm32f10x.h"

/*Unique Devices IDs register set*/
#define         ID1          (0x1FFFF7E8)
#define         ID2          (0x1FFFF7EC)
#define         ID3          (0x1FFFF7F0)

/* Define the STM32F10x hardware depending on the used evaluation board */
#define USB_DISCONNECT                      GPIOD
#define USB_DISCONNECT_PIN                  GPIO_Pin_9

#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOD

#define RCC_APB2Periph_ALLGPIO              (RCC_APB2Periph_GPIOA \
                                           | RCC_APB2Periph_GPIOB \
                                           | RCC_APB2Periph_GPIOC \
                                           | RCC_APB2Periph_GPIOD \
                                           | RCC_APB2Periph_GPIOE )

#endif /* __PLATFORM_CONFIG_H */
