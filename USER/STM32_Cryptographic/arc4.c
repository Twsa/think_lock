#include "STM32_Cryptographic/rsa_think.h"


//typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;

/*************************************************************************************
*  Function Name                    :    Arc4EncryptNeedStrCiphertext

*  Create Date                      :    2017/03/10
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    Arc4加密一个字符串的测试函数,并将密文从hex转化成string类型


*  Param                            :    

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    uint8_t  *arc4Key, 
                                         const uint32_t keyLength, 
										                     uint8_t *PlainText,
										                     const uint32_t PainTextLen,
								                         uint8_t *MessageOutput	
																				 其中定义的MessageOutput的内存区域必须是PainText的两倍
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/12    Liu Chuanjia     None           None

**************************************************************************************/
uint8_t Arc4EncryptNeedStrCiphertext( uint8_t  *arc4Key, 
                    const uint32_t keyLength, 
										uint8_t *PlainText,
										const uint32_t PainTextLen,
								    uint8_t *MessageOutput							
											)
{
	int32_t status = ARC4_SUCCESS;
	uint32_t arc4CryptoLen=PainTextLen ;                  //加密后的16进制的密文应与明文的空间大小一样
	uint8_t *arc4CryptoBuf=(uint8_t *)malloc(arc4CryptoLen);

	uint32_t msgRealLen=0;
	
	while(*(PlainText+msgRealLen++)!='\0');	msgRealLen-=1;   //采集传进来的明文的真实长度

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	
//	printf("\r\n%d\r\n",PainTextLen);
	status = STM32_ARC4_Encrypt(PlainText,
										 msgRealLen,
										 arc4Key,
										 keyLength,
	                   arc4CryptoBuf,
										 &arc4CryptoLen
										 );
//	printf("\r\n%d\r\n",arc4CryptoLen);arc4CryptoLen=StrLen=3

	if (status == ARC4_SUCCESS)
  {
    /* add application traitment in case of ARC4 SUCCESS */
//		 strCopy(arc4CryptoBuf,MessageOutput,arc4CryptoLen);
		Hex2Str(MessageOutput,arc4CryptoBuf,arc4CryptoLen); //将密文从Hex形式转化为Str形式  默认为Source HexLen

	      *(MessageOutput+2*arc4CryptoLen)='\0';                     //在末尾追加
	}
	free(arc4CryptoBuf);
	return 0;
}
/*************************************************************************************
*  Function Name                    :    Arc4DecryptFromHexPainText

*  Create Date                      :    2017/03/13
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    Arc4解密一个字符串的测试函数,
                                         需要将传进来的字符数组转化为可以解析的16进制数组，空间缩短一倍


*  Param                            :    

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    uint8_t  *arc4Key, 
                                         const uint32_t keyLength, 
										                     uint8_t *PlainText,
										                     const uint32_t PainTextLen,
								                         uint8_t *MessageOutput	
																				 其中定义的MessageOutput的内存区域必须是CipherText的一半
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/12    Liu Chuanjia     None           None

**************************************************************************************/
uint8_t Arc4DecryptFromHexPainText( uint8_t  *arc4Key, 
                    const uint32_t keyLength, 
										uint8_t *CipherText,
										const uint32_t CipherTextLen,          //String的密文的长度
								    uint8_t *MessageOutput							
											)
{
	int32_t status = ARC4_SUCCESS;
	uint32_t arc4CipherTextLen=0;
	uint8_t *arc4CipherTextHex=NULL;
	uint32_t CipherRealLen=0;
//	printf("\r\n%s\r\n",CipherText);
	while(*(CipherText+CipherRealLen++)!='\0');	CipherRealLen-=1;   //采集传进来的密文的真实长度
	arc4CipherTextLen=CipherRealLen/2;
	
	arc4CipherTextHex=(uint8_t *)malloc(arc4CipherTextLen);         //为16进制密文分配真实空间
	
	/*****需要将传进来的字符数组转化为可以解析的16进制数组*****/
	Str2Hex(CipherText,arc4CipherTextHex,CipherRealLen);    
   
//  for (int i=0;i<CipherRealLen;i++)	
//	{
//		printf("%x",arc4CipherTextHex[i]);
//	}
	/*******************ARC4解密部分************************/
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	status = STM32_ARC4_Decrypt(arc4CipherTextHex,
										 arc4CipherTextLen,
										 arc4Key,
										 keyLength,
	                   MessageOutput,
										 &arc4CipherTextLen    //解密后的字符串明文应与加密后的16进制明文大小一样
										 );
//	printf("arc4CipherTextLen:%d\n",arc4CipherTextLen);
	
	if (status == ARC4_SUCCESS)
  {
    /* add application traitment in case of ARC4 SUCCESS */
    *(MessageOutput+arc4CipherTextLen)='\0';                     //在末尾追加'\0'
	}
	free(arc4CipherTextHex);
	return 0;
}

/*************************************************************************************
*  Function Name                    :    Arc4Encrypt

*  Create Date                      :    2017/03/10
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    Arc4加密一个字符串的测试函数


*  Param                            :    

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/10    Liu Chuanjia     None           None

**************************************************************************************/
uint8_t Arc4Encrypt( uint8_t  *arc4Key, 
                    const uint32_t keyLength, 
										uint8_t *PlainText,
										const uint32_t PainTextLen,
								    uint8_t *MessageOutput	
											)
{
//  int i=0;
	int32_t status = ARC4_SUCCESS;
	uint32_t arc4CryptoLen=PainTextLen ;
	uint8_t *arc4CryptoBuf=(uint8_t *)malloc(arc4CryptoLen);
//	uint8_t *arc4Crypto=(uint8_t *)malloc(2*arc4CryptoLen);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	status = STM32_ARC4_Encrypt(PlainText,
										 PainTextLen,
										 arc4Key,
										 keyLength,
	                   arc4CryptoBuf,
										 &arc4CryptoLen
										 );
	if (status == ARC4_SUCCESS)
  {
    /* add application traitment in case of ARC4 SUCCESS */
		strCopy(arc4CryptoBuf,MessageOutput,arc4CryptoLen);
	}
//	Hex2Str(arc4Crypto ,arc4CryptoBuf,arc4CryptoLen);
//	for(;i<arc4CryptoLen;i++)
//	{
//		printf("%x",arc4CryptoBuf[i]);
//	}
//	printf("\r\n");
//	printf("\r\narc4Crypto:%s\r\n",arc4Crypto);
	free(arc4CryptoBuf);
	return 0;
}
/*************************************************************************************
*  Function Name                    :    Arc4Dencrypt

*  Create Date                      :    2017/03/10
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    Arc4算法解密一串字符串


*  Param                            :    

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/10    Liu Chuanjia     None           None

**************************************************************************************/
uint8_t Arc4Decrypt( uint8_t  *arc4Key, 
                    const uint32_t keyLength, 
										uint8_t *CipherText,
										const uint32_t CipherTextLen,
										uint8_t *MessageOutput	
											)
{
//  int i=0;
	int32_t status = ARC4_SUCCESS;
	uint32_t arc4DecryptLen=CipherTextLen ;
	uint8_t *arc4DecryptBuf=(uint8_t *)malloc(arc4DecryptLen);
//	uint8_t *arc4Decrypt=(uint8_t *)malloc(2*arc4DecryptLen);
	
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	status = STM32_ARC4_Decrypt(CipherText,
										 CipherTextLen,
										 arc4Key,
										 keyLength,
	                   arc4DecryptBuf,
										 &arc4DecryptLen
										 );
	if (status == ARC4_SUCCESS)
  {
    /* add application traitment in case of ARC4 SUCCESS */
		strCopy(arc4DecryptBuf,MessageOutput,arc4DecryptLen);
	}
//	hexStr(arc4Decrypt ,arc4DecryptBuf,arc4CryptoLen);
//	printf("\r\arc4DecryptBuf:%s\r\n",arc4DecryptBuf);
//		for(;i<arc4DecryptLen;i++)
//	{
//		printf("%c",arc4DecryptBuf[i]);
//	}
//	printf("\r\n");
	free(arc4DecryptBuf);
	return 0;
}
/*************************************************************************************
*  Function Name                    :    RC4_Key

*  Create Date                      :    2017/1/05
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ARC4密钥发生器


*  Param                            :    None

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            17.1.05     Liu Chuanjia     None           None

**************************************************************************************/
uint8_t RC4_Key(void)  
{
	cJSON *flag=NULL,*root=NULL;
	char *out=NULL;
	char * rc4Key ="12345678";//临时创建的RC4的密钥

	root=cJSON_CreateObject();            //创建对象结构体
	cJSON_AddItemToObject(root, "data", flag=cJSON_CreateObject());	
	cJSON_AddStringToObject(flag,"rc4Key",rc4Key);
	cJSON_AddStringToObject(root,"flag","26");//添加用户成功
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);
	free(out);	
	
	return 0;
}

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  rc4Encrypt.
  * @param  rc4Key plainMessage
  * @retval EncryptMessage
  */
//uint8_t const *rc4Encrypt(uint8_t  *rc4Key,uint8_t *plainMessage,uint32_t len)
//{
//  int32_t status = ARC4_SUCCESS;
//	uint32_t InputLength = sizeof(InputMessage);
//	/* Size of the output data */
//	uint8_t OutputMessageBuf[2*ARC4_LENGTH]={0};

//	uint32_t OutputMessageLength = 0;
//  uint8_t *encryptMessage=OutputMessageBuf;
//	
//  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
//	
//  /* Encrypt DATA with ARC4 */
//  status = STM32_ARC4_Encrypt( (uint8_t *) InputMessage, InputLength, rc4Key,len, OutputMessage,
//                               &OutputMessageLength);
//	
//  if (status == ARC4_SUCCESS)
//  {
////	    hexStr(OutputMessageBuf ,OutputMessage,ARC4_LENGTH);
//		 return encryptMessage;
//	}
//	else return 0;
//}
/**
  * @brief  rc4Decrypt.
  * @param  rc4Key EncryptMessage
  * @retval De_OutputMessage
  */
//uint8_t const *rc4Decrypt(uint8_t *rc4Key,uint8_t *encryptMessage,uint32_t len)
//{
//	 int32_t status = ARC4_SUCCESS;
//   uint32_t OutputMessageLength = 0;
//	 uint8_t De_OutputMessage[ARC4_LENGTH]={0};
//  /* Buffer to store the output data */

//	 uint8_t *decryptMessage=De_OutputMessage;

//	 /* Data has been encrypted, now decrypt it.
//   * Decrypt DATA with ARC4 */
//  status = STM32_ARC4_Decrypt( (uint8_t *) encryptMessage, ARC4_LENGTH, rc4Key,len,
//                               De_OutputMessage, &OutputMessageLength);

//  if (status == ARC4_SUCCESS)
//  {

//      /* add application traitment in case of ARC4 SUCCESS */
//			return decryptMessage;
// }
//	else return 0;
//}



/**
  * @brief  ARC4 Encryption example.
  * @param  InputMessage: pointer to input message to be encrypted.
  * @param  InputMessageLength: input data message length in byte.
  * @param  ARC4_Key: pointer to the ARC4 rc4Key to be used in the operation
  * @param  KeyLength: ARC4 rc4Key length in byte
  * @param  OutputMessage: pointer to output parameter that will handle the encrypted message
  * @param  OutputMessageLength: pointer to encrypted message length.
  * @retval error status: can be ARC4_SUCCESS if SUCCESS or one of
  *         ARC4_ERR_BAD_OPERATION, ARC4_ERR_BAD_CONTEXT, ARC4_ERR_BAD_PARAMETER
  *         if error occured.
  */
int32_t STM32_ARC4_Encrypt(uint8_t* InputMessage,
                           uint32_t InputMessageLength,
                           uint8_t  *ARC4_Key,
                           uint32_t KeyLength,
                           uint8_t  *OutputMessage,
                           uint32_t *OutputMessageLength)
{
  ARC4ctx_stt ARC4ctx;

  uint32_t error_status = ARC4_SUCCESS;

  int32_t outputLength = 0;

  /* Set flag field to default value */
  ARC4ctx.mFlags = E_SK_DEFAULT;

  /* Set rc4Key length in the context */
  ARC4ctx.mKeySize = KeyLength;

  /* Initialize the operation, by passing the rc4Key.
   * Third parameter is NULL because ARC4 doesn't use any IV */
  error_status = ARC4_Encrypt_Init(&ARC4ctx, ARC4_Key, NULL );

  /* check for initialization errors */
  if (error_status == ARC4_SUCCESS)
  {
    /* Encrypt Data */
    error_status = ARC4_Encrypt_Append(&ARC4ctx,
                                       InputMessage,
                                       InputMessageLength,
                                       OutputMessage,
                                       &outputLength);

    if (error_status == ARC4_SUCCESS)
    {
      /* Write the number of data written*/
      *OutputMessageLength = outputLength;
      /* Do the Finalization */
      error_status = ARC4_Encrypt_Finish(&ARC4ctx, OutputMessage + *OutputMessageLength, &outputLength);
      /* Add data written to the information to be returned */
      *OutputMessageLength += outputLength;
    }
  }

  return error_status;
}


/**
  * @brief  ARC4 Decryption example.
  * @param  InputMessage: pointer to input message to be decrypted.
  * @param  InputMessageLength: input data message length in byte.
  * @param  ARC4_Key: pointer to the ARC4 rc4Key to be used in the operation
  * @param  KeyLength: ARC4 rc4Key length in byte
  * @param  OutputMessage: pointer to output parameter that will handle the decrypted message
  * @param  OutputMessageLength: pointer to decrypted message length.
  * @retval error status: can be ARC4_SUCCESS if SUCCESS or one of
  *         ARC4_ERR_BAD_OPERATION, ARC4_ERR_BAD_CONTEXT, ARC4_ERR_BAD_PARAMETER
  *         if error occured.
  */
int32_t STM32_ARC4_Decrypt(uint8_t* InputMessage,
                           uint32_t InputMessageLength,
                           uint8_t  *ARC4_Key,
                           uint32_t KeyLength,
                           uint8_t  *OutputMessage,
                           uint32_t *OutputMessageLength)
{
  ARC4ctx_stt ARC4ctx;

  uint32_t error_status = ARC4_SUCCESS;

  int32_t outputLength = 0;

  /* Set flag field to default value */
  ARC4ctx.mFlags = E_SK_DEFAULT;

  /* Set rc4Key length in the context */
  ARC4ctx.mKeySize = KeyLength;

  /* Initialize the operation, by passing the rc4Key.
   * Third parameter is NULL because ECB doesn't use any IV */
  error_status = ARC4_Decrypt_Init(&ARC4ctx, ARC4_Key, NULL );

  /* check for initialization errors */
  if (error_status == ARC4_SUCCESS)
  {
    /* Decrypt Data */
    error_status = ARC4_Decrypt_Append(&ARC4ctx,
                                       InputMessage,
                                       InputMessageLength,
                                       OutputMessage,
                                       &outputLength);

    if (error_status == ARC4_SUCCESS)
    {
      /* Write the number of data written*/
      *OutputMessageLength = outputLength;
      /* Do the Finalization */
      error_status = ARC4_Decrypt_Finish(&ARC4ctx, OutputMessage + *OutputMessageLength, &outputLength);
      /* Add data written to the information to be returned */
      *OutputMessageLength += outputLength;
    }
  }

  return error_status;
}
