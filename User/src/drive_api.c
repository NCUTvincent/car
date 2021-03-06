/*---------------------------------------------------------------------------------------------
    Author       : 姜万勇

    Created Date : 2015-01-12

    Descriptions : 底层控制api
      
    Version             Description             Date                Author
    0.1                 Created                 2015-01-12       姜万勇

---------------------------------------------------------------------------------------------*/
#include "typedef.h"
#include <string.h>

extern CMD_PROCESS cmdProcess;
int deviceInit(void)
{
	int ret =0;
	USART_InitTypeDef usartInfo;	
//	EXTI_InitTypeDef 	 extiInfo;
	GPIO_InitTypeDef  gpioInfo;
	NVIC_InitTypeDef nvicInfo;

		
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA |RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2 | RCC_APB1Periph_USART3, ENABLE);
		
	gpioInfo.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_3;
	gpioInfo.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &gpioInfo);
	
	gpioInfo.GPIO_Pin = GPIO_Pin_11;
	gpioInfo.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &gpioInfo);
	  
	gpioInfo.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_2;
	gpioInfo.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInfo.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &gpioInfo);

	gpioInfo.GPIO_Pin = GPIO_Pin_10;
	gpioInfo.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInfo.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOB, &gpioInfo);

	USART_DeInit(USART1);
	USART_DeInit(USART2);
	USART_DeInit(USART3);
	USART_StructInit(&usartInfo);
	usartInfo.USART_BaudRate = 115200;
	usartInfo.USART_WordLength = 8;
	
	USART_Init(USART1, &usartInfo);
	USART_Init(USART2, &usartInfo);
	USART_Init(USART3, &usartInfo);
	
	USART_Cmd(USART1, ENABLE);
	USART_Cmd(USART2, ENABLE);
	USART_Cmd(USART3, ENABLE);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
	while( USART_GetFlagStatus(USART1,USART_FLAG_TC)!= SET);  
	while( USART_GetFlagStatus(USART2,USART_FLAG_TC)!= SET);  
	while( USART_GetFlagStatus(USART3,USART_FLAG_TC)!= SET);  
	

//	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
//	SysTick_Config(72000000 /100);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	nvicInfo.NVIC_IRQChannel = USART1_IRQn;
	nvicInfo.NVIC_IRQChannelPreemptionPriority = 0;
	nvicInfo.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&nvicInfo);


	return ret;
}

int carCal(CAR_INFO * carInfo)
{
	int ret = 0;
	int i,j;
	char * usartSpeedOut 	= carInfo->usartSpeedOut;
	char * usartDirectOut 	= carInfo->usartDirectOut;
	signed char udpSpeedIn 		= carInfo->udpSpeedIn;
//	char udpSpeedOut 	= carInfo->udpSpeedOut;
	unsigned int udpDirectIn 		= carInfo->udpDirectIn;
//	char udpDirectOut 	= carInfo->udpDirectOut;
	char r[5];
	short y;

	memset(usartSpeedOut, 0, 10);
	memset(usartDirectOut, 0, 10);
	
	if(udpSpeedIn > 100)
		udpSpeedIn = 100;
	if(udpSpeedIn < -100)
		udpSpeedIn = -100;
//////////////////////////////////////////////////////////////
	usartSpeedOut[0] = 0x76;//v
	if(udpSpeedIn > 0)
	{	
		y = udpSpeedIn * 120;
		j = 1;
	}
	if(udpSpeedIn < 0)
	{
		y = -udpSpeedIn * 120;
		usartSpeedOut[1] = 0x2d;
		j = 2;
	}
	if(udpSpeedIn == 0)
	{
		y = 0;
		usartSpeedOut[1] = 0x30;
		j = 2;
	}
	
	for(i = 4; i >= 0; i --)
	{
		r[i] = y % 10;
		y = y / 10;
	}
	for(i = 0; i < 5; i++)
	{
		if(r[i])
		{
			break;
		}
	}
	for( ; i < 5; i ++)
	{
		usartSpeedOut[j] = 0x30 + r[i];
		j ++;
	}
	usartSpeedOut[j] = 0x0d;
//////////////////////////////////////////////////////////////////	
	usartDirectOut[0] = 0xff;
	usartDirectOut[1] = 0xff;
	usartDirectOut[2] = 0x00;
	usartDirectOut[3] = 0x07;
	usartDirectOut[4] = 0x03;
	usartDirectOut[5] = 0x1e;

	usartDirectOut[6] = udpDirectIn;
	usartDirectOut[7] = udpDirectIn >> 8;
	usartDirectOut[8] = udpDirectIn >> 16;
	usartDirectOut[9] = udpDirectIn >>24;
	usartDirectOut[10] = ~(usartDirectOut[2] + usartDirectOut[3] + usartDirectOut[4] + usartDirectOut[5] + usartDirectOut[6] + usartDirectOut[7] + usartDirectOut[8] + usartDirectOut[9]); 


	return ret;
}
int carSpeedOut(CAR_INFO * carInfo)
{
	int ret = 0;
	char i;
	char * usartSpeedOut = carInfo->usartSpeedOut;

	for(i = 0; i < 10; i ++)
	{
		if(usartSpeedOut[i])
		{
			USART_SendByte(USART2,usartSpeedOut[i]);
		}
	}

	return ret;
}

int carDirectOut(CAR_INFO * carInfo)
{
	int ret =0;
	char i;
	char * usartDirectOut = carInfo->usartDirectOut;

	for(i = 0; i < 11; i ++)
	{
		USART_SendByte(USART3,usartDirectOut[i]);
	}

	return ret;
}

void cmdTrans(CAR_INFO * carInfo)
{
	int i = 0;

		if(cmdProcess.commandIn[i] == 1)
		{
			i ++;
			if(cmdProcess.commandIn[i] == 1)
			{
				i ++;
				carInfo->udpSpeedIn = cmdProcess.commandIn[i];
			}
			else
			{
			//其他电机指令类型待定义
			}
			i ++;
		}
		else
		{
			i ++;
			
			USART_SendByte(USART1,0x77);
		}
		if(cmdProcess.commandIn[i] == 1)
		{
			i ++;
			if(cmdProcess.commandIn[i] == 1)
			{
				i ++;
				memcpy(&(carInfo->udpDirectIn),&(cmdProcess.commandIn[i]),4);
			}
			else
			{
			//其他舵机指令类型待定义
			}
			i +=4;
		}
		else
		{
			i ++;
		}
		if(cmdProcess.commandIn[i] == 1)
		{
			i ++;
		}
		else
		{
			i ++;
		}
		if(cmdProcess.commandIn[i] == 1)
		{
			i ++;
		}
		else
		{
			i ++;
		}
	
	USART_Cmd(USART1, ENABLE);
	memset(&cmdProcess,0,sizeof(CMD_PROCESS));
}

