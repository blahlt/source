#include "usb_lib.h"
#include "usb_pwr.h"
#include "hw_config.h"

__IO uint8_t PrevXferComplete = 1;

int
main(void)
{
    Set_System();
    USB_Interrupts_Config();
    Set_USBClock();
    USB_Init();
    while (1)
    {
        if (bDeviceState == CONFIGURED)
        {
            if ((JoyState() != 0) && (PrevXferComplete))
            {
                Joystick_Send(JoyState());
            }
        }
    }
}
