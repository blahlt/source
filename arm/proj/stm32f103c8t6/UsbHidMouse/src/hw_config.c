#include "hw_config.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"

ErrorStatus HSEStartUpStatus;
EXTI_InitTypeDef EXTI_InitStructure;

extern __IO uint8_t PrevXferComplete;

static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power.
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32xxx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32xxx.c file
     */ 

  /* Enable the PWR clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

  /* Set all the GPIOs to AIN *************************************************/
  GPIO_AINConfig();
  
  /* Enable the USB disconnect GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

  /* USB_DISCONNECT used as USB pull-up */
  GPIO_InitTypeDef  GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

  /* Joystick buttons configuration *******************************************/
  /* Configure the Joystick buttons in GPIO mode */
  InitButton(BUTTON_RIGHT, BUTTON_MODE_GPIO);
  InitButton(BUTTON_LEFT, BUTTON_MODE_GPIO);
  InitButton(BUTTON_UP, BUTTON_MODE_GPIO);
  InitButton(BUTTON_DOWN, BUTTON_MODE_GPIO);

  /* Configure the Key button in EXTI mode ************************************/
  //InitButton(BUTTON_KEY, BUTTON_MODE_EXTI);

  /* Configure the EXTI line 18 connected internally to the USB IP ************/
  EXTI_ClearITPendingBit(EXTI_Line18);
  EXTI_InitStructure.EXTI_Line = EXTI_Line18;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);

  EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz).
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* Select USBCLK source */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
  
  /* Enable the USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : GPIO_AINConfig
* Description    : Configures all IOs as AIN to reduce the power consumption.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void GPIO_AINConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable all GPIOs Clock*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ALLGPIO, ENABLE);

  /* Configure all GPIO port pins in Analog Input mode (floating input trigger OFF) */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  //GPIO_Init(GPIOA, &GPIO_InitStructure); // Some GPIOA pins are used for JTAG and USB
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_Init(GPIOC, &GPIO_InitStructure);
  GPIO_Init(GPIOD, &GPIO_InitStructure);
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode.
* Description    : Power-off system clocks and power while entering suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;

  /* Clear EXTI Line18 pending bit */
  EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);

  /* Request to enter STOP mode with regulator in low power mode */
  PWR_EnterSTOPMode(PWR_Regulator_LowPower, PWR_STOPEntry_WFI);
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode.
* Description    : Restores system clocks and power while exiting suspend mode.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;
  
  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
  
  /*Enable SystemCoreClock*/
  SystemInit();
}

/*******************************************************************************
* Function Name  : USB_Interrupts_Config.
* Description    : Configures the USB interrupts.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

  /* Enable the USB interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the USB Wake-up interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_Init(&NVIC_InitStructure);   

  /* Enable the Key EXTI line Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = KEY_BUTTON_EXTI_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : USB_Cable_Config.
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : NewState: new state.
* Output         : None.
* Return         : None
*******************************************************************************/
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

/*******************************************************************************
* Function Name : JoyState.
* Description   : Decodes the Joystick direction.
* Input         : None.
* Output        : None.
* Return value  : The direction value.
*******************************************************************************/
uint8_t JoyState(void)
{
    /* "right" key is pressed */
    if (GetButtonState(BUTTON_RIGHT) == BUTTON_STATE_PRESSED)
    {
        return JOY_RIGHT;
    }

    /* "left" key is pressed */
    if (GetButtonState(BUTTON_LEFT) == BUTTON_STATE_PRESSED)
    {
        return JOY_LEFT;
    }

    /* "up" key is pressed */
    if (GetButtonState(BUTTON_UP) == BUTTON_STATE_PRESSED)
    {
        return JOY_UP;
    }

    /* "down" key is pressed */
    if (GetButtonState(BUTTON_DOWN) == BUTTON_STATE_PRESSED)
    {
        return JOY_DOWN;
    }

    /* No key is pressed */
    return JOY_NONE;
}

/*******************************************************************************
* Function Name : Joystick_Send.
* Description   : prepares buffer to be sent containing Joystick event infos.
* Input         : Keys: keys received from terminal.
* Output        : None.
* Return value  : None.
*******************************************************************************/
void Joystick_Send(uint8_t Keys)
{
  uint8_t Mouse_Buffer[4] = {0, 0, 0, 0};
  int8_t X = 0, Y = 0;
  
  switch (Keys)
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
  }
  /* prepare buffer to send */
  Mouse_Buffer[1] = X;
  Mouse_Buffer[2] = Y;
  
  /* Reset the control token to inform upper layer that a transfer is ongoing */
  PrevXferComplete = 0;
  
  /* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
  USB_SIL_Write(EP1_IN, Mouse_Buffer, 4);
  
  /* Enable endpoint for transmission */
  SetEPTxValid(ENDP1);
}

/*******************************************************************************
* Function Name  : Get_SerialNum.
* Description    : Create the serial number string descriptor.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
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

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;
  
  for(idx = 0; idx < len; idx ++)
  {
    if ((value >> 28) < 0xA)
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
