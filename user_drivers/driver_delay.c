#include "driver_delay.h"
#include "cmsis_os.h"

/**
  * @brief   ms延时程序,1us为一个单位
  * @param  
  *		@arg nTime: Delay_us( 1 ) 则实现的延时为 1 * 1us = 1us
  * @retval  无
  */
void Delay_ms(uint32_t nTime)
{ 
	osDelay(nTime);
}

/*********************************************END OF FILE**********************/
