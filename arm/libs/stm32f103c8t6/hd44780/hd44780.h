#ifndef __HD44780_H
#define __HD44780_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "hal_hd44780.h"

#define HD44780_AfterReset	0b00000011

// Clear Display
// RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
// 0   0   0   0   0   0   0   0   0   1
#define HD44780_Command_ClearDisplay	0b00000001

// Return Home
// RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
// 0   0   0   0   0   0   0   0   1   x
// Return the cursor to the home position. Also returns the
// display being shifted to the original position. DD RAM
// contents remain unchanged.
#define HD44780_Command_ReturnHome		0b00000010

// Entry mode set
// RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
// 0   0   0   0   0   0   0   1  I/D  S
// Set cursor move direct and specifies display shift.These
// operations are performed during data rite/read. For
// normal operation, set S to zero.
// I/D=1 : increment; 0 : decrement;
// S=1 : accompanies display shift when data is written, for normal operation, set to zero.

#define HD44780_Command_EntyModeSet_Increment		0b00000110
#define HD44780_Command_EntyModeSet_Decrement		0b00000100
#define HD44780_Command_EntyModeSet_DisplayShift	0b00000101
#define HD44780_Command_EntyModeSet_DisplayNormal	0b00000100

// Display ON/OFF control
// RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
// 0   0   0   0   0   0   1   D   C   B
// Set ON/OFF all display(D),cursor ON/OFF(C), and
// blink of cursor position character(B).
// D=1 : ON display; 0 : OFF display.
// C=1 : ON cursor; 0 : OFF cursor.
// B=1 : ON blink cursor; 0 : OFF blink cursor.
#define HD44780_Command_DisplayControl_DisplayOn	0b00001100
#define HD44780_Command_DisplayControl_DisplayOff	0b00001000
#define HD44780_Command_DisplayControl_CursorOn		0b00001010
#define HD44780_Command_DisplayControl_CursorOff	0b00001000
#define HD44780_Command_DisplayControl_BlinkOn		0b00001001
#define HD44780_Command_DisplayControl_BlinkOff		0b00001000

// Cursor or Display shift
// RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
// 0   0   0   0   0   1  S/C R/L  x   x
// Move the cursor and shift the display without changing
// DD RAM contents.
// S/C=1 : Display shift; 0 : Cursor move.
// R/L=1 : shift to right; 0 : shift to left.
#define HD44780_Command_CursorOrDisplayShift_DisplayShift	0b00011000
#define HD44780_Command_CursorOrDisplayShift_CursorMove		0b00010000
#define HD44780_Command_CursorOrDisplayShift_ShiftToRight	0b00010100
#define HD44780_Command_CursorOrDisplayShift_ShiftToLeft	0b00010000

// Function Set
// RS R/W DB7 DB6 DB5 DB4 DB3 DB2 DB1 DB0
// 0   0   0   0   1  DL   N   F   x   x
// Set the interface data length (DL). Number of display
// lines (N) and character font (F).
// DL=1 : 8 bits; 0 : 4 bits.
// N=1 : 2 lines; 0 : 1 line.
// F=1 : 5x10 dots; 0 : 5x7 dots.
#define HD44780_Command_FunctionSet_8bits		0b00110000
#define HD44780_Command_FunctionSet_4bits		0b00100000
#define HD44780_Command_FunctionSet_2lines		0b00101000
#define HD44780_Command_FunctionSet_1line		0b00100000
#define HD44780_Command_FunctionSet_5x10dots	0b00100100
#define HD44780_Command_FunctionSet_5x7dots		0b00100000

void HD44780_Initialize(void);
void HD44780_PrintChar(uint8_t c);
void HD44780_PrintString(uint8_t *s);
void HD44780_MoveTo(uint8_t y, uint8_t x);

#ifdef __cplusplus
}
#endif

#endif /* __HD44780_H */
