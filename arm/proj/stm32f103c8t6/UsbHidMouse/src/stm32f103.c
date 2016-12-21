#include <stm32f103.h>

GPIO_TypeDef* BUTTON_PORT[BUTTONn] = { KEY_BUTTON_GPIO_PORT, RIGHT_BUTTON_GPIO_PORT, LEFT_BUTTON_GPIO_PORT, UP_BUTTON_GPIO_PORT, DOWN_BUTTON_GPIO_PORT };
const uint16_t BUTTON_PIN[BUTTONn] = { KEY_BUTTON_PIN, RIGHT_BUTTON_PIN, LEFT_BUTTON_PIN, UP_BUTTON_PIN, DOWN_BUTTON_PIN };
const uint32_t BUTTON_CLK[BUTTONn] = { KEY_BUTTON_GPIO_CLK, RIGHT_BUTTON_GPIO_CLK, LEFT_BUTTON_GPIO_CLK, UP_BUTTON_GPIO_CLK, DOWN_BUTTON_GPIO_CLK };
const uint16_t BUTTON_EXTI_LINE[BUTTONn] = { KEY_BUTTON_EXTI_LINE, RIGHT_BUTTON_EXTI_LINE, LEFT_BUTTON_EXTI_LINE, UP_BUTTON_EXTI_LINE, DOWN_BUTTON_EXTI_LINE };
const uint16_t BUTTON_PORT_SOURCE[BUTTONn] = { KEY_BUTTON_EXTI_PORT_SOURCE, RIGHT_BUTTON_EXTI_PORT_SOURCE, LEFT_BUTTON_EXTI_PORT_SOURCE, UP_BUTTON_EXTI_PORT_SOURCE, DOWN_BUTTON_EXTI_PORT_SOURCE };
const uint16_t BUTTON_PIN_SOURCE[BUTTONn] = { KEY_BUTTON_EXTI_PIN_SOURCE, RIGHT_BUTTON_EXTI_PIN_SOURCE, LEFT_BUTTON_EXTI_PIN_SOURCE, UP_BUTTON_EXTI_PIN_SOURCE, DOWN_BUTTON_EXTI_PIN_SOURCE };
const uint16_t BUTTON_IRQn[BUTTONn] = { KEY_BUTTON_EXTI_IRQn, RIGHT_BUTTON_EXTI_IRQn, LEFT_BUTTON_EXTI_IRQn, UP_BUTTON_EXTI_IRQn, DOWN_BUTTON_EXTI_IRQn };

void InitButton(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  EXTI_InitTypeDef EXTI_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the BUTTON Clock */
  RCC_APB2PeriphClockCmd(BUTTON_CLK[Button] | RCC_APB2Periph_AFIO, ENABLE);

  /* Configure Button pin as input floating */
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Pin = BUTTON_PIN[Button];
  GPIO_Init(BUTTON_PORT[Button], &GPIO_InitStructure);

  if (Button_Mode == BUTTON_MODE_EXTI)
  {
    /* Connect Button EXTI Line to Button GPIO Pin */
    GPIO_EXTILineConfig(BUTTON_PORT_SOURCE[Button], BUTTON_PIN_SOURCE[Button]);

    /* Configure Button EXTI line */
    EXTI_InitStructure.EXTI_Line = BUTTON_EXTI_LINE[Button];
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable and set Button EXTI Interrupt to the lowest priority */
    NVIC_InitStructure.NVIC_IRQChannel = BUTTON_IRQn[Button];
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure); 
  }
}

uint32_t GetButtonState(Button_TypeDef Button)
{
  return GPIO_ReadInputDataBit(BUTTON_PORT[Button], BUTTON_PIN[Button]);
}
