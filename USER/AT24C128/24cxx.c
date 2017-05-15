#include "AT24C128/24cxx.h" 
#include "delay/delay.h"
#include "AT24C128/myiic.h"

//��ʼ��IIC�ӿ�
void AT24CXX_Init(void)
{
	IIC_Init();
}
//��AT24CXXָ����ַ����һ������
//ReadAddr:��ʼ�����ĵ�ַ  
//����ֵ  :����������
char AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	char temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//���͸ߵ�ַ
//		IIC_Wait_Ack();		 
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //����������ַ0XA0,д���� 	 

	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //�������ģʽ			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	    
	return temp;
}
//��AT24CXXָ����ַд��һ������
//WriteAddr  :д�����ݵ�Ŀ�ĵ�ַ    
//DataToWrite:Ҫд�������
void AT24CXX_WriteOneByte(u16 WriteAddr,char DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //����д����
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//���͸ߵ�ַ
 	}else
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //����������ַ0XA0,д���� 
	}	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //���͵͵�ַ
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //�����ֽ�							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	delay_ms(10);	 
}
//��AT24CXX�����ָ����ַ��ʼд�볤��ΪLen������
//�ú�������д��16bit����32bit������.
//WriteAddr  :��ʼд��ĵ�ַ  
//DataToWrite:���������׵�ַ
//Len        :Ҫд�����ݵĳ���2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,char Len)
{  	
	char t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//��AT24CXX�����ָ����ַ��ʼ��������ΪLen������
//�ú������ڶ���16bit����32bit������.
//ReadAddr   :��ʼ�����ĵ�ַ 
//����ֵ     :����
//Len        :Ҫ�������ݵĳ���2,4
u32 AT24CXX_ReadLenByte(u16 ReadAddr,char Len)
{  	
	char t;
	u32 temp=0;
	for(t=0;t<Len;t++)
	{
		temp<<=8;
		temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 	 				   
	}
	return temp;												    
}
//���AT24CXX�Ƿ�����
//��������24XX�����һ����ַ(255)���洢��־��.
//���������24Cϵ��,�����ַҪ�޸�
//����1:���ʧ��
//����0:���ɹ�
char AT24CXX_Check(void)
{
	char temp;
	temp=AT24CXX_ReadOneByte(255);//����ÿ�ο�����дAT24CXX			   
	if(temp==0X55)return 0;		   
	else//�ų���һ�γ�ʼ�������
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//��AT24CXX�����ָ����ַ��ʼ����ָ������������
//ReadAddr :��ʼ�����ĵ�ַ ��24c02Ϊ0~255
//pBuffer  :���������׵�ַ
//NumToRead:Ҫ�������ݵĸ���
void AT24CXX_Read(u16 ReadAddr,char *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���8
void AT24CXX_Write(u16 WriteAddr,char *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
//��AT24CXX�����ָ����ַ��ʼд��ָ������������
//WriteAddr :��ʼд��ĵ�ַ ��24c02Ϊ0~255
//pBuffer   :���������׵�ַ
//NumToWrite:Ҫд�����ݵĸ���32
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
void AT24CXX_Write_WORD(uint16_t WriteAddr,uint32_t *pBuffer,uint16_t NumToWrite)
{
	uint16_t index=0;
	char Num_write=NumToWrite*sizeof(uint32_t);
//	char buffer[Num_write];
	char *buffer = (char*)malloc(Num_write);
	for(index=0;index<Num_write;index++)
	{
		buffer[index]=(*(pBuffer+index/sizeof(uint32_t))>>Num_write);
	}
	while(Num_write--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*buffer);
		WriteAddr++;
		buffer++;
	}
}
//0~127,128~255
 
extern char ID_buffer[12];
extern char STM32_ID[24];

void AT24C128_ID(void)
{
	char datatemp[12];
	signed char buffer;
  int i=0,j=0;
//	while(AT24CXX_Check())//��ⲻ��24c02
//	{
//		delay_ms(500);
//    printf("please check!\n");
//		delay_ms(500);
//	}
//	IIC_Send_Byte(0X01);
//	
//	printf("check successfully!\n");
	AT24CXX_Write(128,(char *)(ID_buffer),12);
	AT24CXX_Read(128,datatemp,12);
//	printf("data_output:%s",datatemp); 
//	printf("my stm32ID:");
	for (i=0;i<12;i++)
	{
    buffer=((signed char)datatemp[i]>>4&0x0f);
//				printf("%x",buffer);
		if (buffer>=0X00&&buffer<=0X09)
		 STM32_ID[j++]=buffer+'0';
		else if (buffer>=0x0a&&buffer<=0X0f)
		 STM32_ID[j++]=buffer+'W';	
		
		buffer=((signed char)datatemp[i]&0x0f);
//		printf("%x",buffer);
		if (buffer>=0X00&&buffer<=0X09)
		 STM32_ID[j++]=buffer+'0';
		else if (buffer>=0x0a&&buffer<=0X0f)
		 STM32_ID[j++]=buffer+'W';	
	}
//	printf("%s\n",STM32_ID);//��ID���͸�App
}
void AT24C128_Save(uint16_t WriteAddr,char *Data,uint16_t SIZE)
{
	char *datatemp=(char*)malloc(SIZE);
	
	AT24CXX_Write(WriteAddr,(char*)(Data),SIZE);
//	AT24CXX_Read(WriteAddr,datatemp,SIZE);

//	printf("%s",datatemp);
	free(datatemp);
}
void AT24C128_Read(uint16_t WriteAddr,uint16_t SIZE)
{
	char *datatemp=(char*)malloc(SIZE);	
	AT24CXX_Read(WriteAddr,datatemp,SIZE);
	printf("%s\n",datatemp);
	free(datatemp);
}

