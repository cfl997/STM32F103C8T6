#include "stm32f10x.h"                  // Device header


// 毫秒级延时函数
void delay_ms(uint32_t ms)
{
    // SystemCoreClock / 1000 就是 1ms 需要的计数值
    SysTick_Config(SystemCoreClock / 1000); 
    while(ms--)
    {
        while(!((SysTick->CTRL) & (1 << 16))); 
    }
    // 关闭定时器
    SysTick->CTRL &= ~(1 << 0); 
}


int main(void)
{
//	RCC->APB2ENR=0x00000010;
//	GPIOC->CRH=0x00300000;
//	GPIOC->ODR=0x00002000;
	
	// 第一步：开时钟（合上电闸）
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	// 第二步：配置引脚（告诉电工怎么装插座）
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;       // 比如 PC13 通常接板载LED
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	// 第三步：使用引脚（通电了，插座装好了，可以用了）
	GPIO_SetBits(GPIOC,GPIO_Pin_13); //输出高电平，点亮LED
	
	GPIO_ResetBits(GPIOC, GPIO_Pin_13); // 输出低电平
	
	for(int i=0;i<10;i++)
	{
		if(i%2==0)
				GPIO_ResetBits(GPIOC, GPIO_Pin_13); // 输出低电平
		else
				GPIO_SetBits(GPIOC,GPIO_Pin_13); //输出高电平，点亮LED
		    
		delay_ms(500); // 调用上面写的函数，延时 500 毫秒

	}
	while(1)
	{

	}
}
