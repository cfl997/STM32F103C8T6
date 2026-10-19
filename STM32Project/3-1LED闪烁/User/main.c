#include "stm32f10x.h"
#include "Delay.h"

/*
void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState);
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState);
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState);

void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct);

uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx);
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx);
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal);
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal);

*/
int main(void)
{
	//led 长脚正极  短脚负极
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_InitTypeDef gpioInitStruct;
	gpioInitStruct.GPIO_Mode=GPIO_Mode_Out_PP;//推挽输出  低电平 长脚正极接正极    高电平 长脚正极接引脚A0 短的接负极
	gpioInitStruct.GPIO_Mode=GPIO_Mode_Out_OD;//开漏输出 低电平有驱动能力，高电平 高阻态 没有驱动能力
	gpioInitStruct.GPIO_Pin=GPIO_Pin_0;
	gpioInitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpioInitStruct);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	GPIO_SetBits(GPIOA,GPIO_Pin_0);
	
	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);//熄灭
	GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);//点亮
	
	while(1)
	{
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_SET);//熄灭
		Delay_ms(500);
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,Bit_RESET);//点亮
		Delay_ms(500);
		
		
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,(BitAction)0);//熄灭
		Delay_ms(500);
		GPIO_WriteBit(GPIOA,GPIO_Pin_0,(BitAction)1);//点亮
		Delay_ms(500);
	}
}
