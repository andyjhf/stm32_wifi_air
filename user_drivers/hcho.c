#include "hcho.h"
#include "string.h"

//#include "delay.h"

ADC_HandleTypeDef hadc2;

int16_t tp_val = 0;
uint16_t hm_val = 0;


float hcho_value;

void ADC1_2_IRQHandler(void)
{
	HAL_ADC_IRQHandler(&hadc2);

}



void hcho_adc_Init(void)
{
//	ADC_InitTypeDef ADC_InitStructure;
//	ADC_MultiModeTypeDef ADC_MultiModeStructure;
	GPIO_InitTypeDef GPIO_InitStruct;
	ADC_ChannelConfTypeDef sConfig;
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
	__HAL_RCC_ADC2_CLK_ENABLE();


	GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_0,GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_1,GPIO_PIN_SET);



    /**Common config 
    */
  hadc2.Instance = ADC2;
  hadc2.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc2.Init.ContinuousConvMode = DISABLE;
  hadc2.Init.DiscontinuousConvMode = DISABLE;
  hadc2.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc2.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc2.Init.NbrOfConversion = 1;
  if (HAL_ADC_Init(&hadc2) != HAL_OK)
  {
    printf("adc init fail\r\n");
  }

    /**Configure Regular Channel 
    */
  sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    printf("adc config fail\r\n");
  }
	printf("adc config success\r\n");


//	ADC_MultiModeStructure.Mode = ADC_MODE_INDEPENDENT;
//  HAL_ADCEx_MultiModeConfigChannel(&Adc_Handle,&ADC_MultiModeStructure);
	
	
	/* Enable ADC1 */
//	ADC_Cmd(ADC2, ENABLE);
//	ADC_TempSensorVrefintCmd(ENABLE);					 //开启内部温度传感器及Vref通道

	/* Enable ADC1 reset calibaration register */
//	ADC_ResetCalibration(ADC2);							 //ADC复位校正
	/* Check the end of ADC1 reset calibration register */
//	while(ADC_GetResetCalibrationStatus(ADC2));			 //必须等待校正结束

	/* Start ADC1 calibaration */
//	ADC_StartCalibration(ADC2);							 //ADC校正
	/* Check the end of ADC1 calibration */
//	while(ADC_GetCalibrationStatus(ADC2));				 //必须等待校正结束
}

uint16_t tp_tbl[] = {
	3363	,	//	-20
	3329	,	//	-19
	3293	,	//	-18
	3257	,	//	-17
	3220	,	//	-16
	3182	,	//	-15
	3144	,	//	-14
	3104	,	//	-13
	3063	,	//	-12
	3022	,	//	-11
	2980	,	//	-10
	2936	,	//	-9
	2893	,	//	-8
	2848	,	//	-7
	2803	,	//	-6
	2757	,	//	-5
	2711	,	//	-4
	2664	,	//	-3
	2617	,	//	-2
	2569	,	//	-1
	2521	,	//	0
	2473	,	//	1
	2424	,	//	2
	2376	,	//	3
	2327	,	//	4
	2278	,	//	5
	2229	,	//	6
	2180	,	//	7
	2131	,	//	8
	2083	,	//	9
	2037	,	//	10
	1986	,	//	11
	1938	,	//	12
	1891	,	//	13
	1844	,	//	14
	1797	,	//	15
	1751	,	//	16
	1706	,	//	17
	1661	,	//	18
	1617	,	//	19
	1573	,	//	20
	1530	,	//	21
	1487	,	//	22
	1446	,	//	23
	1405	,	//	24
	1365	,	//	25
	1326	,	//	26
	1287	,	//	27
	1249	,	//	28
	1213	,	//	29
	1176	,	//	30
	1141	,	//	31
	1107	,	//	32
	1073	,	//	33
	1040	,	//	34
	1008	,	//	35
	977 ,	//	36
	947 ,	//	37
	918 ,	//	38
	889 ,	//	39
	861 ,	//	40
	834 ,	//	41
	808 ,	//	42
	782 ,	//	43
	757 ,	//	44
	733 ,	//	45
	710 ,	//	46
	687 ,	//	47
	665 ,	//	48
	644 ,	//	49
	621 ,	//	50
	603 ,	//	51
	584 ,	//	52
	565 ,	//	53
	547 ,	//	54
	529 ,	//	55
	512 ,	//	56
	496 ,	//	57
	480 ,	//	58
	464 ,	//	59
	449 ,	//	60
	435 ,	//	61
	421 ,	//	62

0,
0,
};

void hcho_adc_sample(void)
{
	uint16_t result = 0;
	uint16_t tmp = 0;
		uint16_t i = 0;
	ADC_ChannelConfTypeDef sConfig;
	
	sConfig.Channel = ADC_CHANNEL_6;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    printf("adc config fail\r\n");
  }
	printf("adc config success\r\n");
//	ADC_RegularChannelConfig(ADC2, ADC_Channel_6, 1, ADC_SampleTime_239Cycles5);  //PB0引脚，通道8作为第1个转换，采样周期55.5ADC时钟
//  result = HAL_ADC_PollForConversion(&hadc2,5000);
// 	ADC_ClearFlag(ADC2, ADC_FLAG_EOC);						  //清除ADC标志位
 //   	ADC_SoftwareStartConvCmd(ADC2, ENABLE); 				  //软件启动ADC转换
	HAL_ADC_Start(&hadc2);
	result = HAL_ADC_PollForConversion(&hadc2,5000);
 // while(HAL_ADC_GetState(&hadc2)!=HAL_ADC_STATE_REG_EOC);			  //判断ADC转换结束
	printf("result = %d\r\n",result);
	if(result == HAL_OK)
	{	
			printf("Conversion successfully\r\n");
      tmp  = HAL_ADC_GetValue(&hadc2);		  //读取一次PM2.5值，存放于数组
			while(1)
			{
				if(tp_tbl[i] < tmp)
				{
					break;
				}
				else
				{
					i++;
					if(i>= (sizeof(tp_tbl)/sizeof(uint16_t)))
					{
						break;
					}
				}
			}
			//printf("tp_adc = %d\n",tmp);

			tp_val = (tp_tbl[i-1] - tmp)*10 / (tp_tbl[i-1] - tp_tbl[i]) + 10*(i - 21);

			//printf("tp_val = %d.%d\n",tp_val/10,tp_val%10);
			}
	else
		printf("Conversion fail\r\n");
	
	HAL_ADC_Stop(&hadc2);
}


const unsigned int TAD[]={	
//90----80---70---60---50---40---30---20--10
//????
3771,3485,2779,1753,774,262,45,8,3, 
3839,3571,2952,1938,964,354,61,12,5, 
3886,3653,3118,2170,1186,496,90,14,6, 
3920,3726,3273,2429,1412,616,119,19,8, 
3954,3793,3387,2659,1645,757,153,29,11, 
3979,3843,3466,2807,1890,936,209,41,15, 
3998,3886,3563,2952,1973,993,218,49,19, 
4020,3918,3638,3088,2150,1107,284,63,26, 
4036,3946,3716,3239,2354,1284,359,79,35, 
4043,3966,3782,3360,2551,1528,433,107,47, 
4050,3983,3821,3475,2740,1698,534,129,65
};
  
//90->92->95 80->82->87->89 70->76.5 60->65 50->54 40->44 30->31.5
void hm_adc_sample(void)
{
	ADC_ChannelConfTypeDef sConfig;
	uint16_t hm_tmp = 0;
	uint16_t result = 0;
	//HG_ADD---------------------------
	uint16_t Rehum,k,CurAD[9];
	 uint8_t num;//????????
	//HG_ADD_END
	
	sConfig.Channel = ADC_CHANNEL_7;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_239CYCLES_5;
  if (HAL_ADC_ConfigChannel(&hadc2, &sConfig) != HAL_OK)
  {
    printf("adc config fail\r\n");
  }

//	ADC_RegularChannelConfig(ADC2, ADC_Channel_7, 1, ADC_SampleTime_239Cycles5);  //PB0引脚，通道8作为第1个转换，采样周期55.5ADC时钟
  HAL_ADC_Start(&hadc2);
	result = HAL_ADC_PollForConversion(&hadc2,5000);
//	while(HAL_ADC_GetState(&hadc2)!=HAL_ADC_STATE_REG_EOC);	
// 	ADC_ClearFlag(ADC2, ADC_FLAG_EOC);						  //清除ADC标志位
//    	ADC_SoftwareStartConvCmd(ADC2, ENABLE); 				  //软件启动ADC转换
//   	 while(ADC_GetFlagStatus(ADC2,ADC_FLAG_EOC)==0);			  //判断ADC转换结束
	if(result == HAL_OK)
	{
      hm_tmp  = HAL_ADC_GetValue(&hadc2);		  //读取一次PM2.5值，存放于数组
			
	
	//HG ADD----------------------
	//Tem = tp_val;
	 if(tp_val<=0)//0??????0?
	{
		for(num=0;num<9;num++)
		{
	    CurAD[num] = TAD[num];
		}

	}
	else if(tp_val>=500)//??50????
	{
		for(num=0;num<9;num++)
		{
	    CurAD[num] = TAD[90+num];
		}
	}
	else 
	{
	 for(k=1;k<11;k++)//?1??
	 {
	 	if(tp_val<(k*50))	//????10??
		{
		   Rehum = k*50-tp_val;
		   k = 9*k;
			for(num=0;num<9;num++)
			{
			    CurAD[num] = TAD[k+num] - (uint32_t)(TAD[k+num]-TAD[k-9+num])*Rehum/50;//????????
			}
			break;
		}
	 }
	}

	if(hm_tmp<CurAD[8])//10%--
	{
		hm_val =  10-(CurAD[8]-hm_tmp)*10/(CurAD[7]-CurAD[8]);//分辨率1
		if(hm_val>20)hm_val=0;
	}
	else
		for(k=1;k<9;k++)//?1??,???   90%>HumAD>80%
		{
			 if(hm_tmp>=CurAD[k])	//???90%-80%-70%-60%-50%-40%
			{
				hm_val =  (uint32_t)(hm_tmp-CurAD[k])*10/(CurAD[k-1]-CurAD[k])+90-k*10;//分辨率1
				break;
			}	
		}
	//if(hm_val>=980)hm_val=980;//分辨率0.1
		if(hm_val>=98)hm_val=98;

	}
	HAL_ADC_Stop(&hadc2);
}

