#ifndef __MYIIC_H
#define __MYIIC_H

//*********************************************************************************
//IIC驱动
//淘宝店铺:https://shop275468129.taobao.com/
//修改日期:2018/6/25
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州创芯电子 2018-2028
//All rights reserved
//*********************************************************************************
#include "driver_utility.h"
#include "stm32f1xx_hal.h"


//IO操作函数	 
#define IIC_SCL    PBout(10) //SCL
#define IIC_SDA    PBout(11) //SDA	 
#define READ_SDA   PBin(11)  //输入SDA 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(uint8_t txd);			//IIC发送一个字节
uint8_t IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
uint8_t IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  
#endif
















