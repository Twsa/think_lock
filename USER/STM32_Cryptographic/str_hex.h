#ifndef __STR_HEX_H_
#define __STR_HEX_H_

#include "stm32f10x.h"
#include "main.h"

void Str2HexV1(uint8_t *StrSource,uint8_t *StrDestination,uint32_t len);
static void abc(uint8_t *buffer1,uint8_t *buffer,int i,int j);
uint8_t Hex2Str(uint8_t *String ,const uint8_t *Hex,uint32_t HexLen);
void Str2HexPain(uint8_t *String ,uint8_t *Hex,uint32_t StringLen);
uint8_t Str2Hex(const uint8_t *String ,uint8_t *Hex,uint32_t StringLen);
#endif
