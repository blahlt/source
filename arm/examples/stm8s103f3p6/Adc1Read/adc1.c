#define PB_ODR *(unsigned char*)0x5005 // Port B data output latch register (Datasheet, 32 page, Table 7. I/O port hardware register map)
#define PB_DDR *(unsigned char*)0x5007 // Port B data direction register (Datasheet, 32 page, Table 7. I/O port hardware register map)
#define PB_CR1 *(unsigned char*)0x5008 // Port B control register 1 (Datasheet, 32 page, Table 7. I/O port hardware register map )

#define ADC_CSR *(unsigned char*)0x005400
#define ADC_CR1 *(unsigned char*)0x005401
#define ADC_CR2 *(unsigned char*)0x005402
#define ADC_DRH *(unsigned char*)0x005404
#define ADC_DRL *(unsigned char*)0x005405

void myDelay(unsigned int value)
{
     unsigned int i,j;
     for(i=0;i<1000;i++)
     {
         for(j=0;j<value;j++);
     }
}

void GPIO_Config(void)
{ 
 	PB_DDR = 0x20;
	PB_CR1 = 0x20;
}

unsigned int readADC1(unsigned int channel) 
{
     unsigned int val=0;
     //using ADC in single conversion mode
     ADC_CSR |= ((0x0F)&channel); // select channel
     ADC_CR2 |= (1<<3); // Right Aligned Data
     ADC_CR1 |= (1<<0); // ADC ON 
     ADC_CR1 |= (1<<0); // ADC Start Conversion
     while(((ADC_CSR)&(1<<7))== 0); // Wait till EOC
     val |= (unsigned int)ADC_DRL;
     val |= (unsigned int)ADC_DRH<<8;
     ADC_CR1 &= ~(1<<0); // ADC Stop Conversion
     val &= 0x03ff;
     return (val);
}

int main(void)
{
      unsigned int stepNos;
      GPIO_Config();
 
      while(1)
      {
            stepNos=readADC1(3); // Channel 3 - PD2 (AIN3)
            PB_ODR ^= 0x20;
            myDelay(stepNos);
            PB_ODR ^= 0x20;
            myDelay(stepNos);
       }
}
