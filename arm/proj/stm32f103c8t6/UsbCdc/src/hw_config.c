#include "stm32_it.h"
#include "usb_lib.h"
#include "usb_prop.h"
#include "usb_desc.h"
#include "hw_config.h"
#include "usb_pwr.h"

ErrorStatus HSEStartUpStatus;
EXTI_InitTypeDef EXTI_InitStructure;

// https://en.wikipedia.org/wiki/Circular_buffer
uint8_t  USART_Rx_Buffer[USART_RX_DATA_SIZE]; // Circular data buffer to be send to host (PC)
uint32_t USART_Rx_ptr_in = 0; // Circular data buffer write (added data) pointer
uint32_t USART_Rx_ptr_out = 0; // Circular data buffer read (send data to host (PC)) pointer

uint32_t USART_Rx_length  = 0;

USBTxState  USB_Tx_State = USBTxState_CheckDataToBeSendAfterSOF;

static void IntToUnicode(uint32_t value , uint8_t *pbuf , uint8_t len);

extern LINE_CODING linecoding;

/*******************************************************************************
* Function Name  : Set_System
* Description    : Configures Main system clocks & power
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_System(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable USB_DISCONNECT GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIO_DISCONNECT, ENABLE);

  /* Configure USB pull-up pin */
  GPIO_InitStructure.GPIO_Pin = USB_DISCONNECT_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_Init(USB_DISCONNECT, &GPIO_InitStructure);

  /* Configure the EXTI line 18 connected internally to the USB IP */
  EXTI_ClearITPendingBit(EXTI_Line18);
  EXTI_InitStructure.EXTI_Line = EXTI_Line18;
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
}

/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
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
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
  /* Set the device state to suspend */
  bDeviceState = SUSPENDED;
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
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
* Function Name  : USB_Interrupts_Config
* Description    : Configures the USB interrupts
* Input          : None.
* Return         : None.
*******************************************************************************/
void USB_Interrupts_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure; 
  
  /* 2 bit for pre-emption priority, 2 bits for subpriority */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  
  
  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the USB Wake-up interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_Init(&NVIC_InitStructure);
}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config(FunctionalState NewState)
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
* Function Name  : Handle_USBAsynchXfer.
* Description    : send data to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void Handle_USBAsynchXfer(void)
{
    if (USB_Tx_State != USBTxState_CheckDataToBeSendAfterSOF)
    {
        return;
    }

    // Reset read pointer to buffer start
    if (USART_Rx_ptr_out == USART_RX_DATA_SIZE)
    {
        USART_Rx_ptr_out = 0;
    }

    // No data to be send
    if (USART_Rx_ptr_out == USART_Rx_ptr_in)
    {
        USB_Tx_State = USBTxState_CheckDataToBeSendAfterSOF;
        return;
    }

    // Get data length to buffer end or difference between read and write pointers
    if (USART_Rx_ptr_out > USART_Rx_ptr_in)
    {
        USART_Rx_length = USART_RX_DATA_SIZE - USART_Rx_ptr_out;
    }
    else
    {
        USART_Rx_length = USART_Rx_ptr_in - USART_Rx_ptr_out;
    }

    uint16_t USB_Tx_ptr = USART_Rx_ptr_out; // Buffer start index to transmit data
    uint16_t USB_Tx_length; // Transmit data length

    // Check max data length to be transmit to USB endpoint
    if (USART_Rx_length > VIRTUAL_COM_PORT_DATA_SIZE)
    {
        USB_Tx_length = VIRTUAL_COM_PORT_DATA_SIZE;

        USART_Rx_ptr_out += VIRTUAL_COM_PORT_DATA_SIZE;
        USART_Rx_length -= VIRTUAL_COM_PORT_DATA_SIZE;
    }
    else
    {
        USB_Tx_length = USART_Rx_length;

        USART_Rx_ptr_out += USART_Rx_length;
        USART_Rx_length = 0;
    }

    USB_Tx_State = USBTxState_SendDataNormal;

    UserToPMABufferCopy(&USART_Rx_Buffer[USB_Tx_ptr], ENDP1_TXADDR, USB_Tx_length);
    SetEPTxCount(ENDP1, USB_Tx_length);
    SetEPTxValid(ENDP1);
}

/*******************************************************************************
* Function Name  : Add_Data_To_Rx_Buffer.
* Description    : send the data to USB.
* Input          : None.
* Return         : none.
*******************************************************************************/
void Add_Data_To_Rx_Buffer(uint8_t data)
{
    if (linecoding.datatype == 7)
    {
      USART_Rx_Buffer[USART_Rx_ptr_in] = data & 0x7F;
    }
    else if (linecoding.datatype == 8)
    {
      USART_Rx_Buffer[USART_Rx_ptr_in] = data;
    }

    USART_Rx_ptr_in++;

    /* To avoid buffer overflow */
    if (USART_Rx_ptr_in == USART_RX_DATA_SIZE)
    {
      USART_Rx_ptr_in = 0;
    }
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
    IntToUnicode (Device_Serial0, &Virtual_Com_Port_StringSerial[2] , 8);
    IntToUnicode (Device_Serial1, &Virtual_Com_Port_StringSerial[18], 4);
  }
}

/*******************************************************************************
* Function Name  : HexToChar.
* Description    : Convert Hex 32Bits value into char.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
static void IntToUnicode(uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;
  
  for (idx = 0; idx < len; idx++)
  {
    if (((value >> 28)) < 0xA)
    {
      pbuf[2*idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2*idx] = (value >> 28) + 'A' - 10;
    }
    
    value = value << 4;
    
    pbuf[2*idx+1] = 0;
  }
}
