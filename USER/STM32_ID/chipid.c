/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * 文件名  ：chipid.c
 * 描述    ：获取CPU的ID函数，每个芯片都有唯一的 96_bit unique ID         
 * 实验平台：野火STM32开发板
 * 硬件连接：-----------------
 *          |                 |
 *          |       无        |
 *          |                 |
 *           -----------------
 * 库版本  ：ST3.5.0
 * 作者    ：wildfire team 
 * 论坛    ：http://www.amobbs.com/forum-1008-1.html
 * 淘宝    ：http://firestm32.taobao.com
**********************************************************************************/
#include "STM32_ID/chipid.h"
#include "usart1/usart1.h"
#include "Usart_string/Usart_string.h"
	#include "AT24C128/24cxx.h" 
char ID_buffer[12]={0};
char STM32_ID[24]={0};
/*
 * 函数名：Get_ChipID
 * 描述  ：获取芯片ID
 * 输入  ：无
 * 输出  ：无
 */
void Get_ChipID(void)
{
	char i=0,j=3;
	uint32_t ChipUniqueID[3]={0};
	
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // 高字节
	ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
	ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7E8); // 低字节
	for (i=0;i<12;)
	{
		ID_buffer[i++]=ChipUniqueID[i/4]>>(j*8);
		j--;
		ID_buffer[i++]=ChipUniqueID[i/4]>>(j*8);
		j--;
		ID_buffer[i++]=ChipUniqueID[i/4]>>(j*8);
		j--;
		ID_buffer[i++]=ChipUniqueID[i/4]>>(j*8);
		j=3;
	}
}


void MISI(char *misi_data)
	
{   char Test_Buffer[]="123456";

		AT24C128_Save(0,Test_Buffer,sizeof(Test_Buffer));
}
