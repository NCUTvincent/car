/*---------------------------------------------------------------------------------------------
    Author       : ������

    Created Date : 2015-01-12

    Descriptions : �ײ����api
      
    Version             Description             Date                Author
    0.1                 Created                 2015-01-12       ������

---------------------------------------------------------------------------------------------*/
#include "typedef.h"

int deviceInit(SYS_INFO * sysInfo)
{
	int ret =0;
	 USART_InitTypeDef * usartInfo 	= &(sysInfo->usartInfo);	
	 EXTI_InitTypeDef *	 extiInfo 	= &(sysInfo->extiInfo);
	 GPIO_InitTypeDef * gpioInfo 	= &(sysInfo->gpioInfo);
	 NVIC_InitTypeDef * nvicInfo 	= &(sysInfo->nvicInfo);
	 RCC_ClocksTypeDef * clockInfo 	= &(sysInfo->clockInfo);

	USART_DeInit(USART1);
	USART_DeInit(USART2);
	USART_StructInit(usartInfo);
	usartInfo->USART_BaudRate = 115200;
	USART_Init(USART1, usartInfo);
	USART_Init(USART2, usartInfo);

	return ret;
}

