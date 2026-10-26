#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "MPU6050.h"
#include "MyI2C.h"

uint8_t ID;								//定义用于存放ID号的变量
int16_t AX, AY, AZ, GX, GY, GZ;			//定义用于存放各个数据的变量




/* ===== MPU6500/兼容芯片 寄存器地址 ===== */
#define MPU_PWR_MGMT_1      0x6B
#define MPU_PWR_MGMT_2      0x6C
#define MPU_GYRO_CONFIG     0x1B
#define MPU_GYRO_XOUT_H     0x43

/* ===== 底层读写函数（基于你的软件I2C） ===== */
void MPU_WriteReg(uint8_t RegAddr, uint8_t Data)
{
    MyI2C_Start();
    MyI2C_SendByte(0xD0);          // 地址0x68<<1 | 写
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddr);
    MyI2C_ReceiveAck();
    MyI2C_SendByte(Data);
    MyI2C_ReceiveAck();
    MyI2C_Stop();
}

uint8_t MPU_ReadReg(uint8_t RegAddr)
{
    uint8_t Data;
    MyI2C_Start();
    MyI2C_SendByte(0xD0);          // 写
    MyI2C_ReceiveAck();
    MyI2C_SendByte(RegAddr);
    MyI2C_ReceiveAck();
    
    MyI2C_Start();                 // 重复起始
    MyI2C_SendByte(0xD1);          // 读
    MyI2C_ReceiveAck();
    Data = MyI2C_ReceiveByte();
    MyI2C_SendAck(1);              // 给NACK，结束读取
    MyI2C_Stop();
    return Data;
}

/* ===== 核心：初始化并验证写入 ===== */
/* 返回值：0=成功，0xE1=6B写失败，0xE2=6C写失败，0xE3=1B写失败 */
uint8_t MPU_Init_Check(void)
{
    uint8_t check;
    
    // 直接写 0x00 唤醒，跳过复位步骤
    MPU_WriteReg(MPU_PWR_MGMT_1, 0x00);
    Delay_ms(100);  // 多等一会儿，让兼容芯片稳定
    
    check = MPU_ReadReg(MPU_PWR_MGMT_1);
    // 注意：这里不要求 check == 0x00，只要不是睡眠状态下的典型值即可
    // 如果 check 仍是 0x00，但后续读 0x43 有变化，说明唤醒其实成功了
    if(check != 0x00 && check != 0x01) return 0xE1;
    
    // 继续配置其他寄存器...
    MPU_WriteReg(MPU_PWR_MGMT_2, 0x00);
    MPU_WriteReg(MPU_GYRO_CONFIG, 0x18);  // 改用明确的量程值，便于验证写入
    
    check = MPU_ReadReg(MPU_GYRO_CONFIG);
    if(check != 0x18) return 0xE3;  // 如果这里失败，说明读写函数有问题
    
    return 0;
}


int main(void)
{
	   // ... 你的系统初始化、Delay初始化、MyI2C_Init() ...
    	MyI2C_Init();									//先初始化底层的I2C
    uint8_t init_result = MPU_Init_Check();
    
    // 如果初始化失败，进入死循环（可以用调试器看 init_result 的值）
    if(init_result != 0) {
        while(1); 
    }
		
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	MPU6050_Init();		//MPU6050初始化
	
	/*显示ID号*/
	OLED_ShowString(1, 1, "ID:");		//显示静态字符串
	ID = MPU6050_GetID();				//获取MPU6050的ID号
	OLED_ShowHexNum(1, 4, ID, 2);		//OLED显示ID号
	
	while (1)
	{
		MPU6050_GetData(&AX, &AY, &AZ, &GX, &GY, &GZ);		//获取MPU6050的数据
		OLED_ShowSignedNum(2, 1, AX, 5);					//OLED显示数据
		OLED_ShowSignedNum(3, 1, AY, 5);
		OLED_ShowSignedNum(4, 1, AZ, 5);
		OLED_ShowSignedNum(2, 8, GX, 5);
		OLED_ShowSignedNum(3, 8, GY, 5);
		OLED_ShowSignedNum(4, 8, GZ, 5);
		
		
		//测试 角速度的。有问题
		//uint8_t acc_H=MPU_ReadReg(0x6c);
		//uint8_t gyro_h=MPU_ReadReg(0x43);
		
		//OLED_ShowSignedNum(3, 8, acc_H, 5);
		//OLED_ShowSignedNum(4, 8, gyro_h, 5);
	}
}
