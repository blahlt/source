#include "diag/Trace.h"
#include "stm32f10x.h"
#include "cortexm/ExceptionHandlers.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#define LED_PERIPH			RCC_APB2Periph_GPIOB
#define LED_PORT			GPIOB
#define LED_UPPER_LEFT		GPIO_Pin_13
#define LED_UPPER_RIGHT		GPIO_Pin_12
#define LED_LOWER_LEFT		GPIO_Pin_15
#define LED_LOWER_RIGHT		GPIO_Pin_14

int
main(int argc, char* argv[])
{
	// Init pins
	RCC_APB2PeriphClockCmd(LED_PERIPH, ENABLE);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LED_UPPER_LEFT | LED_UPPER_RIGHT | LED_LOWER_LEFT | LED_LOWER_RIGHT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(LED_PORT, &GPIO_InitStructure);

	// Init interrupt
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Init timer
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 40000;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100;//1800;//100
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV4;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	TIM_Cmd(TIM2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	while (1) {
	}
}

static uint8_t counter = 0;

void TIM2_IRQHandler()
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
    	if (counter > 11)
    		counter = 0;

        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);

        if (counter < 6)
        {
        	GPIO_SetBits(LED_PORT, LED_LOWER_RIGHT);
			GPIO_ResetBits(LED_PORT, LED_LOWER_LEFT);
        	if (counter % 2 == 0)
				GPIO_SetBits(LED_PORT, LED_UPPER_LEFT);
			else
				GPIO_ResetBits(LED_PORT, LED_UPPER_LEFT);
        } else {
        	GPIO_SetBits(LED_PORT, LED_LOWER_LEFT);
        	GPIO_ResetBits(LED_PORT, LED_LOWER_RIGHT);

        	if (counter % 2 == 0)
				GPIO_SetBits(LED_PORT, LED_UPPER_RIGHT);
			else
				GPIO_ResetBits(LED_PORT, LED_UPPER_RIGHT);
        }

    	counter++;
    }
}

#pragma GCC diagnostic pop
