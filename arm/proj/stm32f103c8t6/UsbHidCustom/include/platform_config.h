#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

#include "stm32f10x.h"
#include "stm32f103.h"

/*Unique Devices IDs register set*/
#define         ID1          (0x1FFFF7E8)
#define         ID2          (0x1FFFF7EC)
#define         ID3          (0x1FFFF7F0)

/* Define the STM32F10x hardware depending on the used evaluation board */
#define USB_DISCONNECT                      GPIOD
#define USB_DISCONNECT_PIN                  GPIO_Pin_9

#define RCC_APB2Periph_GPIO_DISCONNECT      RCC_APB2Periph_GPIOD

/* STM32F103xx Reference Manual
 * 50 page, Table 3. Register boundary addresses (continued)
 * ADC1 peripheral boundary adress - 0x40012400 - 0x400127FF
 *
 * 253 page, Table 72. ADC register map and reset values (continued)
 * ADC_DR register offset - 0x4C
 *
 * ADC1_DR_Address = (ADC1 address start) + (ADC_DR offset) =
 * = 0x40012400 + 0x4C = 0x4001244C
 * */
#define ADC1_DR_Address                     ((uint32_t)0x4001244C)
#define RCC_APB2Periph_GPIO_IOAIN           RCC_APB2Periph_GPIOA
#define GPIO_IOAIN                          GPIOA
#define GPIO_IOAIN_PIN                      GPIO_Pin_0
#define ADC_AIN_CHANNEL                     ADC_Channel_0

#endif /* __PLATFORM_CONFIG_H */
