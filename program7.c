#include "PLL.h" 
#include "tm4c123gh6pm.h" 
// Constant declarations to access port registers using 
// symbolic names instead of addresses
void SysTick_Wait10ms(unsigned long delay); 
void DisableInterrupts(void); // Disable interrupts 
void EnableInterrupts(void);  // Enable interrupts 
void WaitForInterrupt(void);  // low power mode 
void PortA_Init(void);        // start sound output 
void PortF_Init(void);
void SysInit(void);
void SysFun(void); 
void SysLoad(unsigned long period);

int main(void)
{  
	PLL_Init();
	SysInit();
	SysFun();  // 80 MHz  
	PortA_Init();   // initialize output and interrupts
	PortF_Init();
	EnableInterrupts();
	/*Initialize ports and timers*/
	/*PA 2,3,4 CONNECTED TO WEST RED YELLOW AND GREEN RES. 
	AND PA 5,6,7 TO SOUTH SIMILARLY. 
	PF1 AND PF3 ARE ON BOARD RED AND GREEN*/  
	while(1)
		{  
			GPIO_PORTA_DATA_R = (0x84);      //west is stopped 
			GPIO_PORTF_DATA_R = (0x08);      //people can walk 
			SysLoad(400000000); 
			GPIO_PORTA_DATA_R = (0x48);    //both yellow 
			GPIO_PORTF_DATA_R = (0x02);      //people should stop 
			SysLoad(80000000); 
			GPIO_PORTA_DATA_R = (0x30);    //west is going and south stop
			SysLoad(520000000);
			GPIO_PORTA_DATA_R = (0x48);    //both yellow 
			SysLoad(160000000);
		} 
}

void PortA_Init(void)
{ 
	unsigned long volatile delay;  
	SYSCTL_RCGC2_R |= 0x00000001; // activate port A  
	delay = SYSCTL_RCGC2_R;  
	GPIO_PORTA_AMSEL_R &= ~0xFC;      // no analog   
	GPIO_PORTA_PCTL_R &= ~0xFFFFFF00; // regular function  
	GPIO_PORTA_DIR_R |= 0xFC;     // make PA2-7 out  
	//GPIO_PORTA_DR8R_R |= 0x20;// can drive up to 8mA out  
	GPIO_PORTA_AFSEL_R &= ~0xFC;  // disable alt funct on PA2-7  
	GPIO_PORTA_DEN_R |= 0xFC;// enable digital I/O on PA5 
}
void PortF_Init(void)
{
	unsigned long volatile delay;
	SYSCTL_RCGC2_R |= 0x00000020;// clock for Port F
	delay = SYSCTL_RCGC2_R;// wait 3-5 bus cycles 	
	GPIO_PORTF_LOCK_R = 0x4C4F434B;//unlock GPIOPortF
	GPIO_PORTF_CR_R = 0x0A; // allow changes to PF1,3
	// only PF0 to be unlocked, other bits can't be
	GPIO_PORTF_AMSEL_R &=~ 0x0A;// disable analog
	GPIO_PORTF_PCTL_R &= ~0x0F0F0;// bits for PF1,3
	GPIO_PORTF_DIR_R |= 0x0A;// PF1 input
	GPIO_PORTF_AFSEL_R &=~ 0x0A;//disable alt func
	GPIO_PORTF_DEN_R = 0x1F;// enable digital I/O
}

void SysLoad(unsigned long period)
{
	NVIC_ST_RELOAD_R = period-1;  // number of counts to wait  
	NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears  
		while((NVIC_ST_CTRL_R&0x00010000)==0)
			{ // wait for count flag  
			}
}

void SysInit(void)
{
	NVIC_ST_CTRL_R = 0;
	NVIC_ST_CURRENT_R = 0;// any write to current clears it
	NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0x00FFFFFF;// priority 0
	NVIC_ST_CTRL_R = 0x00000005;// enable with core clock and interrupts
}
void SysFun(void)
{
	NVIC_ST_CTRL_R = 0;               // disable SysTick during setup  
	NVIC_ST_CTRL_R = 0x00000005;          
} 


