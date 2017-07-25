#include "Tem_User/Tem_User.h"

/*
data:2016/10/18
name:Tem_set
function:临时用户注册
developer：liuchuanjia
*/

extern uint32_t temuserAdd;
extern uint32_t usernameIndex;
extern uint32_t temuserNum;

void Tem_set(cJSON *object)
{
	 	User Temset={0};
	  cJSON *item=NULL; 
	  char count_buffer[3]={0};
		char temuserAddBuf[3]={0};
//		char TemUserNameBuf[NAMELEN]={0};
		
    uint32_t indexTemuser=FAULT;
//		uint32_t indexTemuserName=0;
		uint8_t status=FAULT;
//		uint8_t justifyStatus=1;
		
		item=cJSON_GetObjectItem(object,"UserNameValues");   //提取增加普通用户的用户名
	  sprintf(Temset.user_name,"%s",item->valuestring);
		
	  item=cJSON_GetObjectItem(object,"UserPwdValues");    //提取增加普通用户的登录密码
	  sprintf(Temset.log_in_word,"%s",item->valuestring);
		
	  item=cJSON_GetObjectItem(object,"UserOpenPwdValues");//提取增加普通用户的开门密码
	  sprintf(Temset.open_word,"%s",item->valuestring);
		
	  item=cJSON_GetObjectItem(object,"UserOpenCountValues");//提取增加普通用户的开门密码
	  sprintf(count_buffer,"%s",item->valuestring);
		
		usernameIndex+=NAMELEN;                              //需要验证用户名地址增加
		indexTemuser = FindThisUserBase_Name(Temset.user_name);
    
		if(indexTemuser==FAULT)
		{
			status=SUCCESS;
		}
		else if(indexTemuser!=FAULT)
		{
//			indexTemuserName = indexTemuser+OPENPWDLEN;            //寻找当前的用户是否存在，如果已经存在就不能重新设置
//			AT24CXX_Read(indexTemuserName,TemUserNameBuf,sizeof(TemUserNameBuf));
////			printf("indexComuserName:%s\r\n",comUserNameBuf); 
//			justifyStatus=strcmp(Temset.user_name,TemUserNameBuf);
//			if(justifyStatus==0)
//			{
				status=FAULT;
//			}
//			else if(justifyStatus!=0)
//			{
//				status=SUCCESS;
//			}
		}
		
    /********************获取当前临时用户设置的个数********************/
		AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf));		
		temuserAdd=atoi(temuserAddBuf);
		if(temuserAdd==0)
		{
			AT24C128_Save(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf));
		}
	  /********************设置临时用户的上限判断********************/
		if(temuserAdd/(USERLEN+TEMCOUNTLEN)>TEMUSERSETLIMIT)
		{
			status=FAULT;                  //超过预设置的用户上限，不能再添加临时用户
		}
//		printf("Tem_set:temuserAdd:%d\n",temuserAdd);		
      
		if(status==SUCCESS)
		{
			temuserAdd+=(USERLEN+TEMCOUNTLEN);//临时用户索引增加
			temuserNum=(temuserAdd/(USERLEN+TEMCOUNTLEN));
			sprintf(temuserAddBuf,"%d",temuserAdd);
			AT24C128_Save(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf));
			
			AT24C128_Save(TEMOPENPWDAD,Temset.open_word,OPENPWDLEN);	
			AT24C128_Save(TEMNAMEAD,Temset.user_name,NAMELEN);//保存用户名
			AT24C128_Save(TEMLOGAD,Temset.log_in_word,LOGPWDLEN);//保存登录密码
			AT24C128_Save(LOGUSERNAME,Temset.user_name,sizeof(Temset.user_name));	
			AT24C128_Save(TEMCOUNTAD,count_buffer,sizeof(count_buffer));
		}	
		
    Userset_result(status);//用户添加成功返回消息
}

///*************************************************************************************
//*  Function Name                    :   FindThisTemUser

//*  Create Date                      :    2017/03/09
//*  Author/Corporation               :    Liu Chuanjia
//*  Description                      :    寻找当前的临时用户
//                                        

//*  Param                            :    char *RX_name,char *RX_log
//*  Return Code                      :    None
//																			
//*  Global Variable                  :    None
//*  File Static Variable             :    None
//*  Function Static Variable         :    None
//																			 
//*--------------------------------Revision History--------------------------------------
//*  No         Version         Date        RevisedBy        Item         Description  
//*  1          V1.0            2017/03/09 Liu Chuanjia     None           None

//**************************************************************************************/
//uint32_t FindThisTemUserBase_Name(char *RX_name)
//{
//	 uint32_t index=0;
//	 char Name_buf[NAMELEN]={0};
////	 char Log_buf[LOGPWDLEN]={0};//保存的用户名和登录密码，等待验证

//   char temuserAddBuf[LIMITLENOPENRCORD/10]={0}; 
//	 
//	 int verify_name=1;
////		 verify_log=1;	 	 
//	 
//		index=TEMBASEAD;
//		AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf)); 
//		temuserAdd=atoi(temuserAddBuf);				
//		for (;index<TEMLOGAD;)
//		{  		
//			 index+=OPENPWDLEN;
//			 AT24CXX_Read(index,Name_buf,NAMELEN);//读取已经保存的用户名
////			 printf("Name_buf:%s\r\n",Name_buf);
////			 index+=NAMELEN;
////			 AT24CXX_Read(index,Log_buf,LOGPWDLEN);//读取保存的用户登录密码		
////       printf("Log_buf:%s\r\n",Log_buf);			
//			 index=(index-OPENPWDLEN);
//			 verify_name=strcmp(Name_buf,RX_name);	
////			 verify_log=strcmp(Log_buf,RX_log);
//			 if (verify_name==0)
//				 break;
//			 index+=(USERLEN+TEMCOUNTLEN);
//		}				
//			
//	  if (verify_name!=0)
//		{			
//       return FAULT;
//		}

////			printf("index:%d\r\n",index);//用户登录地址调试信息
////		  AT24CXX_Read(index,Openwd_Send,OPENPWDLEN);//读取开门密码
////		printf("index:%s\r\n",Openwd_Send);
//		else
//		return index;
//}
/*************************************************************************************
*  Function Name                    :   FindThisTemUser

*  Create Date                      :    2017/03/09
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    寻找当前的临时用户
                                        

*  Param                            :    char *RX_name,char *RX_log
*  Return Code                      :    None
																			
*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/09 Liu Chuanjia     None           None

**************************************************************************************/
uint32_t FindThisTemUserBase_Name_Logpwd(char *RX_name,char *RX_log)
{
	 uint32_t index=0;
	 char Name_buf[NAMELEN]={0};
	 char Log_buf[LOGPWDLEN]={0};//保存的用户名和登录密码，等待验证

   char temuserAddBuf[LIMITLENOPENRCORD/10]={0}; 
	 
	 int verify_name=1,verify_log=1;	 	 
	 
		index=TEMBASEAD;
		AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf)); 
		temuserAdd=atoi(temuserAddBuf);				
		for (;index<TEMLOGAD;)
		{  		
			 index+=OPENPWDLEN;
			 AT24CXX_Read(index,Name_buf,NAMELEN);//读取已经保存的用户名
//			 printf("Name_buf:%s\r\n",Name_buf);
			 index+=NAMELEN;
			 AT24CXX_Read(index,Log_buf,LOGPWDLEN);//读取保存的用户登录密码		
//       printf("Log_buf:%s\r\n",Log_buf);			
			 index=(index-NAMELEN-OPENPWDLEN);
			 verify_name=strcmp(Name_buf,RX_name);	
			 verify_log=strcmp(Log_buf,RX_log);
			 if (verify_name==0&&verify_log==0)
				 break;
			 index+=(USERLEN+TEMCOUNTLEN);
		}				
			
	  if (verify_name!=0 || verify_log!=0)
		{			
       return FAULT;
		}
		else
		return index;
}
