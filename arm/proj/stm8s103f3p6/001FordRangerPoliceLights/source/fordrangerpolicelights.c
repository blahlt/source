#define DEBUG 0

#if DEBUG

// heartbeat
#define PB_ODR *(unsigned char*)0x5005
#define PB_DDR *(unsigned char*)0x5007
#define PB_CR1 *(unsigned char*)0x5008

#define PB5 1<<5

#endif /* DEBUG */

// lights
#define PC_ODR *(unsigned char*)0x500A
#define PC_DDR *(unsigned char*)0x500C
#define PC_CR1 *(unsigned char*)0x500D

#define PC4 1<<4 // Upper right (BLUE)
#define PC5 1<<5 // Upper left (RED)
#define PC6 1<<6 // Lower center (BLUE)
#define PC7 1<<7 // Lower side (RED)


int main()
{
    int i = 0;

    #if DEBUG

    PB_DDR = PB5;
    PB_CR1 = PB5;

    #endif /* DEBUG */

    PC_DDR = PC4 | PC5 | PC6 | PC7;
    PC_CR1 = PC4 | PC5 | PC6 | PC7;
    
    do {
        #if DEBUG

        PB_ODR ^= PB5; // heartbeat

        #endif /* DEBUG */

        PC_ODR |= PC6;

        PC_ODR |= PC4;
        for (i = 0; i < 30000; i++) { }
        for (i = 0; i < 30000; i++) { }
        for (i = 0; i < 30000; i++) { }
        PC_ODR &= ~PC4;
        for (i = 0; i < 10000; i++) { }
        PC_ODR |= PC4;
        for (i = 0; i < 30000; i++) { }
        for (i = 0; i < 30000; i++) { }
        for (i = 0; i < 30000; i++) { }
        PC_ODR &= ~PC4;

        PC_ODR &= ~PC6;

        PC_ODR |= PC7;

        PC_ODR |= PC5;
        for (i = 0; i < 30000; i++) { }
        for (i = 0; i < 30000; i++) { }
        for (i = 0; i < 30000; i++) { }
        PC_ODR &= ~PC5;
        for (i = 0; i < 10000; i++) { }
        PC_ODR |= PC5;
        for (i = 0; i < 30000; i++) { }
        for (i = 0; i < 30000; i++) { }
        for (i = 0; i < 30000; i++) { }
        PC_ODR &= ~PC5;

        PC_ODR &= ~PC7;
    } while(1);
}
