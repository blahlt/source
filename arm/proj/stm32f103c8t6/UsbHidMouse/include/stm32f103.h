#ifndef __STM32F103_H
#define __STM32F103_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f10x.h"

typedef enum 
{  
  BUTTON_KEY = 0,
  BUTTON_RIGHT = 1,
  BUTTON_LEFT = 2,
  BUTTON_UP = 3,
  BUTTON_DOWN = 4,
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef enum
{
    BUTTON_STATE_RELEASED = Bit_RESET,
    BUTTON_STATE_PRESSED = Bit_SET
} ButtonState_TypeDef;

typedef enum 
{ 
  JOY_NONE = 0,
  JOY_DOWN = 1,
  JOY_LEFT = 2,
  JOY_RIGHT = 3,
  JOY_UP = 4
} JOYState_TypeDef;

#define BUTTONn                          5

#define KEY_BUTTON_PIN                   GPIO_Pin_4
#define KEY_BUTTON_GPIO_PORT             GPIOB
#define KEY_BUTTON_GPIO_CLK              RCC_APB2Periph_GPIOB
#define KEY_BUTTON_EXTI_LINE             EXTI_Line4
#define KEY_BUTTON_EXTI_PORT_SOURCE      GPIO_PortSourceGPIOB
#define KEY_BUTTON_EXTI_PIN_SOURCE       GPIO_PinSource4
#define KEY_BUTTON_EXTI_IRQn             EXTI4_IRQn

#define RIGHT_BUTTON_PIN                 GPIO_Pin_12
#define RIGHT_BUTTON_GPIO_PORT           GPIOB
#define RIGHT_BUTTON_GPIO_CLK            RCC_APB2Periph_GPIOB
#define RIGHT_BUTTON_EXTI_LINE           EXTI_Line12
#define RIGHT_BUTTON_EXTI_PORT_SOURCE    GPIO_PortSourceGPIOB
#define RIGHT_BUTTON_EXTI_PIN_SOURCE     GPIO_PinSource12
#define RIGHT_BUTTON_EXTI_IRQn           EXTI15_10_IRQn

#define LEFT_BUTTON_PIN                  GPIO_Pin_13
#define LEFT_BUTTON_GPIO_PORT            GPIOB
#define LEFT_BUTTON_GPIO_CLK             RCC_APB2Periph_GPIOB
#define LEFT_BUTTON_EXTI_LINE            EXTI_Line13
#define LEFT_BUTTON_EXTI_PORT_SOURCE     GPIO_PortSourceGPIOB
#define LEFT_BUTTON_EXTI_PIN_SOURCE      GPIO_PinSource13
#define LEFT_BUTTON_EXTI_IRQn            EXTI15_10_IRQn

#define UP_BUTTON_PIN                    GPIO_Pin_14
#define UP_BUTTON_GPIO_PORT              GPIOB
#define UP_BUTTON_GPIO_CLK               RCC_APB2Periph_GPIOB
#define UP_BUTTON_EXTI_LINE              EXTI_Line14
#define UP_BUTTON_EXTI_PORT_SOURCE       GPIO_PortSourceGPIOB
#define UP_BUTTON_EXTI_PIN_SOURCE        GPIO_PinSource14
#define UP_BUTTON_EXTI_IRQn              EXTI15_10_IRQn

#define DOWN_BUTTON_PIN                  GPIO_Pin_15
#define DOWN_BUTTON_GPIO_PORT            GPIOB
#define DOWN_BUTTON_GPIO_CLK             RCC_APB2Periph_GPIOB
#define DOWN_BUTTON_EXTI_LINE            EXTI_Line15
#define DOWN_BUTTON_EXTI_PORT_SOURCE     GPIO_PortSourceGPIOB
#define DOWN_BUTTON_EXTI_PIN_SOURCE      GPIO_PinSource15
#define DOWN_BUTTON_EXTI_IRQn            EXTI15_10_IRQn

void InitButton(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t GetButtonState(Button_TypeDef Button);

#ifdef __cplusplus
}
#endif
  
#endif /* __STM32F103_H */
