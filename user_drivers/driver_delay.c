#include "driver_delay.h"
#include "cmsis_os.h"

/**
  * @brief   ms��ʱ����,1usΪһ����λ
  * @param  
  *		@arg nTime: Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 1us = 1us
  * @retval  ��
  */
void Delay_ms(uint32_t nTime)
{ 
	osDelay(nTime);
}

/*********************************************END OF FILE**********************/
