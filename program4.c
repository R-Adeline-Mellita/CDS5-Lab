#include <stdint.h>
#include "tm4c123gh6pm.h"
void PortD_Init(void);
void Delay(unsigned long time);
unsigned long IN1,IN2;// input from PD0,PD1

int main(void)
{
	long led_read;
	PortD_Init();

	while(1)
	{
		IN1 = GPIO_PORTD_DATA_R&0x01;// READ PD0 into IN1
		IN2 = GPIO_PORTD_DATA_R&0x02;// READ PD1 into IN2

		if(IN1&&IN2) // both not pressed
			GPIO_PORTD_DATA_R = 0x00;// LEDs are OFF
		else if((!IN1)&&IN2)// PF0 IS PRESSED
		{
			led_read=GPIO_PORTD_DATA_R;
			led_read=led_read^0x04;
			GPIO_PORTD_DATA_R=led_read;
			Delay(1);
		}

		else if((IN1&&(!IN2))//PF4 IS PRESSED
		{
			led_read=GPIO_PORTD_DATA_R;
			led_read=led_read^0x08;
			GPIO_PORTD_DATA_R=led_read;
			Delay(1);
		}
		else if((!IN1) && (!IN2))// BOTH ARE PRESSED
		{
			led_read=GPIO_PORTD_DATA_R;
			led_read=led_read^0x0C;
			GPIO_PORTD_DATA_R=led_read;
			Delay(1);
		}
		else
			GPIO_PORTD_DATA_R=0x00;// LED is off
	}

}

void PortD_Init(void)
{
	volatile unsigned long delay;
	SYSCTL_RCGC2_R |= 0x08; // Port D clock
	delay = SYSCTL_RCGC2_R; // wait 3-5 bus cycles
	GPIO_PORTD_DIR_R |= 0x0C; // PD3 PD2 output
	GPIO_PORTD_DIR_R &= (~0x02); // PD1 PD0 input
	GPIO_PORTD_AFSEL_R &= ~0x0F; // not alternative
	GPIO_PORTD_AMSEL_R &= ~0x0F; // no analog
	GPIO_PORTD_PCTL_R &= ~0xFFFF; // bits for PD3 PD2 PD1 PD0
	GPIO_PORTD_DEN_R |= 0x0F; // enable PD3, PD2, PD1, PD0
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
