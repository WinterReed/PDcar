#include "stm32f10x.h"                  // Device header
#define IR1_PIN GPIO_Pin_3  // B3 - 最左边传感器
#define IR2_PIN GPIO_Pin_4  // B4
#define IR3_PIN GPIO_Pin_5  // B5 - 中间传感器
#define IR4_PIN GPIO_Pin_6  // B6
#define IR5_PIN GPIO_Pin_7  // B7 - 最右边传感器
#define IR_PORT GPIOB


void Sensor_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    // 使能GPIOB时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = IR1_PIN | IR2_PIN | IR3_PIN | IR4_PIN | IR5_PIN ;	

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;  // 上拉输入
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(IR_PORT, &GPIO_InitStructure);
}

// 读取所有红外传感器状态，1=检测到黑线，0=未检测到
int Read_IR_Sensors(void)
{
    int sensor_state = 0;
    
    if(GPIO_ReadInputDataBit(IR_PORT, IR1_PIN)) sensor_state |= 0x01;  // 最左
    if(GPIO_ReadInputDataBit(IR_PORT, IR2_PIN)) sensor_state |= 0x02;
    if(GPIO_ReadInputDataBit(IR_PORT, IR3_PIN)) sensor_state |= 0x04;  // 中间
    if(GPIO_ReadInputDataBit(IR_PORT, IR4_PIN)) sensor_state |= 0x08;
    if(GPIO_ReadInputDataBit(IR_PORT, IR5_PIN)) sensor_state |= 0x10;  // 最右
    
    return sensor_state;
}


// 返回值：负值表示偏左，正值表示偏右
float Get_Error(void)
{    
	float error = 0.0;
    uint8_t sensors = Read_IR_Sensors();

    // 给每个传感器分配权重
    switch(sensors) {
        case 1: error = -10.0; break;  // 00001: 只有最左传感器检测到
        case 2: error = -4.0; break;   // 00010
        case 4: error = 0.0; break;    // 00100: 只有中间传感器检测到
        case 8: error = 4.0; break;    // 01000
        case 16: error = 10.0; break;  // 10000: 只有最右传感器检测到
        
        case 3: error = -10; break;    // 00011
        case 6: error = -2.5; break;   // 00110
        case 12: error = 2.5; break;   // 01100
        case 24: error = 10; break;    // 11000
		
		case 5: error = -10;break;		// 00101
		case 20: error = 10;break;		//10100
        
        case 7: error = -10.0; break;   // 00111
        case 28: error = 10.0; break;   // 11100
        case 14: error = 0.0; break;   // 01110: 三个中间传感器检测到
		
        case 15: error = -10;break;		//01111
		case 30: error = 10;break;		//11110
		
        case 31: error = 0.0; break;   // 11111: 全部检测到（十字路口）
        case 0: error = 99.0; break;   // 00000: 全部未检测到（脱轨）
        
        default: error = 0.0; break;
    }
    
    return error;
}


