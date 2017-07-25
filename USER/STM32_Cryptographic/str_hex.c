#include "usart1/usart1.h"
#include "stdlib.h"
#include "STM32_Cryptographic/str_hex.h"

static void abc(uint8_t *buffer1,uint8_t *buffer,int i,int j)
{	 
	  uint8_t buf_2[2]={0};
	  int case_buf[2]={0};
		buf_2[0]=*(buffer+i);
		buf_2[1]=*(buffer+i+1);
		
		if (buf_2[0]>='0'&&buf_2[0]<='9')
      case_buf[0]=1;
		else if(buf_2[0]>='a'&&buf_2[0]<='z')
			case_buf[0]=2;
		else if(buf_2[0]>='A'&&buf_2[0]<='Z')
      case_buf[0]=3;
	
		if (buf_2[1]>='0'&&buf_2[1]<='9')
      case_buf[1]=1;
		else if(buf_2[1]>='A'&&buf_2[1]<='Z')
			case_buf[1]=2;
		else if(buf_2[1]>='a'&&buf_2[1]<='z')
      case_buf[1]=3;
		
		switch(case_buf[0])
		{
			case 1:switch(case_buf[1])
			{
				case 1:	buf_2[1]=buf_2[1]-'0';break;
				case 2: buf_2[1]=buf_2[1]-0x57;break;
				case 3: buf_2[1]=buf_2[1]-0x57;break;
				default:break;
			} *(buffer1+j)=(buf_2[0]-'0')*16+buf_2[1];break;
			case 2:switch(case_buf[1])
			{
        case 1:	buf_2[1]=buf_2[1]+0x50;break;
				case 2: buf_2[1]=buf_2[1]+0x49;break;
				case 3: buf_2[1]=buf_2[1]+0x29;break;
				default:break;
			} *(buffer1+j)=(buf_2[0]+0x21)*16+buf_2[1];break;
			case 3:switch(case_buf[1])
			{
				case 1:	buf_2[1]=buf_2[1]+0x30;break;
				case 2: buf_2[1]=buf_2[1]+0x29;break;
				case 3: buf_2[1]=buf_2[1]+0x09;break;
				default:break;
			} *(buffer1+j)=(buf_2[0]+0x23)*16+buf_2[1];break;
				default:break;
		}
		
//			printf("%x",*(buffer1+j));
}

void Str2HexV1(uint8_t *StrSource,uint8_t *StrDestination,uint32_t len)
{
//	int N1=N/2;
//  uint8_t *buffer1 = (uint8_t*)malloc(N1);

 int i=0,j=0;
	while(i<len)
	{
	  abc(StrDestination,StrSource,i,j);
				i+=2;
		    j++;
	}
}
/*************************************************************************************
*  Function Name                    :    Hex2Str

*  Create Date                      :    2017/3/12
*  Author/Corporation               :          Liu Chuanjia
*  Description                      :    16进制数组转化为字符串数组


*  Param                                :    uint8_t *String ,uint8_t *Hex,uint32_t HexLen

*  Return Code                          :    None

*  Global Variable                      :    None
*  File Static Variable                 :    None
*  Function Static Variable                  :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016.10.18  Liu Chuanjia     None           None

**************************************************************************************/
uint8_t Hex2Str(uint8_t *String ,const uint8_t *Hex,uint32_t HexLen)
{
	int i=0,j=0,k=0;
	int caseHex1=0;
	int caseHex2=0;
  
	if(HexLen<=0)
	{
		Error_Handler();
		return 0;
	}
	
	for (;i<HexLen;i++)
	{
		while(j<2)
		{
		    if(((Hex[i]>>4&0x0f)>=0x00)&&((Hex[i]>>4&0x0f)<=0x09))
                caseHex1=1;
            else if(((Hex[i]>>4&0x0f)>=0x0a)&&((Hex[i]>>4&0x0f)<=0x0f))
                caseHex1=2;
            else if(((Hex[i]>>4&0x0A)>=0x0a)&&((Hex[i]>>4&0x0f)<=0x0F))
                caseHex1=3;

            if(((Hex[i]&0x0f)>=0x00)&&(Hex[i]&0x0f)<=0x09)
                caseHex2=1;
            else if(((Hex[i]&0x0f)>=0x0a)&&(Hex[i]&0x0f)<=0x0f)
                caseHex2=2;
            else if(((Hex[i]&0x0f)>=0x0A)&&(Hex[i]&0x0f)<=0x0F)
                caseHex2=3;
                switch(caseHex1)
                {
                  case 1:* (String+k++)=(Hex[i]>>4&0x0f)+0x30;j++;
                  switch(caseHex2)
                  {
                      case 1:* (String+k++)=(Hex[i]&0x0f)+0x30;j++;break;
                      case 2:* (String+k++)=(Hex[i]&0x0f)+0x57;j++;break;
                      case 3:* (String+k++)=(Hex[i]&0x0f)+0x57;j++;break;
                      default:break;
                  }break;

                  case 2:* (String+k++)=(Hex[i]>>4&0x0f)+0x57;j++;
                  switch(caseHex2)
                  {
                      case 1:* (String+k++)=(Hex[i]&0x0f)+0x30;j++;break;
                      case 2:* (String+k++)=(Hex[i]&0x0f)+0x57;j++;break;
                      case 3:* (String+k++)=(Hex[i]&0x0f)+0x57;j++;break;
                      default:break;
                  }break;
                  case 3:* (String+k++)=(Hex[i]>>4&0x0f)+0x57;j++;
                  switch(caseHex2)
                  {
                      case 1:* (String+k++)=(Hex[i]&0x0f)+0x30;j++;break;
                      case 2:* (String+k++)=(Hex[i]&0x0f)+0x57;j++;break;
                      case 3:* (String+k++)=(Hex[i]&0x0f)+0x57;j++;break;
                      default:break;
                  }break;
                  default:break;
                }

		}
		j=0;
	}
	return 0;
}
/*************************************************************************************
*  Function Name                    :    Str2Hex

*  Create Date                      :    2017/3/12
*  Author/Corporation          :    Liu Chuanjia
*  Description                       :    明文字符串数组转化为16进制数组，利用哈希表进行转化


*  Param                                   :    None

*  Return Code                          :    None

*  Global Variable                      :    None
*  File Static Variable                 :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016.10.18  Liu Chuanjia     None           None

**************************************************************************************/
//to {0x31,0x32};
//char *String ,char *Hex,uint32_t StringLen
void Str2HexPain(uint8_t *String ,uint8_t *Hex,uint32_t StringLen)
{
	uint32_t hexLen=StringLen/2;
	int doubleIndex=0;
	uint32_t strIndex=0;          //Str数据流长度索引
	uint32_t hexIndex=0;        //Hex数据流长度索引
	uint8_t strBuf[3]={0};

	uint8_t  finalIndex=0;

	uint8_t HexLib[37]={0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,
		0x61,0x62,0x63,0x64,0x65,0x66,0x67,0x68,0x69,
		0x6a,0x6b,0x6c,0x6d,0x6e,0x6f,
		0x70,0x71,0x72,0x73,0x74,0x75,0x76,0x77,0x78,0x79
		,0x7a};


	for (hexIndex=0;hexIndex<hexLen;hexIndex++)   //按照16进制的数据的个数进行第循环；
	{
		for(doubleIndex=0;doubleIndex<2;doubleIndex++)
		{
			strBuf[doubleIndex]=String[strIndex++];
		}
		finalIndex=atoi((char *)strBuf);
		if(finalIndex>=30&&finalIndex<=39)
			finalIndex%=30;
		else if(finalIndex>=61&&finalIndex<=69)
			finalIndex=(finalIndex%60)+9;
		else if(finalIndex==6)
		{
			finalIndex=(int)strBuf[1]-78;
		}
		else if(finalIndex>=70&&finalIndex<=79)
		{
			finalIndex=(finalIndex%50)+5;
		}
		else if(finalIndex==7)
		{
			finalIndex=35;
		}

		Hex[hexIndex]=HexLib[finalIndex];
	}
}
/*************************************************************************************
*  Function Name                    :    Str2Hex

*  Create Date                      :    2017/3/13
*  Author/Corporation          :    Liu Chuanjia
*  Description                       :    字符串数组转化为16进制数组，利用哈希表进行转化


*  Param                                   :    None

*  Return Code                          :    None

*  Global Variable                      :    None
*  File Static Variable                 :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016.10.18  Liu Chuanjia     None           None

**************************************************************************************/
uint8_t Str2Hex(const uint8_t *String ,uint8_t *Hex,uint32_t StringLen)
{
#define caseFirstNum   0x01
#define caseFirstLetter 0x02
#define caseLastNum   0x03
#define caseLastLetter 0x04

  if(StringLen<2)
	{
		Error_Handler();
		return 0;
	}
	uint32_t hexLen=StringLen/2;
	int doubleIndex=0;
	uint32_t strIndex=0;          //Str数据流长度索引
	uint32_t hexIndex=0;        //Hex数据流长度索引

	uint8_t strBuf[3]={0};
	uint8_t caseIndex0=0;
	uint8_t caseIndex1=0;

	//uint8_t HexLib[16]={
	//	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,
	//	0x0a,0x0b,0x0c,0x0d,0x0e,0x0f
	//};

	for (hexIndex=0;hexIndex<hexLen;hexIndex++)   //按照16进制的数据的个数进行第循环；
	{
		for(doubleIndex=0;doubleIndex<2;doubleIndex++)
		{
			strBuf[doubleIndex]=String[strIndex++];
		}
		if(strBuf[0]>='0'&&strBuf[0]<='9')  caseIndex0=caseFirstNum;
		else if(strBuf[0]>='a'&&strBuf[0]<='f')  caseIndex0=caseFirstLetter;

		if(strBuf[1]>='0'&&strBuf[1]<='9')  caseIndex1=caseLastNum;
		else if(strBuf[1]>='a'&&strBuf[1]<='f')  caseIndex1=caseLastLetter;

		switch(caseIndex0)
		{
		case caseFirstNum: strBuf[0]=strBuf[0]&0x0f;
			switch(caseIndex1)
			{
			case caseLastNum:strBuf[1]=strBuf[1]&0x0f;break;
			case caseLastLetter:strBuf[1]=(strBuf[1]&0x0f)+0x09;break;
			default:break;
			} break;
		case caseFirstLetter:strBuf[0]=(strBuf[0]&0x0f)+0x09;
			switch(caseIndex1)
			{
			case caseLastNum:strBuf[1]=strBuf[1]&0x0f;break;
			case caseLastLetter:strBuf[1]=(strBuf[1]&0x0f)+0x09;break;
			default:break;
			} break;
		default:break;
		}
		Hex[hexIndex]=(strBuf[0]<<4)+strBuf[1];
	}
	return 0;
}

