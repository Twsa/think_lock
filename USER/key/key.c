#include "stm32f10x.h"
#include "key/key.h"
#include "delay/delay.h"
#include "usart1/usart1.h"



uint16_t Key_Count=0;		//°´¼ü°´ÏÂÖ®ºó¼ÆÊ±
char  Key_Status=0;  //°´¼üÄ¬ÈÏÎªµ¯Æğ×´Ì¬ª
//KEY   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)//¶ÁÈ¡°´¼ü0
//°´¼ü³õÊ¼»¯º¯Êı
void KEY_Init(void)
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//Ê¹ÄÜPORTAÊ±ÖÓ
		
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //ÉèÖÃ³ÉÉÏÀ­ÊäÈë
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//³õÊ¼»¯GPIOA0
	
}

void KEY_Scan(void)
{		
	 if(KEY==ON)                         //°´¼ü±»°´ÏÂ£¬Ôò½øÈë°´¼ü¼ÆÊ±
	 {
		 Key_Status=1;		 
	 }
   if (KEY==OFF)                       //Èç¹û°´¼üÃ»ÓĞ°´ÏÂ»òÕß°´¼üµ¯ÆğÁË£¬Ôò½øÈë³õÊ¼»¯×´Ì¬           
	 {
		 Key_Count=0;
		 Key_Status=0;
	 }		 
}
