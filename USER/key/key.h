#ifndef __KEY_H
#define __KEY_H	 

#define ON    1
#define OFF   0

#define KEY  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2)

void KEY_Init(void);//IO≥ı ºªØ
void KEY_Scan(void);

#endif
