#ifndef __STM32F103_H
#define __STM32F103_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"

typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3
} Led_TypeDef;

typedef enum 
{  
  BUTTON_TAMPER = 0,
  BUTTON_KEY = 1
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

#define LEDn                             4

#define LED1_PIN                         GPIO_Pin_5
#define LED1_GPIO_PORT                   GPIOB
#define LED1_GPIO_CLK                    RCC_APB2Periph_GPIOB
  
#define LED2_PIN                         GPIO_Pin_6
#define LED2_GPIO_PORT                   GPIOB
#define LED2_GPIO_CLK                    RCC_APB2Periph_GPIOB

#define LED3_PIN                         GPIO_Pin_7
#define LED3_GPIO_PORT                   GPIOB
#define LED3_GPIO_CLK                    RCC_APB2Periph_GPIOB
  
#define LED4_PIN                         GPIO_Pin_8
#define LED4_GPIO_PORT                   GPIOB
#define LED4_GPIO_CLK                    RCC_APB2Periph_GPIOB

#define BUTTONn                          2

#define TAMPER_BUTTON_PIN                GPIO_Pin_10
#define TAMPER_BUTTON_GPIO_PORT          GPIOB
#define TAMPER_BUTTON_GPIO_CLK           RCC_APB2Periph_GPIOB
#define TAMPER_BUTTON_EXTI_LINE          EXTI_Line10
#define TAMPER_BUTTON_EXTI_PORT_SOURCE   GPIO_PortSourceGPIOB
#define TAMPER_BUTTON_EXTI_PIN_SOURCE    GPIO_PinSource10
#define TAMPER_BUTTON_EXTI_IRQn          EXTI15_10_IRQn 

#define KEY_BUTTON_PIN                   GPIO_Pin_9
#define KEY_BUTTON_GPIO_PORT             GPIOB
#define KEY_BUTTON_GPIO_CLK              RCC_APB2Periph_GPIOB
#define KEY_BUTTON_EXTI_LINE             EXTI_Line9
#define KEY_BUTTON_EXTI_PORT_SOURCE      GPIO_PortSourceGPIOB
#define KEY_BUTTON_EXTI_PIN_SOURCE       GPIO_PinSource9
#define KEY_BUTTON_EXTI_IRQn             EXTI9_5_IRQn

void InitLed(Led_TypeDef Led);
void SetLedOn(Led_TypeDef Led);
void SetLedff(Led_TypeDef Led);
void ToggleLed(Led_TypeDef Led);

void InitButton(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t GetButtonState(Button_TypeDef Button);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F103_H */
