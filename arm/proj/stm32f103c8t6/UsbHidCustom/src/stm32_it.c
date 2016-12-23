#include "stm32_it.h"
#include "usb_istr.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "hw_config.h"

__IO uint8_t Send_Buffer[2];
extern __IO uint8_t PrevXferComplete;
extern uint32_t ADC_ConvertedValueX;
extern uint32_t ADC_ConvertedValueX_1;
extern __IO uint32_t TimingDelay;

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
    TimingDelay--;
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
  USB_Istr();
}

void DMA1_Channel1_IRQHandler(void)
{  
    Send_Buffer[0] = 0x07;
  
    if ((ADC_ConvertedValueX >> 4) - (ADC_ConvertedValueX_1 >> 4) > 10)
    {
        if ((PrevXferComplete) && (bDeviceState == CONFIGURED))
        {
            Send_Buffer[1] = (uint8_t)(ADC_ConvertedValueX >> 4);
      
            /* Write the descriptor through the endpoint */
            USB_SIL_Write(EP1_IN, (uint8_t*) Send_Buffer, 2);
            SetEPTxValid(ENDP1);
            ADC_ConvertedValueX_1 = ADC_ConvertedValueX;
            PrevXferComplete = 0;
        }
    }

    DMA_ClearFlag(DMA1_FLAG_TC1);
}

void EXTI9_5_IRQHandler(void)
{
    if(EXTI_GetITStatus(KEY_BUTTON_EXTI_LINE) != RESET)
    {
        if ((PrevXferComplete) && (bDeviceState == CONFIGURED))
        {
            Send_Buffer[0] = 0x05;

            if (GetButtonState(BUTTON_KEY) == Bit_SET)
            {
                Send_Buffer[1] = 0x01;
            }
            else
            {
                Send_Buffer[1] = 0x00;
            }

            /* Write the descriptor through the endpoint */
            USB_SIL_Write(EP1_IN, (uint8_t*) Send_Buffer, 2);
            SetEPTxValid(ENDP1);
            PrevXferComplete = 0;
        }

        /* Clear the EXTI line  pending bit */
        EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
    }
}

void EXTI15_10_IRQHandler(void)
{
    if(EXTI_GetITStatus(TAMPER_BUTTON_EXTI_LINE) != RESET)
    {
        if ((PrevXferComplete) && (bDeviceState == CONFIGURED))
        {
            Send_Buffer[0] = 0x06;
      
            if (GetButtonState(BUTTON_TAMPER) == Bit_SET)
            {
                Send_Buffer[1] = 0x01;
            }
            else
            {
                Send_Buffer[1] = 0x00;
            }

            /* Write the descriptor through the endpoint */
            USB_SIL_Write(EP1_IN, (uint8_t*) Send_Buffer, 2);

            SetEPTxValid(ENDP1);

            PrevXferComplete = 0;
        }
        /* Clear the EXTI line 13 pending bit */
        EXTI_ClearITPendingBit(TAMPER_BUTTON_EXTI_LINE);
    }
}

void USBWakeUp_IRQHandler(void)
{
  EXTI_ClearITPendingBit(EXTI_Line18);
}
