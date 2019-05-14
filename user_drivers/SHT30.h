#ifndef __SHT30_H
#define __SHT30_H 
#include "driver_utility.h"
#include "stm32f1xx_hal.h"

//*********************************************************************************
//SHT30����
//�Ա�����:https://shop275468129.taobao.com/
//�޸�����:2018/6/25
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ���ݴ�о���� 2018-2028
//All rights reserved
//*********************************************************************************



#define SHT30_ADDR (uint8_t)(0x44<<1) //sht30 i2c��ַ��ADDR�ܽŽӵ͵�ƽʱΪ0x44,�Ӹߵ�ƽΪ0x45

//SHT30����
#define SHT30_READ_HUMITURE (uint16_t)0x2c06  //����ʪ��

//CRC����ʽ
#define POLYNOMIAL 0x31 // X^8 + X^5 + X^4 + 1

uint8_t SHT30_Read_Humiture(int *temp,uint16_t *humi);
void SHT30_Init(void);

#endif


