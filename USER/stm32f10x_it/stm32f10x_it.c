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
** 函数名: TIM1_IRQHandler
** 功能描述: 定时器6的更新中断服务程序
** 输入参数: 无
** 输出参数: 无
***********************************************************/
void TIM2_IRQHandler(void)
{
	static int Opentime_count=0;
	
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)	 //如果发生溢出事件   
	{			
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);       //清除更新中断标志位
		
/*--------------按键中断处理部分---------------*/ 
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
//			  if (Key_Count>=100)     //每30ms检测一次按键
//			  {
//				  printf("按键复位成功!\n");
//				  Key_Count=0;			
//			  }
//		  }
//			else
//				  Key_Count=0;
//		}	
/*----------------------------------------------*/		
	}
}
