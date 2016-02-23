#include <stdint.h>
#include "tm4c123gh6pm.h"
void PortD_Init(void);
void Delay(unsigned long time);

int main(void)
{
	long led_read;
	PortD_Init();

	while(1)
	{
		led_read=GPIO_PORTD_DATA_R;
		led_read=led_read^0x09;
		GPIO_PORTD_DATA_R=led_read;
		Delay(1);
	}
}

void PortD_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x08; // Port D clock
	delay = SYSCTL_RCGC2_R; // wait 3-5 bus cycles
	GPIO_PORTD_DIR_R |= 0x09; // PD3 PD0 output
	GPIO_PORTD_AFSEL_R &= (~0x09); // not alternative
	GPIO_PORTD_AMSEL_R &= (~0x09); // no analog
	GPIO_PORTD_PCTL_R &= (~0xF00F);//bits for PD3 PD0
	GPIO_PORTD_DEN_R |= 0x09; // enable PD3, PD0
}

void Delay(unsigned long time)
{
	unsigned long i;
	while(time>0)
	{
		i=1333333;
		while(i>0)
		{
			i=i-1;
		}
		time=time-1;
	}
}

