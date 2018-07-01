
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2018 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
//#include "cmsis_os.h"
#include "XApp.h"
//#include "stm32f1xx_hal.h"
//#include "driver_gpio.h"
//#include "driver_rcc.h"
#include "driver_uart1.h"
#include "driver_uart2.h"


osThreadId defaultTaskHandle;

//uint32_t led_blinkcnt = 0;
//uint32_t led_blinktime = 1000;
//static void LED_blink(uint16_t tmOnce)
//{
//	if(led_blinkcnt > led_blinktime)
//	{
//		LED_Toggle(LED1);
//		led_blinkcnt = 0;
//	}
//	led_blinkcnt++;

//}

void StartDefaultTask(void const * argument)
{

  /* USER CODE BEGIN 5 */
  /* Infinite loop */
  for(;;)
  {
		LED_Toggle(LED1);
    osDelay(1000);
  }
  /* USER CODE END 5 */ 
}

/**
  * @brief  The application entry point.
  *
  * @retval None
  */
int main(void)
{
	
  HAL_Init();
  SystemClock_Config();
  GPIO_CLK_Init();
	USART2_Init();
	LED_Init(LED1);
	SWI_Init(SW1_1);
	if(!SWI_GetState(SW1_1))
		g_SmartConfig = 1;
	printf("system init\r\n");
	USART1_Init();
	osThreadDef(defaultTask, StartDefaultTask, osPriorityNormal, 0, 128);
  defaultTaskHandle = osThreadCreate(osThread(defaultTask), NULL);
	XTaskEsp8266_Init();
//	CXTaskComERV_InitTask();
	
	osKernelStart();
  /* USER CODE BEGIN 2 */
  while (1)
  {
		osDelay(1000);

  }
  

}



/* USER CODE BEGIN 4 */

/* USER CODE END 4 */





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
