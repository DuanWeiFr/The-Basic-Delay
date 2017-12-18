#include "stm32f10x.h"
volatile uint16_t Tick_Counter = 0;


void delay(uint16_t ms)
{
	Tick_Counter = ms;
	SysTick->CTRL |= 0x01;	//Bit0 is SysTickEnable; Bit1 is InterruptEnable
	while(Tick_Counter != 0);
	SysTick->CTRL &= (~1);
}

int main(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	SysTick_Config(SystemCoreClock/1000);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);

	GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_SET);
	while(1)
	{
//		GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_SET);
		GPIO_SetBits(GPIOB,GPIO_Pin_9);
		/* 在这个地方曾经发生了一个小故事
		在调用这个函数实现切换LED状态时，结果出现了一个很奇怪的情况
		然后软件仿真，发现被置位的是其他的几位
		最后多次调试才发现，函数参数给错了
		实例如下：
		GPIO_SetBits(GPIOB,9)
		结果发现在 stm32f10x_gpio.h 中，GPIO_Pin_9定义为0x0200
		而9则是0x009,相当于GPIO_Pin_3(0x0008) + GPIO_Pin_0x0001 = 0x0009
		所以出现了错误
		 */
		delay(500);
//		GPIO_WriteBit(GPIOB,GPIO_Pin_9,Bit_RESET);
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);
		delay(500);
	}
}

/**
 * @breaf This function is used for handler the Systick Interrupt
**/
void SysTick_Handler(void)
{
	if(Tick_Counter > 0)
	{
		Tick_Counter --;
	}
}
