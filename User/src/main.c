/*---------------------------------------------------------------------------------------------
    Author       : ������

    Created Date : 2015-01-12

    Descriptions : �ײ����������
      
    Version             Description             Date                Author
    0.1                 Created                 2015-01-12       ������

---------------------------------------------------------------------------------------------*/
#include "typedef.h"
#include "drive_api.h"


#define TEST 1

int sysFrequency;
int main(void)
{
	CAR_INFO  carInfo;
	RCC_ClocksTypeDef  clockInfo;
	
	deviceInit();
	#if TEST

	RCC_GetClocksFreq(&clockInfo);
	sysFrequency = clockInfo.SYSCLK_Frequency;

	carInfo.udpSpeedIn = -50;
	
	carInfo.udpDirectIn = 0x0200;
	carCal(&carInfo);
	
	carDirectOut(&carInfo);
	#endif
	while(1)
	{
		//carSpeedOut(&carInfo);
		
		//carDirectOut(&carInfo);
		//USART_SendByte(USART1,0x01);

	}

	
	

}
