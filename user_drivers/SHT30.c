#include "driver_iic.h"
#include "sht30.h"


//*********************************************************************************
//SHT30����
//�Ա�����:https://shop275468129.taobao.com/
//�޸�����:2018/6/25
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ���ݴ�о���� 2018-2028
//All rights reserved
//*********************************************************************************
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

#define POLYNOMIAL_CXDZ 0x31 // X^8 + X^5 + X^4 + 1
//SHT3X CRCУ��
unsigned char SHT3X_CRC(uint8_t *data, uint8_t len)
{
	unsigned char bit;        // bit mask
	unsigned char crc = 0xFF; // calculated checksum
	unsigned char byteCtr;    // byte counter

	// calculates 8-Bit checksum with given polynomial @GZCXDZ
	for(byteCtr = 0; byteCtr < len; byteCtr++) {
			crc ^= (data[byteCtr]);
			for(bit = 8; bit > 0; --bit) {
					if(crc & 0x80) {
							crc = (crc << 1) ^ POLYNOMIAL_CXDZ;
					}  else {
							crc = (crc << 1);
					}
			}
	}
  return crc;
}

//SHT30�����
void SHT30_CMD(uint16_t cmd)
{
	IIC_Start();
  IIC_Send_Byte(SHT30_ADDR+0);  //�����豸��ַ,д�Ĵ���
	IIC_Wait_Ack();
	IIC_Send_Byte((cmd>>8)&0xff); //MSB
	IIC_Wait_Ack();
	IIC_Send_Byte(cmd&0xff); //LSB
	IIC_Wait_Ack();
	IIC_Stop();
	delay_ms(50);//��������Ҫ�ȴ�20ms���ϲ��ܶ�д
}



//SHT30��ȡ��ʪ��
//temp:�¶ȣ�-400~1250��ʵ���¶�=temp/10,�ֱ���0.1��,���ȡ�0.3��
//humi:ʪ�ȣ�0~1000��ʵ��ʪ��=humi/10,�ֱ���0.1%rh,���ȡ�3
//����0�ɹ���1ʧ��
uint8_t SHT30_Read_Humiture(int *temp,uint16_t *humi)
{
	uint8_t buff[6];
	
	SHT30_CMD(SHT30_READ_HUMITURE);//����ʪ������
	
	IIC_Start();
	IIC_Send_Byte(SHT30_ADDR+1); //�����豸��ַ,���Ĵ���
	IIC_Wait_Ack();
	buff[0]=IIC_Read_Byte(1);//����������Ӧ��
	buff[1]=IIC_Read_Byte(1);//����������Ӧ��
	buff[2]=IIC_Read_Byte(1);//����������Ӧ��
	buff[3]=IIC_Read_Byte(1);//����������Ӧ��
	buff[4]=IIC_Read_Byte(1);//����������Ӧ��
	buff[5]=IIC_Read_Byte(0);//��������ֹͣӦ��
	IIC_Stop();
	
//	printf("buff=%d,%d,%d,%d,%d,%d\r\n",buff[0],buff[1],buff[2],buff[3],buff[4],buff[5]);
	//CRCУ��
	if(SHT3X_CRC(&buff[0],2)==buff[2] && SHT3X_CRC(&buff[3],2)==buff[5])
	{
		*temp=(-45+(175.0*((buff[0]<<8)+buff[1])/65535.0))*10;
		*humi=10*100*((buff[3]<<8)+buff[4])/65535.0;
		if(*temp>1250) *temp=1250;
		else if(*temp<-400) *temp=-400;
		return SUCCESS;
	}
	else return ERROR;	
}

//SHT30��ʼ��
void SHT30_Init()
{
	IIC_Init();
}


