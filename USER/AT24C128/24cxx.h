#ifndef __24CXX_H
#define __24CXX_H
#include "AT24C128/myiic.h"

#define AT24C01		127
#define AT24C02		255       //һҳ��128���ֽ�
#define AT24C04		511
#define AT24C08		1023
#define AT24C16		2047
#define AT24C32		4095
#define AT24C64	    8191
#define AT24C128	16383
#define AT24C256	32767  
//���õ�ģ����24c128�����Զ���EE_TYPEΪAT24C128
#define EE_TYPE AT24C256
					  
char AT24CXX_ReadOneByte(u16 ReadAddr);							//ָ����ַ��ȡһ���ֽ�
void AT24CXX_WriteOneByte(u16 WriteAddr,char DataToWrite);		//ָ����ַд��һ���ֽ�
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,char Len);//ָ����ַ��ʼд��ָ�����ȵ�����
u32 AT24CXX_ReadLenByte(u16 ReadAddr,char Len);					//ָ����ַ��ʼ��ȡָ����������
void AT24CXX_Write(u16 WriteAddr,char *pBuffer,u16 NumToWrite);	//��ָ����ַ��ʼд��ָ�����ȵ�����
void AT24CXX_Read(u16 ReadAddr,char *pBuffer,u16 NumToRead);   	//��ָ����ַ��ʼ����ָ�����ȵ�����
void AT24CXX_Write_WORD(uint16_t WriteAddr,uint32_t *pBuffer,uint16_t NumToWrite);

char AT24CXX_Check(void);  //�������

void AT24CXX_Init(void); //��ʼ��IIC
void AT24C128_ID(void);
void AT24C128_Save(uint16_t WriteAddr,char *Data,uint16_t SIZE);
void AT24C128_Read(uint16_t WriteAddr,uint16_t SIZE);

#endif
















