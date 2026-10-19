#include "stm32f10x.h"
#include "Delay.h"

int main(void)
{
    /* 开启 GPIOA 和 GPIOB 时钟 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;

    /* PB12 推挽输出 —— 蜂鸣器 */
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* PA0~PA7 推挽输出 —— LED */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_All;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* 第一阶段：蜂鸣器响两声 */
    GPIO_ResetBits(GPIOB, GPIO_Pin_12); Delay_ms(100);
    GPIO_SetBits  (GPIOB, GPIO_Pin_12); Delay_ms(100);
    GPIO_ResetBits(GPIOB, GPIO_Pin_12); Delay_ms(100);
    GPIO_SetBits  (GPIOB, GPIO_Pin_12); Delay_ms(700);

    /* 第二阶段：LED 流水灯，一直循环 */
    while (1)
    {
        GPIO_Write(GPIOA, ~0x0001); Delay_ms(100);
        GPIO_Write(GPIOA, ~0x0002); Delay_ms(100);
        GPIO_Write(GPIOA, ~0x0004); Delay_ms(100);
        GPIO_Write(GPIOA, ~0x0008); Delay_ms(100);
        GPIO_Write(GPIOA, ~0x0010); Delay_ms(100);
        GPIO_Write(GPIOA, ~0x0020); Delay_ms(100);
        GPIO_Write(GPIOA, ~0x0040); Delay_ms(100);
        GPIO_Write(GPIOA, ~0x0080); Delay_ms(100);
    }
}