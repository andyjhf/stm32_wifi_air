#ifndef __DHT11_H__
#define __DHT11_H__

#include "driver_utility.h"
#include "stm32f1xx_hal.h"

#if 1
uint16_t DHT11_get_data(uint16_t *temp, uint16_t *humi);
#else

#define DHT11_GPIO_TYPE  GPIOB
#define DHT11_GPIO_PIN   GPIO_PIN_1
//#define DHT11_RCC        RCC_APB2Periph_GPIOB


#define DHT11_OUT_H HAL_GPIO_WritePin(DHT11_GPIO_TYPE, DHT11_GPIO_PIN, GPIO_PIN_SET)
#define DHT11_OUT_L HAL_GPIO_WritePin(DHT11_GPIO_TYPE, DHT11_GPIO_PIN, GPIO_PIN_RESET)
#define DHT11_IN    HAL_GPIO_ReadPin(DHT11_GPIO_TYPE, DHT11_GPIO_PIN)

void MX_TIM3_Init(void);
void dht11_gpio_input(void);
void dht11_gpio_output(void);
uint16_t dht11_scan(void);
uint16_t dht11_read_bit(void);
uint16_t dht11_read_byte(void);
uint16_t dht11_read_data(uint8_t buffer[4]);
#endif
#endif