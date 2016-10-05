// heartbeat
#define PB_ODR *(unsigned char*)0x5005
#define PB_DDR *(unsigned char*)0x5007
#define PB_CR1 *(unsigned char*)0x5008

// lights
#define PD_ODR *(unsigned char*)0x500F
#define PD_DDR *(unsigned char*)0x5011
#define PD_CR1 *(unsigned char*)0x5012

#define PD5 1<<5
#define PD6 1<<6

int main()
{
    int i = 0;
    PB_DDR = 0x20;
    PB_CR1 = 0x20;

    PD_DDR = PD5 | PD6;
    PD_CR1 = PD5 | PD6;
    do {
        PB_ODR ^= 0x20; // heartbeat
		
        PD_ODR = PD5;
        for (i = 0; i < 20000; i++) { }
        for (i = 0; i < 20000; i++) { }
        PD_ODR = !PD5;
        for (i = 0; i < 20000; i++) { }
        PD_ODR = PD5;
        for (i = 0; i < 30000; i++) { }
        for (i = 0; i < 30000; i++) { }
        for (i = 0; i < 30000; i++) { }

        PD_ODR = PD6;
        for (i = 0; i < 20000; i++) { }
        for (i = 0; i < 20000; i++) { }
        PD_ODR = !PD6;
        for (i = 0; i < 20000; i++) { }
        PD_ODR = PD6;
        for (i = 0; i < 30000; i++) { }
        for (i = 0; i < 30000; i++) { }
        for (i = 0; i < 30000; i++) { }
    } while(1);
}
