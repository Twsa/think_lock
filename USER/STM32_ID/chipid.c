/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��chipid.c
 * ����    ����ȡCPU��ID������ÿ��оƬ����Ψһ�� 96_bit unique ID         
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�-----------------
 *          |                 |
 *          |       ��        |
 *          |                 |
 *           -----------------
 * ��汾  ��ST3.5.0
 * ����    ��wildfire team 
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "STM32_ID/chipid.h"
#include "usart1/usart1.h"
#include "Usart_string/Usart_string.h"
	#include "AT24C128/24cxx.h" 
char ID_buffer[12]={0};
char STM32_ID[24]={0};
/*
 * ��������Get_ChipID
 * ����  ����ȡоƬID
 * ����  ����
 * ���  ����
 */
void Get_ChipID(void)
{
	char i=0,j=3;
	uint32_t ChipUniqueID[3]={0};
	
	ChipUniqueID[0] = *(__IO u32 *)(0X1FFFF7F0); // ���ֽ�
	ChipUniqueID[1] = *(__IO u32 *)(0X1FFFF7EC); // 
	ChipUniqueID[2] = *(__IO u32 *)(0X1FFFF7E8); // ���ֽ�
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
