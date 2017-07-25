#include "JSON_Service/json_infer.h"

uint8_t ClearRecord(void)
{
  char clearBuffer[3]={0};
	AT24C128_Save(INDEXOPENUSER,clearBuffer,sizeof(clearBuffer));			//测试清空开锁记录函数

  return 0;
}

uint8_t ClearUser(void)
{
	  char clearBuffer[3]={0};
		AT24C128_Save(INDEXCOMUSERADD,clearBuffer,sizeof(clearBuffer));   //清空普通用户
    AT24C128_Save(INDEXTEMUSERADD,clearBuffer,sizeof(clearBuffer));   //清空临时用户
		
		return 0;
}

void JSON_Handle(void)
{ 
	cJSON *json=NULL,*item=NULL,*object=NULL; 
  uint16_t index=0;
	
  char flag[3]={0};
  char public_Buffer[RXBUFLEN]={0};

//  Check_Listing();
	
	Usart_str(public_Buffer);
	
	json=cJSON_Parse(public_Buffer);  
	object=cJSON_GetObjectItem(json,"data");

	item= cJSON_GetObjectItem(json,"flag");
  memcpy(flag,item->valuestring,sizeof(flag));
	
	if (flag[1]>='0'&&flag[1]<='9')
	index=(flag[0]-'0')*10+(flag[1]-'0');
	else index=flag[0]-'0';
   

	switch (index)
	{
		 case RSAHANDLER:RSA_Handler(object);cJSON_Delete(json);break;//接受公钥并加密	
		
		 case USELOG:
			 UserLogVerify(object);cJSON_Delete(json);SoftReset();break; //用户登录验证
		 case OPENDOOR:
			 JSON_Open(object);cJSON_Delete(json);break;       //开门
		 case CHECKUSERLIST:
			 cJSON_Delete(json);Check_Listing();break;         //获取申请用户列表
		 case SETCOMUSER:
			 Com_set(object);cJSON_Delete(json);break;         //普通用户命令集
		 case DETUSER:
			 User_det(object);cJSON_Delete(json);break;       //删除用户信息 管理员删除普通用户和删除临时用户
		 case CHECKOPENRECORD:Opendata_A(object);cJSON_Delete(json);break;        //管理员申请查看开锁记录
		 case RESETOPENPWD:
			  ResetThisUserOpenPwd(object);cJSON_Delete(json);break;//修改开锁密码
		 case SETTEMUSER:
			 Tem_set(object);cJSON_Delete(json);break;     //临时用户命令集
		 case SETADMINUSER:
			 Admin_set(object);cJSON_Delete(json);break;   //管理员命令集
		 case RESETLOGPWD:
			  resetThisUserLogPwd(object);cJSON_Delete(json);break;//更改用户的登录密码
		 case CHANGETEMCOUNT:
			 Change_temcount(object);cJSON_Delete(json);break; //修改临时用户的开锁次数
	//	 case 26:RC4_Key(void)                                      //接受RSA验证成功消息，然后发送RC4密钥  
//	 case 50:
	  case 23:ClearRecord();break;
	  case 24:ClearUser();break;
		case DELETERECOREFLAG:DeleteRecord(object);cJSON_Delete(json);break;       //删除开锁记录
	default:break;
	}
} 

/*************************************************************************************
*  Function Name                    :    SoftReset

*  Create Date                      :    2017/01/06
*  Author/Corporation               :   
*  Description                      :    软件复位


*  Param                            :    None

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017.01.06                    None           None

**************************************************************************************/
static uint8_t SoftReset(void)
{
  __set_FAULTMASK(1);      // 关闭所有中端
  NVIC_SystemReset();      // 复位
	return 0;
}
/*************************************************************************************
*  Function Name                    :   FindThisUserInAllUserBase_Name_LogPwd

*  Create Date                      :    2017/03/11
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    利用当前用户的用户名和登录密码寻找当前用户的Bsae地址
                                        

*  Param                            :    cJSON *object
*  Return Code                      :    当前用户的Bsae地址  如果没有找到当前用户返回(uint32_t)0
																			
*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/11 Liu Chuanjia     None           None

**************************************************************************************/
int32_t FindThisUserInAllUserBase_Name_LogPwd(char *RX_name,char *RX_log)
{
	 int32_t index=0;
	 char Name_buf[NAMELEN]={0};
	 char Log_buf[LOGPWDLEN]={0};                  //保存的用户名和登录密码，等待验证
	 char comuserAddBuf[LIMITLENOPENRCORD/10]={0};
   char temuserAddBuf[LIMITLENOPENRCORD/10]={0}; 

	 
	 int verify_name=1,verify_log=1;	 	 
	 
    index=ADMINADDRESS;
		for (;index<ADLOGAD;)
		{  
			 index+=OPENPWDLEN;
	     AT24CXX_Read(index,Name_buf,NAMELEN);//读取已经保存的用户名
			 index+=NAMELEN;
			 AT24CXX_Read(index,Log_buf,LOGPWDLEN);//读取保存的用户登录密码
       index=(index-NAMELEN-OPENPWDLEN);		
			
			 verify_name=strcmp(Name_buf,RX_name);	
			 verify_log=strcmp(Log_buf,RX_log);
			
			 if (verify_name==0&&verify_log==0)
				 break;
				index+=USERLEN;
		}                                        //在管理员用户区没有找到当前用户
		if (verify_name!=0 || verify_log!=0)
		{
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
			}                                       //在普通用户区没有找到当前用户
		}
		if (verify_name!=0 || verify_log!=0)
		{
			index=TEMBASEAD;
			AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf)); 
	    temuserAdd=atoi(temuserAddBuf);				     
			for (;index<TEMLOGAD;)
			{  		
				 index+=OPENPWDLEN;
				 AT24CXX_Read(index,Name_buf,NAMELEN);//读取已经保存的用户名
				 index+=NAMELEN;
				 AT24CXX_Read(index,Log_buf,LOGPWDLEN);//读取保存的用户登录密码		 
				 index=(index-NAMELEN-OPENPWDLEN);
				 verify_name=strcmp(Name_buf,RX_name);	
				 verify_log=strcmp(Log_buf,RX_log);
				 if (verify_name==0&&verify_log==0)
					 break;
				 index+=(USERLEN+TEMCOUNTLEN);
			}				
		}		                                    //在临时用户区没有找到当前用户
//	  if (verify_name!=0 || verify_log!=0)
//		{			
//			  root=cJSON_CreateObject();             //创建验证对象结构体
//		    cJSON_AddItemToObject(root, "data", img=cJSON_CreateObject());
//				
//				cJSON_AddStringToObject(img,"userType","0");
//			  cJSON_AddStringToObject(img,"UserOpenPwdValues","0");
//			  cJSON_AddStringToObject(root,"flag","4");
//				out=cJSON_Print(root);	cJSON_Delete(root);	
//			  printf("%s\n",out);
//			  free(out);	
//		}
		if (verify_name!=0 || verify_log!=0)
		{			
       return FAULT;
		}
		else return index;              //指示验证当前用户的登录验证成功的状态，
		                                //也是返回当前用户的Base地址
}
/*************************************************************************************
*  Function Name                    :    SendThisUserLogResult

*  Create Date                      :    2017/03/11
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    发送当前用户的登录结果
                                        

*  Param                            :    cJSON *object
*  Return Code                      :    SUCCESS,FAULT
																			
*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    int32_t UserLogVerifyStatus
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/11 Liu Chuanjia     None           None
**************************************************************************************/
uint8_t SendThisUserLogResult(const int32_t UserLogVerifyStatus)
{
	cJSON *root=NULL,*img=NULL; char *out=NULL;
  int32_t index=UserLogVerifyStatus;
	
	char thisUserLogPwd[LOGPWDLEN]={0};
	char thisUserLogPwdArc4En[2*LOGPWDLEN]={0};
	
	root=cJSON_CreateObject();             //创建验证对象结构体
  cJSON_AddItemToObject(root, "data", img=cJSON_CreateObject());
	/************测试时使用的arc4Key*************/
//	char thisUserLogPwd[3]="123";
  uint8_t arc4Key[8]="12345678";
	/************测试时使用的arc4Key*************/

	if(UserLogVerifyStatus != FAULT)                  //当前用户登录信息验证成功
	{	
		AT24CXX_Read(index,thisUserLogPwd,OPENPWDLEN);   //读取当前用户的的开门密码
			/****************对管理员用户的开门密码进行加密**************/
//			Arc4Encrypt(arc4Key,sizeof(arc4Key),(uint8_t *)thisUserLogPwd,
//									sizeof(thisUserLogPwd),(uint8_t *)thisUserLogPwd);
			Arc4EncryptNeedStrCiphertext(arc4Key,sizeof(arc4Key),(uint8_t *)thisUserLogPwd,
									                 sizeof(thisUserLogPwd),(uint8_t *)thisUserLogPwdArc4En);
			/****************对管理员用户的开门密码密文进行解密**************/
//			Arc4DecryptFromHexPainText( arc4Key,sizeof(arc4Key),(uint8_t *)thisUserLogPwdArc4En,
//										      sizeof(thisUserLogPwdArc4En),(uint8_t *)thisUserLogPwd);
//			Arc4Decrypt(arc4Key,sizeof(arc4Key),(uint8_t *)thisUserLogPwd,
//					                     sizeof(thisUserLogPwd),(uint8_t *)thisUserLogPwd);
		if(index>=0&&index<COMBASEAD)
		{
			cJSON_AddStringToObject(img,"userType","1");
			cJSON_AddStringToObject(img,"UserOpenPwdValues",thisUserLogPwd);					
		}
		else if(index>=COMBASEAD&&index<TEMBASEAD)
		{
			cJSON_AddStringToObject(img,"userType","2");
			cJSON_AddStringToObject(img,"UserOpenPwdValues",thisUserLogPwd);					
		}
		else if(index>=TEMBASEAD&&index<TEMENDAD)
		{
			cJSON_AddStringToObject(img,"userType","3");
			cJSON_AddStringToObject(img,"UserOpenPwdValues",thisUserLogPwd);					  		
		}
	}
	else if(UserLogVerifyStatus==FAULT)
	{
		cJSON_AddStringToObject(img,"userType","0");
		cJSON_AddStringToObject(img,"UserOpenPwdValues","0");
	}
	cJSON_AddStringToObject(root,"flag","4");
	out=cJSON_Print(root);	cJSON_Delete(root);	
	printf("%s\n",out);
	free(out);	
	
	return 0;
}

/*************************************************************************************
*  Function Name                    :   UserLogVerify

*  Create Date                      :    2017/03/11
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    验证用户登录信息
                                        

*  Param                            :    cJSON *object
*  Return Code                      :    
																			
*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/11 Liu Chuanjia     None           None

**************************************************************************************/
uint8_t UserLogVerify(cJSON *object)
{
		User Log={0};              //登录用户结构体信息
	  cJSON *item=NULL;
		int32_t thisUserLogStatus=0;
		
		item=cJSON_GetObjectItem(object,"UserNameValues");
	  memcpy(Log.user_name,item->valuestring,sizeof(Log.user_name));
	
	  item=cJSON_GetObjectItem(object,"UserPwdValues");
	  memcpy(Log.log_in_word,item->valuestring,sizeof(Log.log_in_word));
		
		/***************寻找当前需要登录的用户的信息并验证****************/
		thisUserLogStatus=FindThisUserInAllUserBase_Name_LogPwd(Log.user_name,Log.log_in_word);
		/***************上传当前用户的登录验证结果****************/
		SendThisUserLogResult(thisUserLogStatus);
		return 0;
}
/*************************************************************************************
*  Function Name                    :    strCopy

*  Create Date                      :    2017/1/05
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    将一个数组复制到同等大小的另一个数组中


*  Param                            :    Source,Destination,len

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            17.1.05    Liu Chuanjia     None           None

**************************************************************************************/

uint8_t strCopy(unsigned char const* Source,unsigned char * const Destination,const int len)
{
	int i=0;
	while(i++<len)
	{
		*(Destination+i-1)=*(Source+i-1);
	}
	return 0;
}

/*************************************************************************************
*  Function Name                    :   SendResetThisUserResetPwdResult

*  Create Date                      :    2017/03/11
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    上传当前重新设置用户的当前用的开门密码的结果
                                        

*  Param                            :    const int32_t resetThisUserOpenPwdStatus,
	                                       const char *newOpenPwd,const int32_t newOpenPwdLen 
*  Return Code                      :    None
																			
*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/11 Liu Chuanjia     None           None

**************************************************************************************/
uint8_t SendResetThisUserResetOpenPwdResult(const int32_t resetThisUserOpenPwdStatus,
	                                          const char *newOpenPwd,const int32_t newOpenPwdLen)
{
		cJSON *root,*img; 
	  char *out;
		int32_t index=resetThisUserOpenPwdStatus;
		int32_t openPwdLen=0;
		char *newOpenPwdBuf=NULL;
		root=cJSON_CreateObject();             //创建验证对象结构体
		cJSON_AddItemToObject(root,"data",img=cJSON_CreateObject());	
	
    if(resetThisUserOpenPwdStatus!=FAULT)		
		{
			openPwdLen= newOpenPwdLen;
			newOpenPwdBuf=(char *)malloc(openPwdLen);
			strCopy((unsigned char *)newOpenPwd,(unsigned char *)newOpenPwdBuf,sizeof(newOpenPwdBuf));
			AT24C128_Save(index,newOpenPwdBuf,OPENPWDLEN);  //保存当前用户新设置的开门密码密码	
		  cJSON_AddStringToObject(img,"changePwdResult","true");
		  cJSON_AddStringToObject(img,"UserOpenPwdValues",newOpenPwdBuf);
		}		
		else if(resetThisUserOpenPwdStatus==FAULT)         //没有找到当前需要设置新的开门密码的用户，返回错误
		{
      cJSON_AddStringToObject(img,"changePwdResult","false");
		}			

		cJSON_AddStringToObject(root,"flag","16");	
	  out=cJSON_Print(root);	cJSON_Delete(root);	
		printf("%s\n",out);free(out);
		if(resetThisUserOpenPwdStatus!=FAULT) free(newOpenPwdBuf);
		return 0;
}

/*************************************************************************************
*  Function Name                    :   resetThisUserOpenPwd

*  Create Date                      :    2017/03/11
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    重新设置当前用户的开门密码
                                        

*  Param                            :    cJSON *object
*  Return Code                      :    
																			
*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/11 Liu Chuanjia     None           None

**************************************************************************************/
uint8_t ResetThisUserOpenPwd(cJSON *object)
{
		User resetUserOpenPwd={0};              //登录用户结构体信息
	  cJSON *item=NULL;
		int32_t findThisUserStatus=0;
		char newOpenPwdValues[OPENPWDLEN]={0};		
		
		item=cJSON_GetObjectItem(object,"UserNameValues");
	  memcpy(resetUserOpenPwd.user_name,item->valuestring,sizeof(resetUserOpenPwd.user_name));
	
	  item=cJSON_GetObjectItem(object,"UserPwdValues");
	  memcpy(resetUserOpenPwd.log_in_word,item->valuestring,sizeof(resetUserOpenPwd.log_in_word));
		
		item=cJSON_GetObjectItem(object,"UserOpenPwdValues");//提取新的开门密码
		memcpy(newOpenPwdValues,item->valuestring,sizeof(newOpenPwdValues));
		
		/***************寻找当前需要登录的用户的信息并验证****************/
		findThisUserStatus=FindThisUserInAllUserBase_Name_LogPwd(resetUserOpenPwd.user_name,resetUserOpenPwd.log_in_word);
		SendResetThisUserResetOpenPwdResult(findThisUserStatus,newOpenPwdValues,sizeof(newOpenPwdValues));
		
		return 0;	
}


/*************************************************************************************
*  Function Name                    :   SendResetThisUserResetLogPwdResult

*  Create Date                      :    2017/03/11
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    上传当前重新设置用户的当前用的登录密码的结果
                                        

*  Param                            :    const int32_t resetThisUserLogPwdStatus,
	                                       const char *newLogPwd,const int32_t newLogPwdLen 
*  Return Code                      :    None
																			
*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/11 Liu Chuanjia     None           None

**************************************************************************************/
uint8_t SendResetThisUserResetLogPwdResult(const int32_t resetThisUserLogPwdStatus,
	                                          const char *newLogPwd,const int32_t newLogPwdLen)
{
		cJSON *root,*img; 
	  char *out;
		int32_t index=resetThisUserLogPwdStatus;
		int32_t logPwdLen=0;
		char *newLogPwdBuf=NULL;
		root=cJSON_CreateObject();             //创建验证对象结构体
		cJSON_AddItemToObject(root,"data",img=cJSON_CreateObject());	
	
    if(resetThisUserLogPwdStatus!=FAULT)		
		{
			index+= (NAMELEN+LOGPWDLEN);
			logPwdLen= newLogPwdLen;
			newLogPwdBuf=(char *)malloc(logPwdLen);
			strCopy((unsigned char *)newLogPwd,(unsigned char *)newLogPwdBuf,sizeof(newLogPwdBuf));
			AT24C128_Save(index,newLogPwdBuf,LOGPWDLEN);  //保存当前用户新设置的开门密码密码	
		  cJSON_AddStringToObject(img,"changePwdResult","true");
//		  cJSON_AddStringToObject(img,"UserPwdValues",newLogPwdBuf);
		}		
		else if(resetThisUserLogPwdStatus==FAULT)         //没有找到当前需要设置新的开门密码的用户，返回错误
		{
      cJSON_AddStringToObject(img,"changePwdResult","false");
		}			

		cJSON_AddStringToObject(root,"flag","16");	
	  out=cJSON_Print(root);	cJSON_Delete(root);	
		printf("%s\n",out);free(out);
		if(resetThisUserLogPwdStatus!=FAULT) free(newLogPwdBuf);
		return 0;
}

/*************************************************************************************
*  Function Name                    :   resetThisUserLogPwd

*  Create Date                      :    2017/03/11
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    重新设置当前用户的登录密码
                                        

*  Param                            :    cJSON *object
*  Return Code                      :    
																			
*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/11 Liu Chuanjia     None           None

**************************************************************************************/
uint8_t resetThisUserLogPwd(cJSON *object)
{
		User resetUserLogPwd={0};              //登录用户结构体信息
	  cJSON *item=NULL;
		int32_t findThisUserStatus=0;
		char newLogPwdValues[OPENPWDLEN]={0};		
		
		item=cJSON_GetObjectItem(object,"UserNameValues");
	  memcpy(resetUserLogPwd.user_name,item->valuestring,sizeof(resetUserLogPwd.user_name));
	
	  item=cJSON_GetObjectItem(object,"UserPwdValues");
	  memcpy(resetUserLogPwd.log_in_word,item->valuestring,sizeof(resetUserLogPwd.log_in_word));
		
		item=cJSON_GetObjectItem(object,"NewPwdValues");//提取新的开门密码
		memcpy(newLogPwdValues,item->valuestring,sizeof(newLogPwdValues));
		
		/***************寻找当前需要登录的用户的信息并验证****************/
		findThisUserStatus=FindThisUserInAllUserBase_Name_LogPwd(resetUserLogPwd.user_name,resetUserLogPwd.log_in_word);
		SendResetThisUserResetLogPwdResult(findThisUserStatus,newLogPwdValues,sizeof(newLogPwdValues));
		
		return 0;	
}

