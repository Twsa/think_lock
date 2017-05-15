//验证用户类型，进行用户信息验证，执行开门命令，然后反馈用户信息
//Led指示，检查门是否关上
#include "Door_Control/Door_Control.h"


unsigned char Opendoor_Staus=CLOSED;

void Open_door(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 	
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  GPIO_ResetBits(GPIOB, GPIO_Pin_0);
//  GPIO_SetBits(GPIOB,GPIO_Pin_1);	 //来个延时
//	delay_ms(500);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_0);
  GPIO_ResetBits(GPIOB, GPIO_Pin_1);
  GPIO_SetBits(GPIOB,GPIO_Pin_0);	 //来个延时
	delay_ms(500);
  GPIO_ResetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_0);	
	Opendoor_Staus=OPENED;
	TIM_Cmd(TIM2, ENABLE);		
}
void Close_door(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_0 | GPIO_Pin_1;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 	
  GPIO_Init(GPIOB, &GPIO_InitStructure);
//  GPIO_ResetBits(GPIOB, GPIO_Pin_1);
//  GPIO_SetBits(GPIOB,GPIO_Pin_0);	 //来个延时
//	delay_ms(500);
//  GPIO_ResetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_0);	

  GPIO_ResetBits(GPIOB, GPIO_Pin_0);
  GPIO_SetBits(GPIOB,GPIO_Pin_1);	 //来个延时
	delay_ms(500);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_0);
	Opendoor_Staus=CLOSED;
	TIM_Cmd(TIM2, DISABLE);		
}

/*************************************************************************************
*  Function Name                    :   static myStringCopysomelen

*  Create Date                      :    2017/01/07
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    按一定长度，字符串复制
                                        

*  Param                            :    const char* Source,char* Destination,const uint8_t lenNeedtoCopy

*  Return Code                      :    None
																			
*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017.1.07  Liu Chuanjia     None           None

**************************************************************************************/
uint8_t myStringCopysomelen(const char* Source,char* Destination,const uint8_t lenNeedtoCopy)
{
	uint8_t lenNeedtoCopyTem=0;lenNeedtoCopyTem=lenNeedtoCopy;
	for(lenNeedtoCopyTem=0;lenNeedtoCopyTem<lenNeedtoCopy;lenNeedtoCopyTem++)
	{
	  *(Destination++)=*(Source++);
	}
	return 0;
}

/*************************************************************************************
*  Function Name                    :   JSON_Open

*  Create Date                      :    2016/10/21
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    开门
                                        

*  Param                            :    cJSON *object

*  Return Code                      :    None
																			
*  Global Variable                  :    usernameIndex,openuserIndex,temopenCount
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016/10/21  Liu Chuanjia     None           None

**************************************************************************************/

uint32_t usernameIndex=0;
uint32_t openuserIndex=0;

uint8_t JSON_Open(cJSON *object)
{
	  open_data Open; 
	  cJSON *item,*flag,*root;
	  int8_t temopenCount=0;
    uint32_t indexTemUser=0;
    uint32_t indexTemCount=0;
	  
	  char *out;char UserType[2]={0};
    char temCount_buf[TEMCOUNTLEN]={0};
		char openuserIndexBuf[LIMITLENOPENRCORD/10]={0};//开门用户地址缓存
    char comuserAddBuf[LIMITLENOPENRCORD/10]={0};
    char temuserAddBuf[LIMITLENOPENRCORD/10]={0};
//		char indextemcountBuf[LIMITLENOPENRCORD/10]={0};

		item=cJSON_GetObjectItem(object,"UserNameValues");
		memcpy(Open.user_name,item->valuestring,sizeof(Open.user_name));
		
		item=cJSON_GetObjectItem(object,"UserPwdValues");
		memcpy(Open.log_in_word,item->valuestring,sizeof(Open.log_in_word));
		
		item=cJSON_GetObjectItem(object,"Time");
		memcpy(Open.time,item->valuestring,sizeof(Open.time));
	  
	  item=cJSON_GetObjectItem(object,"userType");
		memcpy(UserType,item->valuestring,sizeof(UserType));
	
			
		root=cJSON_CreateObject();     	
	  cJSON_AddItemToObject(root, "data", flag=cJSON_CreateObject());	
			
			if (UserType[0]=='1'||UserType[0]=='2')
			{			
				AT24CXX_Read(INDEXOPENUSER,openuserIndexBuf,sizeof(openuserIndexBuf));
				openuserIndex=atoi(openuserIndexBuf);
				AT24CXX_Read(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf)); 
				comuserAdd=atoi(comuserAddBuf);
				
				if(openuserIndex==0)
				{
					AT24C128_Save(INDEXOPENUSER,openuserIndexBuf,sizeof(openuserIndexBuf));			
				}
				cJSON_AddStringToObject(flag,"openResult","true");
				cJSON_AddStringToObject(root,"flag","6");
				out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);
				free(out);
				Open_door();//验证成功，接下来开门
				Menci_config();	
				
//			  printf("USERNAMEOPENRE:%d\r\n",USERNAMEOPENRE);
				AT24C128_Save(USERNAMEOPENRE,Open.user_name,NAMELEN);//开门哦成功，保存用户信息之用户名保存
				AT24C128_Save(TIMEOPENRE,Open.time,TIMELEN);               //保存用户开门的时间
			
				openuserIndex+=OPENRECORDLEN;         //开门用户信息地址增加
				sprintf(openuserIndexBuf,"%d",openuserIndex);
				AT24C128_Save(INDEXOPENUSER,openuserIndexBuf,sizeof(openuserIndexBuf));			
			}
			else if (UserType[0]=='3')
			{			
				AT24CXX_Read(INDEXOPENUSER,openuserIndexBuf,sizeof(openuserIndexBuf));
				openuserIndex=atoi(openuserIndexBuf);
				AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf)); 
				temuserAdd=atoi(temuserAddBuf);	
//				printf("\r\nOpen.user_name,Open.log_in_word:%s\r\n%s\r\n",Open.user_name,Open.log_in_word);
				indexTemUser=FindThisTemUserBase_Name_Logpwd(Open.user_name,Open.log_in_word);		 //	查询当前的临时用户，获取当前临时用户的开门次数信息	
				if(indexTemUser!=FAULT)           //如果找到了当前临时用户获取当前允许的开锁次数
				{
					 indexTemCount=indexTemUser+USERLEN;
//				printf("indextemcount:%d\n",indextemcount);
				
				 AT24CXX_Read(indexTemCount,temCount_buf,TEMCOUNTLEN);
				 temopenCount=atoi(temCount_buf);			
//         printf("temopenCount:%d\r\n",temopenCount);	
				}
			  else if(indexTemUser==FAULT)
				{
					temopenCount=-1;
				}                                 //如果没有找到当前用户默认开锁次数为-1
				          
				if(temopenCount<=0)	
				{
					cJSON_AddStringToObject(flag,"openResult","false");
					cJSON_AddStringToObject(root,"flag","6");
					out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);
					free(out);				
				}
				else if (temopenCount>0)
				{			         
				  temopenCount--;
				  sprintf(temCount_buf,"%d",temopenCount);
          
					/******将当前的剩余的开锁次数保存在当前用户的地址***************/
				  AT24C128_Save(indexTemCount,temCount_buf,TEMCOUNTLEN);
					
						if(openuserIndex==0)
						{
							AT24C128_Save(INDEXOPENUSER,openuserIndexBuf,sizeof(openuserIndexBuf));			
						}
						
					cJSON_AddStringToObject(flag,"openResult","true");
					cJSON_AddStringToObject(root,"flag","6");
					out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);
					free(out);						
					Open_door();//验证成功，接下来开门
			    Menci_config();	
						
					AT24C128_Save(USERNAMEOPENRE,Open.user_name,NAMELEN);//开门哦成功，保存用户信息之用户名保存				
					AT24C128_Save(TIMEOPENRE,Open.time,TIMELEN);               //保存用户开门的时间
						
					openuserIndex+=OPENRECORDLEN;         //开门用户信息地址增加
					sprintf(openuserIndexBuf,"%d",openuserIndex);
					AT24C128_Save(INDEXOPENUSER,openuserIndexBuf,sizeof(openuserIndexBuf));			
				}		
			}	
       return 0;			
}			
/*
data:2016/10/23
name:Menci_close
function:门磁控制
developer：liuchuanjia
*/

void Menci_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//使能PORTA时钟
	 	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //设置成上拉输入
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA
}
