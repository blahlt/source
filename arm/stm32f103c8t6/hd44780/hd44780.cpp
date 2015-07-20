#include "hd44780.h"

void sendCmd(uint8_t c);
void sendCmdLower4bit(uint8_t c);
void strobeEn(void);
void upNib(uint8_t c);
void downNib(uint8_t c);
void delay(volatile uint32_t n);

void HD44780_Initialize(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(HD44780_Periph, ENABLE);
	GPIO_InitStructure.GPIO_Pin = HD44780_Pin_EN | HD44780_Pin_RS | HD44780_Pin_D4 | HD44780_Pin_D5 | HD44780_Pin_D6 | HD44780_Pin_D7;
	GPIO_ResetBits(HD44780_Port, HD44780_Pin_EN | HD44780_Pin_RS | HD44780_Pin_D4 | HD44780_Pin_D5 | HD44780_Pin_D6 | HD44780_Pin_D7);
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(HD44780_Port, &GPIO_InitStructure);
	GPIO_ResetBits(HD44780_Port, HD44780_Pin_EN | HD44780_Pin_RS | HD44780_Pin_D4 | HD44780_Pin_D5 | HD44780_Pin_D6 | HD44780_Pin_D7);

	// wait ~40 ms and reset 1
	delay(0x7ffff);
	sendCmdLower4bit(HD44780_AfterReset);

	// wait 4.1 ms or more and reset 2
	delay(0xffff);
	sendCmdLower4bit(HD44780_AfterReset);

	// wait 100us or more and reset 3
	delay(0x5dc);
	sendCmdLower4bit(HD44780_AfterReset);

	// 4-bit mode
	sendCmdLower4bit(HD44780_Command_FunctionSet_4bits);

	// Function set (4-bit interface, 2 lines, 5x7 dots)
	sendCmd(HD44780_Command_FunctionSet_4bits | HD44780_Command_FunctionSet_2lines | HD44780_Command_FunctionSet_5x7dots);

	// Display off
	sendCmd(HD44780_Command_DisplayControl_DisplayOff);

	// Clear display
	sendCmd(HD44780_Command_ClearDisplay);

	// Entry mode set (Increment cursor position)
	sendCmd(HD44780_Command_EntyModeSet_Increment);

	// Display On/Off control (Display on, Cursor on, Cursor blink on)
	sendCmd(HD44780_Command_DisplayControl_DisplayOn | HD44780_Command_DisplayControl_CursorOn | HD44780_Command_DisplayControl_BlinkOn);
}

void HD44780_PrintChar(uint8_t c) {
	#pragma GCC diagnostic ignored "-Wtype-limits"
	if ((c >= 0x20 && c <= 0x7F) || (c >= 0xA0 && c <= 0xFF)) { //check if 'c' is within display boundry
		GPIO_SetBits(HD44780_Port, HD44780_Pin_RS);
		upNib(c);
		strobeEn();
		downNib(c);
		strobeEn();
		GPIO_ResetBits(HD44780_Port, HD44780_Pin_RS);
	}
}

void HD44780_PrintString(uint8_t *s) {
	uint8_t i=0;

	while(s[i] != '\0') {
		HD44780_PrintChar(s[i]);
		i++;
	}
}

// y: 0 - first line, >0 - second line
// x: 0 to 39 (0x27) position
void HD44780_MoveTo(uint8_t y, uint8_t x)
{
	if (y > 0) sendCmd((uint8_t)((0x80 + 0x40 + x) & 0xFF));
	else sendCmd((uint8_t)((0x80 + x) & 0xFF));
}

void sendCmd(uint8_t c) {
	GPIO_ResetBits(HD44780_Port, HD44780_Pin_RS);
	upNib(c);
	strobeEn();
	downNib(c);
	strobeEn();
}

void sendCmdLower4bit(uint8_t c) {
	GPIO_ResetBits(HD44780_Port, HD44780_Pin_RS);
	downNib((c & 0xF0) >> 4);
	strobeEn();
}

void strobeEn(void) {
	delay(0x1500);
	GPIO_SetBits(HD44780_Port, HD44780_Pin_EN);
	delay(0x1500);
	GPIO_ResetBits(HD44780_Port, HD44780_Pin_EN);
}

void upNib(uint8_t c) {
	if(c & 0x80) GPIO_SetBits(HD44780_Port, HD44780_Pin_D7);
	else GPIO_ResetBits(HD44780_Port, HD44780_Pin_D7);

	if(c & 0x40) GPIO_SetBits(HD44780_Port, HD44780_Pin_D6);
	else GPIO_ResetBits(HD44780_Port, HD44780_Pin_D6);

	if(c & 0x20) GPIO_SetBits(HD44780_Port, HD44780_Pin_D5);
	else GPIO_ResetBits(HD44780_Port, HD44780_Pin_D5);

	if(c & 0x10) GPIO_SetBits(HD44780_Port, HD44780_Pin_D4);
	else GPIO_ResetBits(HD44780_Port, HD44780_Pin_D4);
}

void downNib(uint8_t c) {
	if(c & 0x8) GPIO_SetBits(HD44780_Port, HD44780_Pin_D7);
	else GPIO_ResetBits(HD44780_Port, HD44780_Pin_D7);

	if(c & 0x4) GPIO_SetBits(HD44780_Port, HD44780_Pin_D6);
	else GPIO_ResetBits(HD44780_Port, HD44780_Pin_D6);

	if(c & 0x2) GPIO_SetBits(HD44780_Port, HD44780_Pin_D5);
	else GPIO_ResetBits(HD44780_Port, HD44780_Pin_D5);

	if(c & 0x1) GPIO_SetBits(HD44780_Port, HD44780_Pin_D4);
	else GPIO_ResetBits(HD44780_Port, HD44780_Pin_D4);
}

// Note : 0xffff ~= 5ms
// To calc: n * 83ns = time in nanoseconds
// 1s/72.000.000Hz ~= 13,9ns per cycle -> 'for' loop ~= 6 cycles -> 6 * 13,9ns ~= 83ns
// 1.000ns = 1us
// 1.000us = 1ms
// 1.000ms = 1s
// ex.: 0xffff(65.535) * 83ns = 5.439.405ns ~= 5.439us ~= 5ms
void delay(volatile uint32_t n)
{
  for(; n != 0; n--);
}
