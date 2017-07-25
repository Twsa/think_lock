#include "Usart_string/Usart_string.h"

char count_reset=0;
extern uint8_t usart1It = 0;
void  Usart_str(char *RX_buffer)
{
  uint16_t i=0,status1=0,save=0;
	usart1It = USART1IT;
	while(status1!=1)
	{
	count_reset=0;
	while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)!=SET)
	{
		TIM_Cmd(TIM2, ENABLE);		
		if (count_reset>=30)
		{
			 count_reset=0;
				save=i;
				if (save==0)
				break;

				status1=1;
					break; 
		}															
	}
	if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)==SET)
	{	  
			if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE)!=SET) break;
			
			*(RX_buffer+i)= USART_ReceiveData(USART1);
		  i++;
	}
}
 TIM_Cmd(TIM2, DISABLE);		
 usart1It = 0;
//		 printf("%s",RX_buffer);
//		 printf("\n");
}

