#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

__IO uint8_t PrevXferComplete = 1;

int
main(int argc, char* argv[])
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

#pragma GCC diagnostic pop
