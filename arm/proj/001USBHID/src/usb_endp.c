#include "hw_config.h"
#include "usb_lib.h"
#include "usb_istr.h"

extern __IO uint8_t PrevXferComplete;

// EP1 OUT Callback Routine.
void EP1_IN_Callback(void)
{
	// Set the transfer complete token to inform upper layer that the current transfer has been complete
	PrevXferComplete = 1;
}
