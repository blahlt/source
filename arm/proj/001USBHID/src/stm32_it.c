#include "stm32_it.h"
#include "usb_istr.h"
#include "usb_lib.h"
#include "usb_pwr.h"
#include "platform_config.h"

// Cortex-M Processor Exceptions Handlers

// This function handles NMI exception.
void NMI_Handler(void)
{
}

// This function handles Hard Fault exception.
void HardFault_Handler(void)
{
	/* Go to infinite loop when Hard Fault exception occurs */
	while (1)
	{
	}
}

// This function handles Memory Manage exception.
void MemManage_Handler(void)
{
	/* Go to infinite loop when Memory Manage exception occurs */
	while (1)
	{
	}
}

// This function handles Bus Fault exception.
void BusFault_Handler(void)
{
	/* Go to infinite loop when Bus Fault exception occurs */
	while (1)
	{
	}
}

// This function handles Usage Fault exception.
void UsageFault_Handler(void)
{
	/* Go to infinite loop when Usage Fault exception occurs */
	while (1)
	{
	}
}

// This function handles SVCall exception.
void SVC_Handler(void)
{
}

// This function handles Debug Monitor exception.
void DebugMon_Handler(void)
{
}

// This function handles PendSVC exception.
void PendSV_Handler(void)
{
}

// This function handles SysTick Handler.
void SysTick_Handler(void)
{
}

// STM32 Peripherals Interrupt Handlers
// This function handles USB Low Priority interrupts requests.
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	USB_Istr();
}

// This function handles USB WakeUp interrupt request.
void USBWakeUp_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line18);
}
