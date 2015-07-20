/*
 * hal_hd44780.h
 * Hardware Abstraction Layer
 */

#ifndef __HAL_HD44780_H
#define __HAL_HD44780_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"

#define HD44780_Periph	RCC_APB2Periph_GPIOA
#define HD44780_Port	GPIOA
#define HD44780_Pin_RS	GPIO_Pin_0
#define HD44780_Pin_EN	GPIO_Pin_1
#define HD44780_Pin_D4	GPIO_Pin_2
#define HD44780_Pin_D5	GPIO_Pin_3
#define HD44780_Pin_D6	GPIO_Pin_4
#define HD44780_Pin_D7	GPIO_Pin_5

#ifdef __cplusplus
}
#endif

#endif /* __HAL_HD44780_H */
