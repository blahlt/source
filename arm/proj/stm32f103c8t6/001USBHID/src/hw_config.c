#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

ErrorStatus HSEStartUpStatus;
EXTI_InitTypeDef EXTI_InitStructure;

extern __IO uint8_t PrevXferComplete;

static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);

// Configures Main system clocks & power.
void Set_System(void)
{
	// Enable the PWR clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

	/*
	RCC_APB2PeriphClockCmd(MYUSB_Periph, ENABLE);

	GPIO_InitTypeDef  port;

	// Button1
	port.GPIO_Mode = GPIO_Mode_IPU;
	port.GPIO_Mode = GPIO_Mode_IPD;
	port.GPIO_Pin = MYUSB_Pin_Btn1;
	port.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(MYUSB_Port_Btn, &port);
	// Button2
	//port.GPIO_Mode = GPIO_Mode_IPU;
	port.GPIO_Mode = GPIO_Mode_IPD;
	port.GPIO_Pin = MYUSB_Pin_Btn2;
	port.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(MYUSB_Port_Btn, &port);
	// LED1&2
	port.GPIO_Mode = GPIO_Mode_Out_PP;
	port.GPIO_Pin = MYUSB_Pin_Led1 | MYUSB_Pin_Led2 | MYUSB_Pin_Led3;
	port.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(MYUSB_Port_Led, &port);*/

	// Configure the EXTI line 18 connected internally to the USB IP
	EXTI_ClearITPendingBit(EXTI_Line18);
	EXTI_InitStructure.EXTI_Line = EXTI_Line18;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}

// Configures USB Clock input (48MHz).
void Set_USBClock(void)
{
	// Select USBCLK source
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

	// Enable the USB clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

// Power-off system clocks and power while entering suspend mode.
void Enter_LowPowerMode(void)
{
	// Set the device state to suspend
	bDeviceState = SUSPENDED;

	// Clear EXTI Line18 pending bit */
	// EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);

	// Request to enter STOP mode with regulator in low power mode
	PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

// Restores system clocks and power while exiting suspend mode.
void Leave_LowPowerMode(void)
{
	DEVICE_INFO *pInfo = &Device_Info;

	// Set the device state to the correct state
	if (pInfo->Current_Configuration != 0)
	{
		// Device configured
		bDeviceState = CONFIGURED;
	}
	else
	{
		bDeviceState = ATTACHED;
	}

	// Enable SystemCoreClock
	SystemInit();
}

// Configures the USB interrupts.
void USB_Interrupts_Config(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	// 2 bit for pre-emption priority, 2 bits for subpriority
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	// Enable the USB interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	// Enable the USB Wake-up interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Init(&NVIC_InitStructure);
}

// Software Connection/Disconnection of USB Cable.
void USB_Cable_Config (FunctionalState NewState)
{
	if (NewState != DISABLE)
	{
		GPIO_ResetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
	}
	else
	{
		GPIO_SetBits(USB_DISCONNECT, USB_DISCONNECT_PIN);
	}
}

// Decodes the Joystick direction.
uint8_t JoyState(void)
{
	/*
	if (!STM_EVAL_PBGetState(Button_RIGHT)) { return JOY_RIGHT; }
	if (!STM_EVAL_PBGetState(Button_LEFT)) { return JOY_LEFT; }
	if (!STM_EVAL_PBGetState(Button_UP)) { return JOY_UP; }
	if (!STM_EVAL_PBGetState(Button_DOWN)) { return JOY_DOWN; }
	else { return 0; } // No key is pressed
	*/
	//return 0;
	return 1;
}

// prepares buffer to be sent containing Joystick event infos.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
void Joystick_Send(uint8_t Keys)
{
	uint8_t Mouse_Buffer[4] = {0, 0, 0, 0};
	int8_t X = 0, Y = 0;

	/*switch (Keys)
	{
	case JOY_LEFT:
	  X -= CURSOR_STEP;
	  break;
	case JOY_RIGHT:

	  X += CURSOR_STEP;
	  break;
	case JOY_UP:
	  Y -= CURSOR_STEP;
	  break;
	case JOY_DOWN:
	  Y += CURSOR_STEP;
	  break;
	default:
	  return;
	}*/
	// prepare buffer to send
	//Mouse_Buffer[0] = 0;	// Mouse buttons (depends on Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC])
	Mouse_Buffer[1] = X;	// Mouse X (depends on Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC])
	Mouse_Buffer[2] = Y;	// Mouse Y (depends on Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC])
	//Mouse_Buffer[3] = 0;	// Mouse wheel (depends on Joystick_ReportDescriptor[JOYSTICK_SIZ_REPORT_DESC])

	// Reset the control token to inform upper layer that a transfer is ongoing
	PrevXferComplete = 0;

	// Copy mouse position info in ENDP1 Tx Packet Memory Area
	USB_SIL_Write(EP1_IN, Mouse_Buffer, 4);

	// Enable endpoint for transmission
	SetEPTxValid(ENDP1);
}
#pragma GCC diagnostic pop


// Create the serial number string descriptor.
// Pakeicia "'S', 0, 'T', 0, 'M', 0, '3', 0, '2', 0, ..........." reiksme pagal ID0 ir ID2
void Get_SerialNum(void)
{
	uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

	Device_Serial0 = *(uint32_t*)ID1;
	Device_Serial1 = *(uint32_t*)ID2;
	Device_Serial2 = *(uint32_t*)ID3;

	Device_Serial0 += Device_Serial2;

	if (Device_Serial0 != 0)
	{
		IntToUnicode (Device_Serial0, &Joystick_StringSerial[2] , 8);
		IntToUnicode (Device_Serial1, &Joystick_StringSerial[18], 4);
	}
}

// Convert Hex 32Bits value into char.
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
	uint8_t idx = 0;

	for( idx = 0 ; idx < len ; idx ++)
	{
		if(((value >> 28)) < 0xA)
		{
			pbuf[2*idx] = (value >> 28) + '0';
		}
		else
		{
			pbuf[2*idx] = (value >> 28) + 'A' - 10;
		}

		value = value << 4;
		pbuf[2*idx + 1] = 0;
	}
}
