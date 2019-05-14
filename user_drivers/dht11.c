#include "dht11.h"

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

//#include "am2302.h"
//#include "delay.h"
/*
 *???:
 *??  :
 *??  :
 *??  :
*/
void AM2302_GPIO_Config(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.Pin = AM2302_PIN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //ÍÆÍìÊä³ö

	HAL_GPIO_Init(AM2302_GPIO_PORT, &GPIO_InitStructure);      
		 
	HAL_GPIO_WritePin(AM2302_GPIO_PORT, AM2302_PIN, GPIO_PIN_SET);
 
}

/* 
 * ???:AM2302_Mode_IPU 
 * ??  :?AM2302-DATA???????? 
 * ??  :? 
 * ??  :? 
 */  
static void AM2302_Mode_IPU(void)  
{  
	GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.Pin = AM2302_PIN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT; //¸¡¿ÕÊäÈë
	
	HAL_GPIO_Init(AM2302_GPIO_PORT, &GPIO_InitStructure);
}  

/* 
 * ???:AM2302_Mode_Out_PP 
 * ??  :?AM2302-DATA???????? 
 * ??  :? 
 * ??  :? 
 */  
static void AM2302_Mode_Out_PP(void)  
{  
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.Pin = AM2302_PIN;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
	GPIO_InitStructure.Pull  = GPIO_NOPULL;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //ÍÆÍìÊä³ö

	HAL_GPIO_Init(AM2302_GPIO_PORT, &GPIO_InitStructure);            
} 

static uint8_t Read_Byte(void)  
{
	uint8_t i, temp=0;  

	for(i=0;i<8;i++)      
	{  
		/*?bit?50us???????,???????? ?50us ??? ??*/   
		while(AM2302_DATA_IN()  ==  GPIO_PIN_RESET) {} 

		/*AM2302 ?22~30us??????“0”,?68~75us?????“1”, 
		????60us?????????????*/  

		delay_us(30); //??50us              
		if(AM2302_DATA_IN() == GPIO_PIN_SET)//60us??????????“1”  
		{  
			/*???????????? 30us ?????*/  
			while(AM2302_DATA_IN()==GPIO_PIN_SET);  
			temp|=(uint8_t)(0x01<<(7-i));  //??7-i??1   
		}  
		else  //60us?????????“0”  
		{                 
			temp&=(uint8_t)~(0x01<<(7-i)); //??7-i??0  
		}  
	}  
	return temp;  
}  

uint8_t Read_AM2302(uint16_t *temp, uint16_t *humi)
{
	unsigned char tmp;
	uint8_t buffer[5];
	AM2302_Mode_Out_PP();  
	AM2302_DATA_OUT(HIGH);  
	delay_ms(10); 
	AM2302_DATA_OUT(LOW);  
	delay_ms(1);  
	AM2302_DATA_OUT(HIGH);   
	delay_us(30);
	AM2302_Mode_IPU();  
	delay_us(2);      
	if(AM2302_DATA_IN()==GPIO_PIN_RESET)   //T !     
	{  
		/*???????? ?80us ??? ??????*/   
		while(AM2302_DATA_IN()==GPIO_PIN_RESET){}  

		/*????????? 80us ??? ??????*/  
		while(AM2302_DATA_IN()==GPIO_PIN_SET){}  

		/*??????*/    
		buffer[0] = Read_Byte();
		buffer[1] = Read_Byte();
		buffer[2] = Read_Byte();
		buffer[3] = Read_Byte();
		buffer[4] = Read_Byte();
		/*????,????????*/  
		AM2302_Mode_Out_PP();  
		/*????*/  
		AM2302_DATA_OUT(HIGH);  

		/*???????????*/  
		tmp = buffer[0] + buffer[1] + buffer[2] + buffer[3];
		if(buffer[4] == tmp)
		{
			*humi = buffer[0]*256 + buffer[1];
			*temp = buffer[2]*256 + buffer[3];
			return SUCCESS;
		}	
		else  
			return ERROR;  
	}  
	else  
	{          
	return ERROR;  
	}  
}
