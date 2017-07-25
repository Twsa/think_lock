#ifndef _JSON_INFER_H_
#define _JSON_INFER_H_
#include "stm32f10x.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "JSON_Service/cJSON.h"
#include "usart1/usart1.h"
#include "Usart_string/Usart_string.h"
#include "Commen_User/Com_user.h"
#include "Administrator/Admin.h"
#include "Tem_User/Tem_User.h"
#include "AT24C128/24cxx.h" 
#include "Lock_Driver.h"
#include "Door_Control/Door_Control.h"
#include "STM32_Cryptographic/rsa_think.h"

typedef struct
{
	char user_name[NAMELEN+1];
	char log_in_word[LOGPWDLEN+1];
	char open_word[OPENPWDLEN+1];
	char IMEI[IMEILEN+1];
}User;

//char *test_json_creat(void);
//void  cJSON_to_struct_array(void); //peopel是例子的一个结构体


int32_t FindThisUserInAllUserBase_Name_LogPwd(char *RX_name,char *RX_log);
uint8_t SendThisUserLogResult(const int32_t UserLogVerifyStatus);
uint8_t UserLogVerify(cJSON *object);

uint8_t strCopy(unsigned char const* Source,unsigned char * const Destination,const int len);
uint8_t SendResetThisUserResetOpenPwdResult(const int32_t resetThisUserOpenPwdStatus,
	                                          const char *newOpenPwd,const int32_t newOpenPwdLen);
uint8_t ResetThisUserOpenPwd(cJSON *object);


uint8_t SendResetThisUserResetLogPwdResult(const int32_t resetThisUserLogPwdStatus,
	                                          const char *newLogPwd,const int32_t newLogPwdLen);
uint8_t resetThisUserLogPwd(cJSON *object);
//void Reset_logpwd(cJSON *object);
//void Reset_openpwd(cJSON *object);
static uint8_t SoftReset(void);

void JSON_Handle(void);

uint8_t ClearRecord(void);
uint8_t ClearUser(void);
#endif

