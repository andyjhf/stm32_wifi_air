#ifndef __SHT30_H
#define __SHT30_H 
#include "driver_utility.h"
#include "stm32f1xx_hal.h"

//*********************************************************************************
//SHT30驱动
//淘宝店铺:https://shop275468129.taobao.com/
//修改日期:2018/6/25
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州创芯电子 2018-2028
//All rights reserved
//*********************************************************************************



#define SHT30_ADDR (uint8_t)(0x44<<1) //sht30 i2c地址，ADDR管脚接低电平时为0x44,接高电平为0x45

//SHT30命令
#define SHT30_READ_HUMITURE (uint16_t)0x2c06  //读温湿度

//CRC多项式
#define POLYNOMIAL 0x31 // X^8 + X^5 + X^4 + 1

uint8_t SHT30_Read_Humiture(int *temp,uint16_t *humi);
void SHT30_Init(void);

#endif


