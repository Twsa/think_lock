#include "stm32f10x_it.h"
#include "usart1/usart1.h"
#include "key/key.h"
#include "Lock_Driver.h"
#include "Door_Control/Door_Control.h"

//extern  uint16_t Key_Count;
//extern  char  Key_Status;
extern  char count_reset;
extern unsigned char Opendoor_Staus;
extern uint8_t usart1It;
/**********************************************************
** ������: TIM1_IRQHandler
** ��������: ��ʱ��6�ĸ����жϷ������
** �������: ��
** �������: ��
***********************************************************/
void TIM2_IRQHandler(void)
{
	static int Opentime_count=0;
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	 //�����������¼�   
	{			
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);       //��������жϱ�־λ
		
/*--------------�����жϴ�����---------------*/ 
		if (usart1It == USART1IT)
		{
			count_reset++;	
		}	
		if(Opendoor_Staus==OPENED)
		{
				Opentime_count++;
			 if(Opentime_count>=5000)
			 {
//					 if (Menci_Key==1)
//					 {
						Opentime_count=0;
						Close_door();
//					 }
			 }
			}
//    if (Scan_Count>=50)	
//		{
//			Scan_Count=0;
//			KEY_Scan();
//			if (Key_Status==1)
//		  {
//			  Key_Count++;
//			  if (Key_Count>=100)     //ÿ30ms���һ�ΰ���
//			  {
//				  printf("������λ�ɹ�!\n");
//				  Key_Count=0;			
//			  }
//		  }
//			else
//				  Key_Count=0;
//		}	
/*----------------------------------------------*/		
	}
}
