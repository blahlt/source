#include "hw_config.h"
#include "usb_lib.h"
#include "usb_istr.h"

uint8_t Receive_Buffer[2];
extern __IO uint8_t PrevXferComplete;

void EP1_OUT_Callback(void)
{
    BitAction Led_State;

    /* Read received data (2 bytes) */
    USB_SIL_Read(EP1_OUT, Receive_Buffer);

    if (Receive_Buffer[1] == 0)
    {
        Led_State = Bit_RESET;
    }
    else
    {
        Led_State = Bit_SET;
    }

    switch (Receive_Buffer[0])
    {
        case 1: /* Led 1 */
            if (Led_State != Bit_RESET)
            {
                SetLedOn(LED1);
            }
            else
            {
                SetLedOff(LED1);
            }
            break;
        case 2: /* Led 2 */
            if (Led_State != Bit_RESET)
            {
                SetLedOn(LED2);
            }
            else
            {
                SetLedOff(LED2);
            }
            break;
        case 3: /* Led 3 */
            if (Led_State != Bit_RESET)
            {
                SetLedOn(LED3);
            }
            else
            {
                SetLedOff(LED3);
            }
            break;
        case 4: /* Led 4 */
            if (Led_State != Bit_RESET)
            {
                SetLedOn(LED4);
            }
            else
            {
                SetLedOff(LED4);
            }
            break;
        default:
            SetLedOff(LED1);
            SetLedOff(LED2);
            SetLedOff(LED3);
            SetLedOff(LED4);
            break;
    }
    SetEPRxStatus(ENDP1, EP_RX_VALID);
}

void EP1_IN_Callback(void)
{
  PrevXferComplete = 1;
}
