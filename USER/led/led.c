#include "led\led.h"
#include "delay\delay.h"

#define ON    1
#define OFF   0

extern  int count_reset;
extern  int all_time;
extern  int Led_count;
u8 Led_Status=OFF;
int time_forbuzz=0;
int BUZZ_count=0;
void Led_ON(void)
{
	 Led_Status=ON;
//	 TIM_Cmd(TIM2, ENABLE);	
//	 TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
}
void Led_Control(void)	      
{
   
    if (Led_count>=300)
	{
	   GPIO_WriteBit(GPIOB, GPIO_Pin_6, 
	  (BitAction)((1-GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_6))));	//出现闪烁效果
	  Led_count=0;
	}
	 if (time_forbuzz>=BUZZ_count)
	 {
	   GPIO_WriteBit(GPIOB, GPIO_Pin_9, 
	  (BitAction)((1-GPIO_ReadOutputDataBit(GPIOB, GPIO_Pin_9))));	//出现闪烁效果
	  time_forbuzz=0;
	  BUZZ_count+=10;
	}
//     GPIO_ResetBits(GPIOB, GPIO_Pin_8);
//     GPIO_SetBits(GPIOB, GPIO_Pin_9);			 //BUZZ蜂鸣器
	  		
	if (all_time>=3000)
	{
	 BUZZ_count=0;	
	 all_time=0;
	 GPIO_ResetBits(GPIOB, GPIO_Pin_9 | GPIO_Pin_6 | GPIO_Pin_5);	         //关闭蜂鸣器
//	 TIM_Cmd(TIM2, DISABLE);
//     TIM_ITConfig(TIM2,TIM_IT_Update,DISABLE);
	 Led_Status=OFF;
	}
}
void Led_Dooropen(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_9 | GPIO_Pin_6);
	delay_ms(1500);
	GPIO_ResetBits(GPIOB, GPIO_Pin_9 | GPIO_Pin_6 | GPIO_Pin_5);	         //关闭蜂鸣器
}
void LED_GPIO_config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  GPIO_ResetBits(GPIOB, GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_6 | GPIO_Pin_9);
}	  
