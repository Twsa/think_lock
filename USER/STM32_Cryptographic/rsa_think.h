
/**
  ******************************************************************************
  * @file    RSA/Enc_Dec/Inc/main.h
  * @author  MCD Application Team
  * @version V3.1.0
  * @date    30-October-2015
  * @brief   Header for main.c module
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RSA_THINK_H_
#define __RSA_THINK_H_

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "crypto.h"
#include "JSON_Service/cJSON.h"
#include "usart1/usart1.h"
#include "stdlib.h"
#include "STM32_Cryptographic/str_hex.h"
#include "Usart_string/Usart_string.h"
#include "JSON_Service/json_infer.h"


typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;
uint8_t RSA_Handler(cJSON *object);
uint8_t RSA_Public_Encrypt(const uint8_t *PainText,
	                         const uint32_t painTextLen,
													 const uint8_t *Modulus,
	                         uint8_t *RSA_Encrypt_Text,
													 const uint32_t bitLen);
													 
//uint8_t const *rc4Encrypt(uint8_t *rc4Key,uint8_t *plainMessage,uint32_t len);
//uint8_t const *rc4Decrypt(uint8_t *rc4Key,uint8_t *encryptMessage,uint32_t len);
//uint8_t RC4_Key(void);

/* Private function prototypes -----------------------------------------------*/

int32_t RSA_Encrypt(RSApubKey_stt *P_pPubKey,
                    const uint8_t *P_pInputMessage,
                    int32_t P_InputSize,
                    uint8_t *P_pOutput);

int32_t RSA_Decrypt(RSAprivKey_stt * P_pPrivKey,
                    const uint8_t * P_pInputMessage,
                    uint8_t *P_pOutput,
                    int32_t *P_OutputSize);
extern TestStatus Buffercmp(const uint8_t* pBuffer, uint8_t* pBuffer1, uint16_t BufferLength);
/* Private function prototypes -----------------------------------------------*/
int32_t STM32_ARC4_Encrypt(uint8_t* InputMessage,
                           uint32_t InputMessageLength,
                           uint8_t  *ARC4_Key,
                           uint32_t KeyLength,
                           uint8_t  *OutputMessage,
                           uint32_t *OutputMessageLength);

int32_t STM32_ARC4_Decrypt(uint8_t* InputMessage,
                           uint32_t InputMessageLength,
                           uint8_t  *ARC4_Key,
                           uint32_t KeyLength,
                           uint8_t  *OutputMessage,
                           uint32_t *OutputMessageLength);
										
uint8_t Arc4Encrypt( uint8_t  *arc4Key, 
                    const uint32_t keyLength, 
										uint8_t *PlainText,
										const uint32_t PainTextLen,
								    uint8_t *MessageOutput	
											);
uint8_t Arc4Decrypt( uint8_t  *arc4Key, 
                    const uint32_t keyLength, 
										uint8_t *CipherText,
										const uint32_t CipherTextLen,
										uint8_t *MessageOutput	
											);	

uint8_t Arc4EncryptNeedStrCiphertext( uint8_t  *arc4Key, 
                    const uint32_t keyLength, 
										uint8_t *PlainText,
										const uint32_t PainTextLen,
								    uint8_t *MessageOutput							
											);										
uint8_t Arc4DecryptFromHexPainText( uint8_t  *arc4Key, 
                    const uint32_t keyLength, 
										uint8_t *CipherText,
										const uint32_t CipherTextLen,          //String�����ĵĳ���
								    uint8_t *MessageOutput							
											);										
/* Exported types --------
----------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __RSA_THINK_H_*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
