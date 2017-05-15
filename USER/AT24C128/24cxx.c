#include "AT24C128/24cxx.h" 
#include "delay/delay.h"
#include "AT24C128/myiic.h"

//初始化IIC接口
void AT24CXX_Init(void)
{
	IIC_Init();
}
//在AT24CXX指定地址读出一个数据
//ReadAddr:开始读数的地址  
//返回值  :读到的数据
char AT24CXX_ReadOneByte(u16 ReadAddr)
{				  
	char temp=0;		  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	   //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(ReadAddr>>8);//发送高地址
//		IIC_Wait_Ack();		 
	}else IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));   //发送器件地址0XA0,写数据 	 

	IIC_Wait_Ack(); 
    IIC_Send_Byte(ReadAddr%256);   //发送低地址
	IIC_Wait_Ack();	    
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);           //进入接收模式			   
	IIC_Wait_Ack();	 
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//产生一个停止条件	    
	return temp;
}
//在AT24CXX指定地址写入一个数据
//WriteAddr  :写入数据的目的地址    
//DataToWrite:要写入的数据
void AT24CXX_WriteOneByte(u16 WriteAddr,char DataToWrite)
{				   	  	    																 
    IIC_Start();  
	if(EE_TYPE>AT24C16)
	{
		IIC_Send_Byte(0XA0);	    //发送写命令
		IIC_Wait_Ack();
		IIC_Send_Byte(WriteAddr>>8);//发送高地址
 	}else
	{
		IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));   //发送器件地址0XA0,写数据 
	}	 
	IIC_Wait_Ack();	   
    IIC_Send_Byte(WriteAddr%256);   //发送低地址
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(DataToWrite);     //发送字节							   
	IIC_Wait_Ack();  		    	   
    IIC_Stop();//产生一个停止条件 
	delay_ms(10);	 
}
//在AT24CXX里面的指定地址开始写入长度为Len的数据
//该函数用于写入16bit或者32bit的数据.
//WriteAddr  :开始写入的地址  
//DataToWrite:数据数组首地址
//Len        :要写入数据的长度2,4
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,char Len)
{  	
	char t;
	for(t=0;t<Len;t++)
	{
		AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
	}												    
}

//在AT24CXX里面的指定地址开始读出长度为Len的数据
//该函数用于读出16bit或者32bit的数据.
//ReadAddr   :开始读出的地址 
//返回值     :数据
//Len        :要读出数据的长度2,4
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
//检查AT24CXX是否正常
//这里用了24XX的最后一个地址(255)来存储标志字.
//如果用其他24C系列,这个地址要修改
//返回1:检测失败
//返回0:检测成功
char AT24CXX_Check(void)
{
	char temp;
	temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX			   
	if(temp==0X55)return 0;		   
	else//排除第一次初始化的情况
	{
		AT24CXX_WriteOneByte(255,0X55);
	    temp=AT24CXX_ReadOneByte(255);	  
		if(temp==0X55)return 0;
	}
	return 1;											  
}

//在AT24CXX里面的指定地址开始读出指定个数的数据
//ReadAddr :开始读出的地址 对24c02为0~255
//pBuffer  :数据数组首地址
//NumToRead:要读出数据的个数
void AT24CXX_Read(u16 ReadAddr,char *pBuffer,u16 NumToRead)
{
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数8
void AT24CXX_Write(u16 WriteAddr,char *pBuffer,u16 NumToWrite)
{
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}
//在AT24CXX里面的指定地址开始写入指定个数的数据
//WriteAddr :开始写入的地址 对24c02为0~255
//pBuffer   :数据数组首地址
//NumToWrite:要写入数据的个数32
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
//	while(AT24CXX_Check())//检测不到24c02
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
//	printf("%s\n",STM32_ID);//将ID发送给App
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

