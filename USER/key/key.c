#include "stm32f10x.h"
#include "key/key.h"
#include "delay/delay.h"
#include "usart1/usart1.h"



uint16_t Key_Count=0;		//��������֮���ʱ
char  Key_Status=0;  //����Ĭ��Ϊ����״̬�
//KEY   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//��ȡ����0
//������ʼ������
void KEY_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTAʱ��
		
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA0
	
}

void KEY_Scan(void)
{		
	 if(KEY==ON)                         //���������£�����밴����ʱ
	 {
		 Key_Status=1;		 
	 }
   if (KEY==OFF)                       //�������û�а��»��߰��������ˣ�������ʼ��״̬           
	 {
		 Key_Count=0;
		 Key_Status=0;
	 }		 
}
