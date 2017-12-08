#define PB_ODR *(unsigned char*)0x5005 // Port B data output latch register (Datasheet, 32 page, Table 7. I/O port hardware register map)
#define PB_DDR *(unsigned char*)0x5007 // Port B data direction register (Datasheet, 32 page, Table 7. I/O port hardware register map)
#define PB_CR1 *(unsigned char*)0x5008 // Port B control register 1 (Datasheet, 32 page, Table 7. I/O port hardware register map )

int sine[102] = { // sin(PI/100) * 1000
	0,	31,	63,	94,	125,	156,	187,	218,
	249,	279,	309,	339,	368,	397,	426,	454,
	482,	509,	536,	562,	588,	613,	637,	661,
	685,	707,	729,	750,	771,	790,	809,	827,
	844,	861,	876,	891,	905,	918,	930,	941,
	951,	960,	969,	976,	982,	988,	992,	996,
	998,	1000,	1000,	1000,	998,	996,	992,	988,
	982,	976,	969,	960,	951,	941,	930,	918,
	905,	891,	876,	861,	844,	827,	809,	790,
	771,	750,	729,	707,	685,	661,	637,	613,
	588,	562,	536,	509,	482,	454,	426,	397,
	368,	339,	309,	279,	249,	218,	187,	156,
	125,	94,	63,	31
};

void delay (int time) {
	int d;
	for (d = 0; d < time; d++) { }
}

int main()
{
	char i = 0;
	char j = 0;
	PB_DDR = 0x20;
	PB_CR1 = 0x20;
	do
	{
		for (j = 0; j < 8; j++)
		{
			PB_ODR ^= 0x20;
			delay(sine[i]);
			PB_ODR ^= 0x20;
			delay(1000 - sine[i]);
		}

		if (i++ > 100)
		{
			i = 0;
		}
	} while (1);
}
