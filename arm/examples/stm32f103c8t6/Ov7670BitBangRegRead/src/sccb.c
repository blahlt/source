#include "sccb.h"

void gpio_init_input(GPIO_TypeDef*, uint16_t);
void gpio_init_output(GPIO_TypeDef*, uint16_t);
void delay();

void sccb_init()
{
    RCC_APB2PeriphClockCmd(GPIO_SCCB_CLOCK_Periph | GPIO_SCCB_DATA_Periph, ENABLE);

    gpio_init_output(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
    GPIO_SetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);

    gpio_init_output(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin);
    GPIO_SetBits(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin);
}

void sccb_start()
{
    GPIO_SetBits(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin);
    GPIO_SetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
    delay();

    GPIO_ResetBits(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin);
    delay();

    GPIO_ResetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
    delay();
}

void sccb_stop()
{
    GPIO_ResetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
    GPIO_ResetBits(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin);
    delay();

    GPIO_SetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
    delay();

    GPIO_SetBits(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin);
    delay();
}

uint8_t sccb_write(uint8_t data)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        GPIO_ResetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
        delay();

        if (data & 0x80)
        {
            GPIO_SetBits(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin);
        }
        else
        {
             GPIO_ResetBits(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin);
        }
        data = data << 1;
        delay();

        GPIO_SetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
        delay();
    }

    // don't care
    GPIO_ResetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
    delay();

    GPIO_SetBits(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin);
    delay();

    GPIO_SetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
    delay();

    uint8_t ack = 1; // error
    if (GPIO_ReadInputDataBit(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin) == Bit_RESET)
    {
        ack = 0; // ok
    }

    GPIO_ResetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
    delay();

    return ack;
}

uint8_t sccb_read()
{
    gpio_init_input(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin);
    delay();

    uint8_t value = 0x00;
    for (uint8_t i = 8; i > 0; i--)
    {
        GPIO_SetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
        delay();

        value = value << 1;
        if (GPIO_ReadInputDataBit(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin))
        {
            value += 1;
        }

        GPIO_ResetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
        delay();
    }

    gpio_init_output(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin);
    delay();

    // NA
    GPIO_SetBits(GPIO_SCCB_DATA_Port, GPIO_SCCB_DATA_Pin);
    delay();

    GPIO_SetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
    delay();

    GPIO_ResetBits(GPIO_SCCB_CLOCK_Port, GPIO_SCCB_CLOCK_Pin);
    delay();

    return value;
}

void gpio_init_input(GPIO_TypeDef* gpio, uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(gpio, &GPIO_InitStructure);
}

void gpio_init_output(GPIO_TypeDef* gpio, uint16_t pin)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = pin;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(gpio, &GPIO_InitStructure);
}

void delay()
{
    for (int i = 0; i < 400; i++)
    {
        asm volatile ("nop");
    }
}
