#include "diag/Trace.h"
#include "stm32f10x.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

int
main(int argc, char* argv[])
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	//GPIO structure used to initialize port
	GPIO_InitTypeDef GPIO_InitStructure;
	//select pin to initialize
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	//select output push-pull mode
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	//highest speed available
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	uint8_t i = 0;
	while (1) {
		trace_printf("i=%u\n", i++);
		if (i % 2 == 0)
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
		else
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	}
}

#pragma GCC diagnostic pop
