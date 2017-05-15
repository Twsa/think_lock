#include "Commen_User/Com_user.h"

/*
data:2016/10/18
name:Com_set
function:普通用户设置
developer：liuchuanjia
*/
//extern long Open_name_openpwd;
extern uint32_t usernameIndex;
extern uint32_t comuserAdd;//用户增加索引
//extern uint32_t comuserNum;

void Com_set(cJSON *object)
{
	  User Comset={0};
	  cJSON *item=NULL; 
	  char comuserAddBuf[3]={0};
//		char comUserNameBuf[NAMELEN]={0};
		
		uint32_t indexComuser=FAULT;
//		uint32_t indexComuserName=0;
		uint8_t status=FAULT;
//		uint8_t justifyStatus=1;
		
		item=cJSON_GetObjectItem(object,"UserNameValues");//提取增加普通用户的用户名
	  sprintf(Comset.user_name,"%s",item->valuestring);

	  item=cJSON_GetObjectItem(object,"UserPwdValues");//提取增加普通用户的登录密码
	  sprintf(Comset.log_in_word,"%s",item->valuestring);
		
	  item=cJSON_GetObjectItem(object,"UserOpenPwdValues");//提取增加普通用户的开门密码
	  sprintf(Comset.open_word,"%s",item->valuestring);
		usernameIndex+=NAMELEN;//需要验证用户名地址增加
		 
    indexComuser = FindThisUserBase_Name(Comset.user_name);

		if(indexComuser==FAULT)
		{
			status=SUCCESS;
		}
		else if(indexComuser!=FAULT)
		{
//			indexComuserName = indexComuser+OPENPWDLEN;            //寻找当前的用户是否存在，如果已经存在就不能重新设置
//			AT24CXX_Read(indexComuserName,comUserNameBuf,sizeof(comUserNameBuf));
////			printf("indexComuserName:%s\r\n",comUserNameBuf); 
//			justifyStatus=strcmp(Comset.user_name,comUserNameBuf);
//			if(justifyStatus==0)
//			{
				status=FAULT;
//			}
//			else if(justifyStatus!=0)
//			{
//				status=SUCCESS;
//			}
		}
		
		/********************获取当前普通用户设置的个数********************/
		AT24CXX_Read(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf)); 
		comuserAdd=atoi(comuserAddBuf);
		if(comuserAdd==0)
		{
			AT24C128_Save(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf));
		}	 
	  /********************设置普通用户的上限判断********************/
//		printf("comuserAdd/USERLEN:%d\n",comuserAdd/USERLEN);	
		if(((comuserAdd/USERLEN)+1)>COMUSERSETLIMIT)
		{
			status=FAULT;                  //超过预设置的用户上限，不能再添加临时用户
		}
		
		if(status==SUCCESS)
		{ 
	//		printf("Com_set:comuserAdd:%d\n",comuserAdd);		
			comuserAdd+=USERLEN;
			comuserNum=(comuserAdd/USERLEN); //用户的个数	
			sprintf(comuserAddBuf,"%d",comuserAdd);
			AT24C128_Save(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf));

			AT24C128_Save(COMOPENPWDAD,Comset.open_word,OPENPWDLEN);
			AT24C128_Save(COMNAMEAD,Comset.user_name,NAMELEN);//保存用户名
			AT24C128_Save(COMLOGAD,Comset.log_in_word,LOGPWDLEN);//保存登录密码	
			AT24C128_Save(LOGUSERNAME,Comset.user_name,NAMELEN);//b保存开门用户名
		}	
	 Userset_result(status);//用户添加成功返回消息
}


/*************************************************************************************
*  Function Name                    :   FindThisComUser

*  Create Date                      :    2017/03/11
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    利用当前的用户名和当前用户的开门密码
                                         寻找当前的普通用户
                                        

*  Param                            :    char *RX_name,char *RX_log
*  Return Code                      :    None
																			
*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/09 Liu Chuanjia     None           None

**************************************************************************************/
uint32_t FindThisComUserBase_Name_Logpwd(char *RX_name,char *RX_log)
{
	 int index=0;
	 char Name_buf[NAMELEN]={0};
	 char Log_buf[LOGPWDLEN]={0};//保存的用户名和登录密码，等待验证
	 char comuserAddBuf[LIMITLENOPENRCORD/10]={0}; 
	 
	 int verify_name=1,verify_log=1;	 	 
	 
	index=COMBASEAD;      //普通用户信息基地址
	AT24CXX_Read(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf)); 
	comuserAdd=atoi(comuserAddBuf);			
	for (;index<COMLOGAD;)
	{  
		 index+=OPENPWDLEN;
		 AT24CXX_Read(index,Name_buf,NAMELEN);//读取已经保存的用户?
		 index+=NAMELEN;
		 AT24CXX_Read(index,Log_buf,LOGPWDLEN);//读取保存的用户登录密码
		 index=(index-NAMELEN-OPENPWDLEN);
		 verify_name=strcmp(Name_buf,RX_name);	
		 verify_log=strcmp(Log_buf,RX_log);
		 if (verify_name==0&&verify_log==0)
			 break;
			index+=USERLEN;		
	 }
	
	  if (verify_name!=0 || verify_log!=0)
		{			
       return FAULT;
		}
		else
		return index;
}
