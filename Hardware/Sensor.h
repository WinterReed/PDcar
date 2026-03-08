#ifndef __Sensor_H
#define __Sensor_H

#define IR1_PIN GPIO_Pin_3  // B3 - 最左边传感器
#define IR2_PIN GPIO_Pin_4  // B4
#define IR3_PIN GPIO_Pin_5  // B5 - 中间传感器
#define IR4_PIN GPIO_Pin_6  // B6
#define IR5_PIN GPIO_Pin_7  // B7 - 最右边传感器
#define IR_PORT GPIOB




void Sensor_Init(void);
int Read_IR_Sensors(void);
float Get_Error(void);

#endif
