#include "dht11.h"

void delay_us(uint32_t nTimer)
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

void delay_ms(uint32_t nTimer)
{
	uint32_t i=1000*nTimer;
	delay_us(i);
}

#if 1

//uint8_t U8T_data_H,U8T_data_L,U8RH_data_H,U8RH_data_L,U8checkdata;
//uint8_t U8T_data_H_temp,U8T_data_L_temp,U8RH_data_H_temp,U8RH_data_L_temp,U8checkdata_temp;
 
#define BOOL unsigned char
 
#ifndef TRUE
#define TRUE 1
#endif
 
#ifndef FALSE
#define FALSE 0
#endif

static void DHT11_DataPin_Configure_Output(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
    
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
		GPIO_InitStructure.Pull  = GPIO_NOPULL;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出

    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}
 
static void DHT11_DataPin_Configure_Input(void)
{
		GPIO_InitTypeDef  GPIO_InitStructure;
    
    GPIO_InitStructure.Pin = GPIO_PIN_1;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
		GPIO_InitStructure.Pull  = GPIO_NOPULL;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; //浮空输入
    
    HAL_GPIO_Init(GPIOB, &GPIO_InitStructure);
}
BOOL DHT11_get_databit(void)
{
	uint8_t val;

	val = HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1);
	if(val == GPIO_PIN_RESET){
		return FALSE;
	}else{
		return TRUE;
	}
}
 
void DHT11_set_databit(BOOL level)
{
	if(level == TRUE){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
	}else{
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	}
}
 
void mdelay(uint16_t ms)
{
	if(ms != 0){
		delay_ms(ms);
	}
}
 
void udelay(uint16_t us)
{
	if(us != 0){
		delay_us(us);
	}
}
static uint8_t DHT11_read_byte(void)
{
	uint8_t i;
	uint8_t data = 0;

	for(i = 0; i < 8; i++)
	{
		data <<= 1;
		while((!DHT11_get_databit()));
		udelay(10);
		udelay(10);
		udelay(10);
 
		if(DHT11_get_databit()){
			data |= 0x1;
			while(DHT11_get_databit());
		} else{
	
		} 
 
	}

	return data;
}

static uint8_t DHT11_start_sampling(void)
{
	DHT11_DataPin_Configure_Output();
	//主机拉低18ms? ?
	DHT11_set_databit(FALSE);
	mdelay(18);
	DHT11_set_databit(TRUE);
	//总线由上拉电阻拉高 主机延时20us
	udelay(10);
	udelay(10);
//	udelay(10);
//	udelay(10);
	//主机设为输入 判断从机响应信号 
	DHT11_set_databit(TRUE);
 
	DHT11_DataPin_Configure_Input();
 
	//判断从机是否有低电平响应信号 如不响应则跳出，响应则向下运行?? ? ?
	if(!DHT11_get_databit())//T
	{
		//判断从机是否发出 80us 的低电平响应信号是否结束?? ? 
		while((!DHT11_get_databit()));
		// printf("DHT11 answers.\r\n");
		//判断从机是否发出 80us 的高电平，如发出则进入数据接收状态
		while((DHT11_get_databit()));
		return 1;
	}
 
	return 0;
}
double hum,temp1;
uint16_t DHT11_get_data(uint16_t *temp, uint16_t *humi)
{
	uint8_t databuffer[5];
	uint8_t check_crc;
	if((temp == NULL) || (humi == NULL))
		return 0;
	if(DHT11_start_sampling()){
		//printf("DHT11 is ready to transmit data\r\n");
		//数据接收状态
		databuffer[0] = DHT11_read_byte();
		databuffer[1] = DHT11_read_byte();
		databuffer[2] = DHT11_read_byte();
		databuffer[3] = DHT11_read_byte();
		databuffer[4] = DHT11_read_byte();
		 

		DHT11_DataPin_Configure_Output();
		DHT11_set_databit(TRUE);
		//数据校验 
		 
		check_crc=(databuffer[0]+databuffer[1]+databuffer[2]+databuffer[3]);
		if(check_crc==databuffer[4])
		{
			*temp = databuffer[2]*10 + databuffer[3];
			*humi = databuffer[0]*10 + databuffer[1];
//			printf("temp = %.2f  hum = %.2f \r\n",temp1,hum);
			return 1;
			 
		}
	}
	return 0;
}

#else

void dht11_gpio_input(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    GPIO_InitStructure.Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
		GPIO_InitStructure.Pull  = GPIO_NOPULL;
    GPIO_InitStructure.Mode = GPIO_MODE_INPUT; //浮空输入
    
    HAL_GPIO_Init(DHT11_GPIO_TYPE, &GPIO_InitStructure);
}

void dht11_gpio_output(void)
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    GPIO_InitStructure.Pin = DHT11_GPIO_PIN;
    GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_MEDIUM;
		GPIO_InitStructure.Pull  = GPIO_NOPULL;
    GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP; //推挽输出

    HAL_GPIO_Init(DHT11_GPIO_TYPE, &GPIO_InitStructure);
}

void dht11_reset(void)
{
    // 按照DHT11手册步骤
    dht11_gpio_output();
    DHT11_OUT_L;
    delay_ms(19);
    DHT11_OUT_H;
    delay_us(30);
    dht11_gpio_input();
}

uint16_t dht11_scan(void)
{
    return DHT11_IN;
}

uint16_t dht11_read_bit(void)
{
    while (DHT11_IN == RESET);
    delay_us(40);
    if (DHT11_IN == SET)
    {
        while (DHT11_IN == SET);
        return 1;
    }
    else
    {
        return 0;
    }
}

uint16_t dht11_read_byte(void)
{
    uint16_t i;
    uint16_t data = 0;
    for (i = 0; i < 8; i++)
    {
        data <<= 1;
        data |= dht11_read_bit();
    }
    return data;
}

uint16_t dht11_read_data(uint8_t buffer[5])
{
    uint16_t i = 0;
    
    dht11_reset();
    if (dht11_scan() == RESET)
    {
        //检测到DHT11响应
        while (dht11_scan() == RESET);
        while (dht11_scan() == SET);
        for (i = 0; i < 5; i++)
        {
            buffer[i] = dht11_read_byte();
        }
        
        while (dht11_scan() == RESET);
        dht11_gpio_output();
        DHT11_OUT_H;
        
        uint8_t checksum = buffer[0] + buffer[1] + buffer[2] + buffer[3];
        if (checksum != buffer[4])
        {
            // checksum error
            return 1;
        }
    }
    
    return 0;
}
#endif
