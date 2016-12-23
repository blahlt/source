#include "hw_config.h"
#include "usb_lib.h"
#include "usb_pwr.h"


__IO uint8_t PrevXferComplete = 1;
__IO uint32_t TimingDelay = 0;

void Delay(__IO uint32_t nCount);

int main(void)
{
  Set_System();

  USB_Interrupts_Config();

  Set_USBClock();

  USB_Init();

  while (1)
  {
  }
}

/*******************************************************************************
* Function Name  : Delay
* Description    : Inserts a delay time.
* Input          : nCount: specifies the delay time length.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(__IO uint32_t nCount)
{
  TimingDelay = nCount;
  for(; nCount!= 0;nCount--);
}
