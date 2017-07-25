#ifndef _DOOR_CONTROL_H_
#define _DOOR_CONTROL_H_

#include "stm32f10x.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "JSON_Service/cJSON.h"
#include "delay/delay.h"
#include "Lock_Driver.h"
#include "AT24C128/24cxx.h" 
#include "usart1/usart1.h"
#include "JSON_Service/json_infer.h"
#include "Tem_User/Tem_User.h"

#define Menci_Key  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)

void Open_door(void);
void Close_door(void);
uint8_t myStringCopysomelen(const char* Source,char* Destination,const uint8_t lenNeedtoCopy);

typedef struct
{
	char user_name[11];
	char time[21];
	char log_in_word[11];
}open_data;

uint8_t JSON_Open(cJSON *object);
void Menci_config(void);

#endif
