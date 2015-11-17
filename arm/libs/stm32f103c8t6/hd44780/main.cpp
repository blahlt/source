#include "hd44780.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

//
// stm32f103c8t6 and hd44780 lcd example
//
int
main(int argc, char* argv[])
{
	HD44780_Initialize();
	HD44780_PrintString((uint8_t*)"1st line");
	HD44780_MoveTo(1, 0); // 2nd line, 1 position
	HD44780_PrintString((uint8_t*)"2nd line");
	while (1)
	{

	}
}

#pragma GCC diagnostic pop
