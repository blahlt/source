#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

void delay();

int main(void)
{
    Set_System();
    Set_USBClock();
    USB_Interrupts_Config();
    USB_Init();

    uint8_t symbol = 0x30; // Ascii table: 0x30 = '0'
    // Circular transmit from '0' to '9'
    while (1)
    {
        delay();
        Add_Data_To_Rx_Buffer(symbol++);
        if (symbol >= 0x3A) // ':'
        {
            symbol = 0x30; // '0'
        }
    }
}

void delay()
{
    for (volatile uint16_t i = 0; i < 0xffff; i++)
    {
        for (volatile uint16_t j = 0; j < 100; j++)
        {
            asm volatile ("nop");
        }
    }
}
