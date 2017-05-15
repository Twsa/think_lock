#ifndef __24CXX_H
#define __24CXX_H
#include "AT24C128/myiic.h"

#define AT24C01		127
#define AT24C02		255       //一页是128个字节
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
//我用的模块是24c128，所以定义EE_TYPE为AT24C128
#define EE_TYPE AT24C256
					  
char AT24CXX_ReadOneByte(u16 ReadAddr);							//指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr,char DataToWrite);		//指定地址写入一个字节
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,char Len);//指定地址开始写入指定长度的数据
u32 AT24CXX_ReadLenByte(u16 ReadAddr,char Len);					//指定地址开始读取指定长度数据
void AT24CXX_Write(u16 WriteAddr,char *pBuffer,u16 NumToWrite);	//从指定地址开始写入指定长度的数据
void AT24CXX_Read(u16 ReadAddr,char *pBuffer,u16 NumToRead);   	//从指定地址开始读出指定长度的数据
void AT24CXX_Write_WORD(uint16_t WriteAddr,uint32_t *pBuffer,uint16_t NumToWrite);

char AT24CXX_Check(void);  //检查器件

void AT24CXX_Init(void); //初始化IIC
void AT24C128_ID(void);
void AT24C128_Save(uint16_t WriteAddr,char *Data,uint16_t SIZE);
void AT24C128_Read(uint16_t WriteAddr,uint16_t SIZE);

#endif
















