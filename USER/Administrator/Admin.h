#ifndef __ADMIN_H_
#define __ADMIN_H_

#include "stm32f10x.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "stm32f10x.h"
#include "JSON_Service/cJSON.h"

#include "JSON_Service/json_infer.h"
#include "Lock_Driver.h"
#include "usart1/usart1.h"
#include "AT24C128/24cxx.h" 
#include "STM32_ID/chipid.h"
#include "Door_Control/Door_Control.h"

uint8_t Admin_set(cJSON *object);        
//设置新用户返回设置结果
uint8_t Userset_result(uint8_t status);

uint8_t Opendata_A(cJSON *object);
typedef struct 
{
	long index;//从基地址开始读取所有用户信息
	long index_change;//从基地址开始读取所有用户信息
}checkList;

typedef struct
	{
		int32_t index_name;
		int32_t index_time;
		int32_t index_startTime;
		int32_t index_endTime;
		int32_t index_len;
	}openIndex;
	

static int8_t myStringCompare(const char* Source,const char* Destination,const uint8_t lenNeedtoCompare);
static openIndex OpenrecordTimequantum(const char* const firstDay,const char* const lastDay);
uint8_t DeleteRecord(cJSON *object);

static checkList CheckListUserVerify(checkList checkIndex);
static uint8_t CheckListJson(const uint32_t checkListNumb,const uint32_t thisCheckListNumb,
	                             uint32_t checkListNumbTemp,checkList checkIndex);
uint8_t Check_Listing(void);
uint8_t User_det(cJSON *object);
uint8_t detResult_send (int det_status);
int Detleting(long detUser_base);
static uint8_t OpenDataJson(const uint32_t openLockHistoryNumb,const uint32_t thisHistoryNumb,
	                           uint32_t checkAccountNumTemp,openIndex OpenrecordIndex);
uint8_t Change_temcount(cJSON *object);
uint32_t FindThisUserBase_Name(char *RX_name);
//Change_temcount(object);
#endif

