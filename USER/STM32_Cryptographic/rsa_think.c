
/**
  ******************************************************************************
  * @file    RSA/Enc_Dec/Src/main.c
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    30-October-2015
  * @brief   Main program body
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "STM32_Cryptographic/rsa_think.h"

/**
  ******************************************************************************
  * @file    RSA/Enc_Dec/Src/main.c
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    30-October-2015
  * @brief   Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  * http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/

/** @addtogroup STM32_Crypto_Examples
  * @
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/** @addtogroup STM32_Crypto_Examples
  * @{
  */
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

uint8_t preallocated_buffer[4096]; /* buffer required for internal allocation of memory */

/******************************************************************************/
/************************** RSA 512 Test Vector  ****************************/
/******************************************************************************/
/* String of entropy */
uint8_t entropy_data[32] =
  {
    0x91, 0x20, 0x1a, 0x18, 0x9b, 0x6d, 0x1a, 0xa7,
    0x0e, 0x69, 0x57, 0x6f, 0x36, 0xb6, 0xaa, 0x88,
    0x55, 0xfd, 0x4a, 0x7f, 0x97, 0xe9, 0x72, 0x69,
    0xb6, 0x60, 0x88, 0x78, 0xe1, 0x9c, 0x8c, 0xa5
  };

/* Private functions ---------------------------------------------------------*/

/*************************************************************************************
*  Function Name                        :    RSA_Public_Encrypt

*  Create Date                          :    2017/3/17
*  Author/Corporation                   :    Liu Chuanjia
*  Description                          :    基于STM32F103ZET6 RSA公钥加密接口


*  Param                                :    const uint8_t *PainText,
										                         uint8_t RSA_Encrypt_*const uint32_t bitLen

*  Return Code                          :    None

*  Global Variable                      :    None
*  File Static Variable                 :    None
*  Function Static Variable                  :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/3/17 Liu Chuanjia     None           None

**************************************************************************************/
//bitLen=512
uint8_t RSA_Public_Encrypt(const uint8_t *PainText,
	                         const uint32_t painTextLen,
													 const uint8_t *Modulus,
	                         uint8_t *RSA_Encrypt_Text,
													 const uint32_t bitLen)
{
//  const uint8_t Message[] ="Hello World";
	if(bitLen<8)
	{
		Error_Handler();
		return FAULT;
	}
	uint32_t modelusHexLen=bitLen/8;    //bit=512位时modelusHexLen为64
	uint32_t modelusStrLen=bitLen/4;    //bit=512是modelusStrLen为128
	uint32_t msgRealLen=0;              //记录传进来的明文的真实长度
	
	uint8_t *modulusHexAndMsgHexOut=(uint8_t *)malloc(modelusHexLen);//modulus的Hex形式	
	int32_t status = RSA_ERR_GENERIC;
//	uint8_t *pubEncrypt=(uint8_t *)malloc(modelusStrLen);   //用String形式表示的的RSA公钥加密的密文

 uint8_t PublicExponent[]=
  {
    0x01, 0x00, 0x01
  };

  while(*(PainText+msgRealLen++)!='\0');  //采集传进来的明文的真实长度 加了一个字节，追加'\0'
	
  RSApubKey_stt PubKey_st;
 
	Str2Hex(Modulus,modulusHexAndMsgHexOut,modelusStrLen);

  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
	
  /* Testing Encryption and Decryption */
  /* Preparing for Encryption */

	PubKey_st.mExponentSize = sizeof(PublicExponent);        //PublicExponent 公钥指数的Hex的长度
  PubKey_st.mModulusSize = modelusHexLen;                  //Modules的Hex长度
  PubKey_st.pmExponent = (uint8_t *) PublicExponent;       //公钥
  PubKey_st.pmModulus = (uint8_t *)modulusHexAndMsgHexOut; //Modules
 
	/**************************************RSA公钥加密****************************/
  status = RSA_Encrypt(&PubKey_st, PainText,msgRealLen, modulusHexAndMsgHexOut);
  
	if (status == RSA_SUCCESS)  //加密成功
  {
			//	printf("PlainText:%s\n",Message);
		/***********************将加密后的密文从Hex形式转化成String的形式***************/
		Hex2Str((uint8_t *)RSA_Encrypt_Text,(uint8_t *)modulusHexAndMsgHexOut,modelusHexLen);	
	}
  else
	{
		Error_Handler();
		return FAULT;
	}
	free(modulusHexAndMsgHexOut);
  /* Turn on the green led in case of RSA encryption/decryption operations are successful*/
//  HAL_GPIO_WritePin(LED2_GPIO_PORT, LED2_PIN, GPIO_PIN_SET);
//  while (1)
//  {}
 return SUCCESS;
}
/*************************************************************************************
*  Function Name                        :    RSA_SendMsg

*  Create Date                          :    2017/3/17
*  Author/Corporation                   :    Liu Chuanjia
*  Description                          :    RSA操作之后返回消息


*  Param                                :    const uint8_t RSA_EncryptStatus,const uint8_t *rc4KeyEncrypt

*  Return Code                          :    None

*  Global Variable                      :    None
*  File Static Variable                 :    None
*  Function Static Variable                  :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/3/17 Liu Chuanjia     None           None

**************************************************************************************/
static void RSA_SendMsg(const uint8_t RSA_EncryptStatus,const uint8_t *rc4KeyEncrypt)
{
	 cJSON *flag,*root;
	 char *out=NULL;
	
		root=cJSON_CreateObject();     	
	  cJSON_AddItemToObject(root, "data", flag=cJSON_CreateObject());	
	  
	  if(RSA_EncryptStatus==SUCCESS)
		{
			 cJSON_AddStringToObject(flag,"status","true");
			 cJSON_AddStringToObject(flag,"rc4KeyEncrypt",(char *)rc4KeyEncrypt);
		}
    else if(RSA_EncryptStatus==FAULT)
		{
			cJSON_AddStringToObject(flag,"status","false");
		}
		cJSON_AddStringToObject(root,"flag","2");
		out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);
		free(out);
}
/*************************************************************************************
*  Function Name                        :    RSA_Handler

*  Create Date                          :    2017/3/17
*  Author/Corporation                   :    Liu Chuanjia
*  Description                          :    用RSA加密算法公钥加密RC4的密钥


*  Param                                :    

*  Return Code                          :    None

*  Global Variable                      :    None
*  File Static Variable                 :    None
*  Function Static Variable                  :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/3/17 Liu Chuanjia     None           None

**************************************************************************************/
#define RSABITLEN  512
#define RSAENCRYPTLEN (RSABITLEN/4+1)

uint8_t RC4_TestKey[]="12345678";

uint8_t RSA_Handler(cJSON *object)
{
	  cJSON *item=NULL; 
	  uint8_t RSA_Encrypt_Text[RSAENCRYPTLEN]={0};
		uint8_t Modulus[RSAENCRYPTLEN]={0};
		uint8_t status=0;
		
		item=cJSON_GetObjectItem(object,"Modulus");   //提取增加普通用户的用户名
	  memcpy(Modulus,item->valuestring,RSAENCRYPTLEN);
		
	/***************RSA加密过程*****************/
	 status=RSA_Public_Encrypt(RC4_TestKey,
	                        sizeof(RC4_TestKey),
													 Modulus,
	                         RSA_Encrypt_Text,
													 RSABITLEN);
		RSA_SendMsg(status,RSA_Encrypt_Text);											
		return 0;
}
/**
  * @brief  RSA Encryption with PKCS#1v1.5
  * @param  P_pPubKey The RSA public key structure, already initialized
  * @param  P_pInputMessage Input Message to be signed
  * @param  P_MessageSize Size of input message
  * @param  P_pOutput Pointer to output buffer
  * @retval error status: can be RSA_SUCCESS if success or one of
  * RSA_ERR_BAD_PARAMETER, RSA_ERR_MESSAGE_TOO_LONG, RSA_ERR_BAD_OPERATION
*/
int32_t RSA_Encrypt(RSApubKey_stt *P_pPubKey,
                    const uint8_t *P_pInputMessage,
                    int32_t P_InputSize,
                    uint8_t *P_pOutput)
{
  int32_t status = RNG_SUCCESS ;
  RNGstate_stt RNGstate;
  RNGinitInput_stt RNGinit_st;
  RNGinit_st.pmEntropyData = entropy_data;
  RNGinit_st.mEntropyDataSize = sizeof(entropy_data);
  RNGinit_st.mPersDataSize = 0;
  RNGinit_st.mNonceSize = 0;

  status = RNGinit(&RNGinit_st, &RNGstate);
  if (status == RNG_SUCCESS)
  {
    RSAinOut_stt inOut_st;
    membuf_stt mb;

    mb.mSize = sizeof(preallocated_buffer);
    mb.mUsed = 0;
    mb.pmBuf = preallocated_buffer;

    /* Fill the RSAinOut_stt */
    inOut_st.pmInput = P_pInputMessage;
    inOut_st.mInputSize = P_InputSize;
    inOut_st.pmOutput = P_pOutput;

    /* Encrypt the message, this function will write sizeof(modulus) data */
    status = RSA_PKCS1v15_Encrypt(P_pPubKey, &inOut_st, &RNGstate, &mb);
  }
  return(status);
}

/**
  * @brief  RSA Decryption with PKCS#1v1.5
  * @param  P_pPrivKey The RSA private key structure, already initialized
  * @param  P_pInputMessage Input Message to be signed
  * @param  P_MessageSize Size of input message
  * @param  P_pOutput Pointer to output buffer
  * @retval error status: can be RSA_SUCCESS if success or RSA_ERR_GENERIC in case of fail
*/ 
int32_t RSA_Decrypt(RSAprivKey_stt * P_pPrivKey,
                    const uint8_t * P_pInputMessage,
                    uint8_t *P_pOutput,
                    int32_t *P_OutputSize)
{
  int32_t status = RSA_SUCCESS ;
  RSAinOut_stt inOut_st;
  membuf_stt mb;

  mb.mSize = sizeof(preallocated_buffer);
  mb.mUsed = 0;
  mb.pmBuf = preallocated_buffer;

  /* Fill the RSAinOut_stt */
  inOut_st.pmInput = P_pInputMessage;
  inOut_st.mInputSize = P_pPrivKey->mModulusSize;
  inOut_st.pmOutput = P_pOutput;

  /* Encrypt the message, this function will write sizeof(modulus) data */
  status = RSA_PKCS1v15_Decrypt(P_pPrivKey, &inOut_st, P_OutputSize, &mb);
  return(status);
}




/**
  * @brief  Compares two buffers.
  * @param  pBuffer, pBuffer1: buffers to be compared.
  * @param  BufferLength: buffer's length
  * @retval PASSED: pBuffer identical to pBuffer1
  *         FAILED: pBuffer differs from pBuffer1
  */
TestStatus Buffercmp(const uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength)
{
  while (BufferLength--)
  {
    if (*pBuffer != *pBuffer1)
    {
      return FAILED;
    }

    pBuffer++;
    pBuffer1++;
  }

  return PASSED;
}




/******************* (C) COPYRIGHT 2015 STMicroeleCCMonics *****END OF FILE****/
