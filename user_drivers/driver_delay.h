#ifndef __SYSTICK_H
#define __SYSTICK_H

#ifdef __cplusplus
	extern "C" {
#endif

#include "driver_utility.h"

void SysTick_Init( void );
void TimingDelay_Decrement( void );
void Delay_ms ( __IO uint32_t nTime );

#ifdef __cplusplus
}
#endif
#endif /* __SYSTICK_H */
