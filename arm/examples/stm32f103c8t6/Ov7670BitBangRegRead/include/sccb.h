#ifndef _SCCB_H
#define _SCCB_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x_gpio.h"

#define GPIO_SCCB_CLOCK_Periph  RCC_APB2Periph_GPIOB
#define GPIO_SCCB_CLOCK_Port    GPIOB
#define GPIO_SCCB_CLOCK_Pin     GPIO_Pin_0

#define GPIO_SCCB_DATA_Periph   RCC_APB2Periph_GPIOB
#define GPIO_SCCB_DATA_Port     GPIOB
#define GPIO_SCCB_DATA_Pin      GPIO_Pin_1

#define OV7670_ADDRESS          0x21                        // 7bit ov7670 address
#define OV7670_ADDRESS_WRITE    (OV7670_ADDRESS << 1)       // 0x42
#define OV7670_ADDRESS_READ     ((OV7670_ADDRESS << 1) + 1) // 0x43

#define REG_PID 0x0A; // PID (Product ID Number MSB (Read only))
#define REG_VER 0x0B; // VER (Product ID Number LSB (Read only))

void sccb_init();
void sccb_start();
void sccb_stop();
uint8_t sccb_write(uint8_t);
uint8_t sccb_read();

#ifdef __cplusplus
}
#endif

#endif /* _SCCB_H */
