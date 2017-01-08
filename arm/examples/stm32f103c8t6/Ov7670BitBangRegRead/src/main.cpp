#include "sccb.h"

void xclk_init();

#pragma GCC diagnostic ignored "-Wunused-parameter"
int
main(int argc, char* argv[])
{
    xclk_init();

    sccb_init();

    uint8_t reg_address = REG_PID;
    //uint8_t reg_address = REG_VER;

    sccb_start();

    if (sccb_write(OV7670_ADDRESS_WRITE) == 1)
    {
        sccb_stop();
        return 0;
    }

    if (sccb_write(reg_address) == 1)
    {
        sccb_stop();
        return 0;
    }

    sccb_stop();

    sccb_start();

    if (sccb_write(OV7670_ADDRESS_READ) == 1)
    {
        sccb_stop();
        return 0;
    }

    #pragma GCC diagnostic ignored "-Wunused-variable"
    uint8_t register_value = sccb_read();

    sccb_stop();

    while (1)
    {
    }
}

void xclk_init()
{
    //
    // Output init
    //
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitTypeDef GPIO_Structure;
    GPIO_Structure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Structure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_Structure);

    //
    // Toggle output init
    //
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    //
    // Output Comparer init
    //
    TIM_OCInitTypeDef TIM_OCStructure;
    TIM_OCStructInit(&TIM_OCStructure);

    TIM_OCStructure.TIM_OCMode = TIM_OCMode_Toggle;
    TIM_OCStructure.TIM_OutputState = TIM_OutputState_Enable;

    TIM_OC1Init(TIM2, &TIM_OCStructure);

    //
    // Timer init
    //
    TIM_TimeBaseInitTypeDef TIM_Structure;
    TIM_TimeBaseStructInit(&TIM_Structure);

    // The objective is to get TIM2 counter clock at 50kHz:
    // Prescaler = (TIM2CLK / TIM2 counter clock) - 1
    //uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 50000) - 1;

    uint16_t PrescalerValue = (uint16_t) (SystemCoreClock / 72000000) - 1;

    // The prescaler can divide the counter clock frequency by any factor between 1 and 65536. It
    // is based on a 16-bit counter controlled through a 16-bit register (in the TIMx_PSC register).
    TIM_Structure.TIM_Prescaler = PrescalerValue; // PSC

    uint16_t Period = 2;

    // In upcounting mode, the counter counts from 0 to the auto-reload value (content of the
    // TIMx_ARR register), then restarts from 0 and generates a counter overflow event.
    TIM_Structure.TIM_Period = Period; // ARR

    TIM_TimeBaseInit(TIM2, &TIM_Structure);

    TIM_Cmd(TIM2, ENABLE);
}
