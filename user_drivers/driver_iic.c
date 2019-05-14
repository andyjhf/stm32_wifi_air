#include "driver_iic.h"

static void delay_us(uint32_t nTimer)
{
	uint32_t i=0;
	for(i=0;i<nTimer;i++){
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
		__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();__NOP();
	}
}

static void delay_ms(uint32_t nTimer)
{
	uint32_t i=1000*nTimer;
	delay_us(i);
}

#define SCL_PORT 			GPIOB
#define SDA_PORT 			GPIOA

#define SCL_GPIO 			GPIO_PIN_1
#define SDA_GPIO 			GPIO_PIN_7

//*********************************************************************************
//IIC驱动
//淘宝店铺:https://shop275468129.taobao.com/
//修改日期:2018/6/25
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州创芯电子 2018-2028
//All rights reserved
//*********************************************************************************
#define IIC_SDA_HIGH() HAL_GPIO_WritePin(SDA_PORT, SDA_GPIO, GPIO_PIN_SET)
#define IIC_SDA_LOW() HAL_GPIO_WritePin(SDA_PORT, SDA_GPIO, GPIO_PIN_RESET)
#define IIC_SDA_READ() HAL_GPIO_ReadPin(SDA_PORT, SDA_GPIO)

#define IIC_SCL_HIGH() HAL_GPIO_WritePin(SCL_PORT, SCL_GPIO, GPIO_PIN_SET)
#define IIC_SCL_LOW() HAL_GPIO_WritePin(SCL_PORT, SCL_GPIO, GPIO_PIN_RESET)
//初始化IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStruct;
	   
	GPIO_InitStruct.Pin = SDA_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;   //推挽输出
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SDA_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(SDA_PORT, SDA_GPIO, GPIO_PIN_SET);
	
	GPIO_InitStruct.Pin = SCL_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;   //推挽输出
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SCL_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(SCL_PORT, SCL_GPIO, GPIO_PIN_SET);
}

//IO方向设置
void SDA_IN()  
{ 
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin =SDA_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT ;
 	HAL_GPIO_Init(SDA_PORT, &GPIO_InitStruct);
}
void SDA_OUT()
{ 
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Pin = SDA_GPIO;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP ;   //推挽输出
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(SDA_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(SDA_PORT, SDA_GPIO, GPIO_PIN_SET);
}


//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT();     //sda线输出
	IIC_SDA_HIGH();	  	  
	IIC_SCL_HIGH();
	delay_us(4);
 	IIC_SDA_LOW();//START:when CLK is high,DATA change form high to low 
	delay_us(4);
	IIC_SCL_LOW();//钳住I2C总线，准备发送或接收数据 
}	  
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT();//sda线输出
	IIC_SCL_LOW();
	IIC_SDA_LOW();//STOP:when CLK is high DATA change form low to high
 	delay_us(4);
	IIC_SCL_HIGH(); 
	IIC_SDA_HIGH();	//发送I2C总线结束信号
	delay_us(4);							   	
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA设置为输入  
	IIC_SDA_HIGH();delay_us(2);	   
	IIC_SCL_HIGH();delay_us(2);	 
	while(IIC_SDA_READ())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_LOW();//时钟输出0 	   
	return 0;  
} 
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL_LOW();
	SDA_OUT();
	IIC_SDA_LOW();
	delay_us(2);
	IIC_SCL_HIGH();
	delay_us(2);
	IIC_SCL_LOW();
}
//不产生ACK应答		    
void IIC_NAck(void)
{
	IIC_SCL_LOW();
	SDA_OUT();
	IIC_SDA_HIGH();
	delay_us(2);
	IIC_SCL_HIGH();
	delay_us(2);
	IIC_SCL_LOW();
}					 				     
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
		SDA_OUT(); 	    
    IIC_SCL_LOW();//拉低时钟开始数据传输
    for(t=0;t<8;t++)
    {              
		if((txd&0x80)>>7)
			IIC_SDA_HIGH();
		else
			IIC_SDA_LOW();
		txd<<=1; 	  
		delay_us(2);   //对TEA5767这三个延时都是必须的
		IIC_SCL_HIGH();
		delay_us(2); 
		IIC_SCL_LOW();	
		delay_us(2);
    }	 
} 	    
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA设置为输入
    for(i=0;i<8;i++ )
	{
        IIC_SCL_LOW(); 
        delay_us(2);
		IIC_SCL_HIGH();
        receive<<=1;
        if(IIC_SDA_READ())receive++;   
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//发送nACK
    else
        IIC_Ack(); //发送ACK   
    return receive;
}



























