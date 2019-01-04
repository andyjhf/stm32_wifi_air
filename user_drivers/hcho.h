#ifndef __HCHO_H
#define __HCHO_H

#include "driver_utility.h"
#include "stm32f1xx_hal.h"

extern int16_t tp_val;
extern uint16_t hm_val;

void hcho_adc_Init(void);
void hcho_adc_sample(void);
void hm_adc_sample(void);



#endif

