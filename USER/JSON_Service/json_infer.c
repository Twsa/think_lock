#include "JSON_Service/json_infer.h"

uint8_t ClearRecord(void)
{
  char clearBuffer[3]={0};
	AT24C128_Save(INDEXOPENUSER,clearBuffer,sizeof(clearBuffer));			//������տ�����¼����

  return 0;
}

uint8_t ClearUser(void)
{
	  char clearBuffer[3]={0};
		AT24C128_Save(INDEXCOMUSERADD,clearBuffer,sizeof(clearBuffer));   //�����ͨ�û�
    AT24C128_Save(INDEXTEMUSERADD,clearBuffer,sizeof(clearBuffer));   //�����ʱ�û�
		
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
		 case RSAHANDLER:RSA_Handler(object);cJSON_Delete(json);break;//���ܹ�Կ������	
		
		 case USELOG:
			 UserLogVerify(object);cJSON_Delete(json);SoftReset();break; //�û���¼��֤
		 case OPENDOOR:
			 JSON_Open(object);cJSON_Delete(json);break;       //����
		 case CHECKUSERLIST:
			 cJSON_Delete(json);Check_Listing();break;         //��ȡ�����û��б�
		 case SETCOMUSER:
			 Com_set(object);cJSON_Delete(json);break;         //��ͨ�û����
		 case DETUSER:
			 User_det(object);cJSON_Delete(json);break;       //ɾ���û���Ϣ ����Աɾ����ͨ�û���ɾ����ʱ�û�
		 case CHECKOPENRECORD:Opendata_A(object);cJSON_Delete(json);break;        //����Ա����鿴������¼
		 case RESETOPENPWD:
			  ResetThisUserOpenPwd(object);cJSON_Delete(json);break;//�޸Ŀ�������
		 case SETTEMUSER:
			 Tem_set(object);cJSON_Delete(json);break;     //��ʱ�û����
		 case SETADMINUSER:
			 Admin_set(object);cJSON_Delete(json);break;   //����Ա���
		 case RESETLOGPWD:
			  resetThisUserLogPwd(object);cJSON_Delete(json);break;//�����û��ĵ�¼����
		 case CHANGETEMCOUNT:
			 Change_temcount(object);cJSON_Delete(json);break; //�޸���ʱ�û��Ŀ�������
	//	 case 26:RC4_Key(void)                                      //����RSA��֤�ɹ���Ϣ��Ȼ����RC4��Կ  
//	 case 50:
	  case 23:ClearRecord();break;
	  case 24:ClearUser();break;
		case DELETERECOREFLAG:DeleteRecord(object);cJSON_Delete(json);break;       //ɾ��������¼
	default:break;
	}
} 

/*************************************************************************************
*  Function Name                    :    SoftReset

*  Create Date                      :    2017/01/06
*  Author/Corporation               :   
*  Description                      :    �����λ


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
  __set_FAULTMASK(1);      // �ر������ж�
  NVIC_SystemReset();      // ��λ
	return 0;
}
/*************************************************************************************
*  Function Name                    :   FindThisUserInAllUserBase_Name_LogPwd

*  Create Date                      :    2017/03/11
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ���õ�ǰ�û����û����͵�¼����Ѱ�ҵ�ǰ�û���Bsae��ַ
                                        

*  Param                            :    cJSON *object
*  Return Code                      :    ��ǰ�û���Bsae��ַ  ���û���ҵ���ǰ�û�����(uint32_t)0
																			
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
	 char Log_buf[LOGPWDLEN]={0};                  //������û����͵�¼���룬�ȴ���֤
	 char comuserAddBuf[LIMITLENOPENRCORD/10]={0};
   char temuserAddBuf[LIMITLENOPENRCORD/10]={0}; 

	 
	 int verify_name=1,verify_log=1;	 	 
	 
    index=ADMINADDRESS;
		for (;index<ADLOGAD;)
		{  
			 index+=OPENPWDLEN;
	     AT24CXX_Read(index,Name_buf,NAMELEN);//��ȡ�Ѿ�������û���
			 index+=NAMELEN;
			 AT24CXX_Read(index,Log_buf,LOGPWDLEN);//��ȡ������û���¼����
       index=(index-NAMELEN-OPENPWDLEN);		
			
			 verify_name=strcmp(Name_buf,RX_name);	
			 verify_log=strcmp(Log_buf,RX_log);
			
			 if (verify_name==0&&verify_log==0)
				 break;
				index+=USERLEN;
		}                                        //�ڹ���Ա�û���û���ҵ���ǰ�û�
		if (verify_name!=0 || verify_log!=0)
		{
			index=COMBASEAD;      //��ͨ�û���Ϣ����ַ
	    AT24CXX_Read(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf)); 
	    comuserAdd=atoi(comuserAddBuf);			
			for (;index<COMLOGAD;)
			{  
				 index+=OPENPWDLEN;
				 AT24CXX_Read(index,Name_buf,NAMELEN);//��ȡ�Ѿ�������û�?
				 index+=NAMELEN;
			   AT24CXX_Read(index,Log_buf,LOGPWDLEN);//��ȡ������û���¼����
				 index=(index-NAMELEN-OPENPWDLEN);
			   verify_name=strcmp(Name_buf,RX_name);	
			   verify_log=strcmp(Log_buf,RX_log);
				 if (verify_name==0&&verify_log==0)
					 break;
					index+=USERLEN;
			}                                       //����ͨ�û���û���ҵ���ǰ�û�
		}
		if (verify_name!=0 || verify_log!=0)
		{
			index=TEMBASEAD;
			AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf)); 
	    temuserAdd=atoi(temuserAddBuf);				     
			for (;index<TEMLOGAD;)
			{  		
				 index+=OPENPWDLEN;
				 AT24CXX_Read(index,Name_buf,NAMELEN);//��ȡ�Ѿ�������û���
				 index+=NAMELEN;
				 AT24CXX_Read(index,Log_buf,LOGPWDLEN);//��ȡ������û���¼����		 
				 index=(index-NAMELEN-OPENPWDLEN);
				 verify_name=strcmp(Name_buf,RX_name);	
				 verify_log=strcmp(Log_buf,RX_log);
				 if (verify_name==0&&verify_log==0)
					 break;
				 index+=(USERLEN+TEMCOUNTLEN);
			}				
		}		                                    //����ʱ�û���û���ҵ���ǰ�û�
//	  if (verify_name!=0 || verify_log!=0)
//		{			
//			  root=cJSON_CreateObject();             //������֤����ṹ��
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
		else return index;              //ָʾ��֤��ǰ�û��ĵ�¼��֤�ɹ���״̬��
		                                //Ҳ�Ƿ��ص�ǰ�û���Base��ַ
}
/*************************************************************************************
*  Function Name                    :    SendThisUserLogResult

*  Create Date                      :    2017/03/11
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ���͵�ǰ�û��ĵ�¼���
                                        

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
	
	root=cJSON_CreateObject();             //������֤����ṹ��
  cJSON_AddItemToObject(root, "data", img=cJSON_CreateObject());
	/************����ʱʹ�õ�arc4Key*************/
//	char thisUserLogPwd[3]="123";
  uint8_t arc4Key[8]="12345678";
	/************����ʱʹ�õ�arc4Key*************/

	if(UserLogVerifyStatus != FAULT)                  //��ǰ�û���¼��Ϣ��֤�ɹ�
	{	
		AT24CXX_Read(index,thisUserLogPwd,OPENPWDLEN);   //��ȡ��ǰ�û��ĵĿ�������
			/****************�Թ���Ա�û��Ŀ���������м���**************/
//			Arc4Encrypt(arc4Key,sizeof(arc4Key),(uint8_t *)thisUserLogPwd,
//									sizeof(thisUserLogPwd),(uint8_t *)thisUserLogPwd);
			Arc4EncryptNeedStrCiphertext(arc4Key,sizeof(arc4Key),(uint8_t *)thisUserLogPwd,
									                 sizeof(thisUserLogPwd),(uint8_t *)thisUserLogPwdArc4En);
			/****************�Թ���Ա�û��Ŀ����������Ľ��н���**************/
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
*  Description                      :    ��֤�û���¼��Ϣ
                                        

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
		User Log={0};              //��¼�û��ṹ����Ϣ
	  cJSON *item=NULL;
		int32_t thisUserLogStatus=0;
		
		item=cJSON_GetObjectItem(object,"UserNameValues");
	  memcpy(Log.user_name,item->valuestring,sizeof(Log.user_name));
	
	  item=cJSON_GetObjectItem(object,"UserPwdValues");
	  memcpy(Log.log_in_word,item->valuestring,sizeof(Log.log_in_word));
		
		/***************Ѱ�ҵ�ǰ��Ҫ��¼���û�����Ϣ����֤****************/
		thisUserLogStatus=FindThisUserInAllUserBase_Name_LogPwd(Log.user_name,Log.log_in_word);
		/***************�ϴ���ǰ�û��ĵ�¼��֤���****************/
		SendThisUserLogResult(thisUserLogStatus);
		return 0;
}
/*************************************************************************************
*  Function Name                    :    strCopy

*  Create Date                      :    2017/1/05
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ��һ�����鸴�Ƶ�ͬ�ȴ�С����һ��������


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
*  Description                      :    �ϴ���ǰ���������û��ĵ�ǰ�õĿ�������Ľ��
                                        

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
		root=cJSON_CreateObject();             //������֤����ṹ��
		cJSON_AddItemToObject(root,"data",img=cJSON_CreateObject());	
	
    if(resetThisUserOpenPwdStatus!=FAULT)		
		{
			openPwdLen= newOpenPwdLen;
			newOpenPwdBuf=(char *)malloc(openPwdLen);
			strCopy((unsigned char *)newOpenPwd,(unsigned char *)newOpenPwdBuf,sizeof(newOpenPwdBuf));
			AT24C128_Save(index,newOpenPwdBuf,OPENPWDLEN);  //���浱ǰ�û������õĿ�����������	
		  cJSON_AddStringToObject(img,"changePwdResult","true");
		  cJSON_AddStringToObject(img,"UserOpenPwdValues",newOpenPwdBuf);
		}		
		else if(resetThisUserOpenPwdStatus==FAULT)         //û���ҵ���ǰ��Ҫ�����µĿ���������û������ش���
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
*  Description                      :    �������õ�ǰ�û��Ŀ�������
                                        

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
		User resetUserOpenPwd={0};              //��¼�û��ṹ����Ϣ
	  cJSON *item=NULL;
		int32_t findThisUserStatus=0;
		char newOpenPwdValues[OPENPWDLEN]={0};		
		
		item=cJSON_GetObjectItem(object,"UserNameValues");
	  memcpy(resetUserOpenPwd.user_name,item->valuestring,sizeof(resetUserOpenPwd.user_name));
	
	  item=cJSON_GetObjectItem(object,"UserPwdValues");
	  memcpy(resetUserOpenPwd.log_in_word,item->valuestring,sizeof(resetUserOpenPwd.log_in_word));
		
		item=cJSON_GetObjectItem(object,"UserOpenPwdValues");//��ȡ�µĿ�������
		memcpy(newOpenPwdValues,item->valuestring,sizeof(newOpenPwdValues));
		
		/***************Ѱ�ҵ�ǰ��Ҫ��¼���û�����Ϣ����֤****************/
		findThisUserStatus=FindThisUserInAllUserBase_Name_LogPwd(resetUserOpenPwd.user_name,resetUserOpenPwd.log_in_word);
		SendResetThisUserResetOpenPwdResult(findThisUserStatus,newOpenPwdValues,sizeof(newOpenPwdValues));
		
		return 0;	
}


/*************************************************************************************
*  Function Name                    :   SendResetThisUserResetLogPwdResult

*  Create Date                      :    2017/03/11
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    �ϴ���ǰ���������û��ĵ�ǰ�õĵ�¼����Ľ��
                                        

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
		root=cJSON_CreateObject();             //������֤����ṹ��
		cJSON_AddItemToObject(root,"data",img=cJSON_CreateObject());	
	
    if(resetThisUserLogPwdStatus!=FAULT)		
		{
			index+= (NAMELEN+LOGPWDLEN);
			logPwdLen= newLogPwdLen;
			newLogPwdBuf=(char *)malloc(logPwdLen);
			strCopy((unsigned char *)newLogPwd,(unsigned char *)newLogPwdBuf,sizeof(newLogPwdBuf));
			AT24C128_Save(index,newLogPwdBuf,LOGPWDLEN);  //���浱ǰ�û������õĿ�����������	
		  cJSON_AddStringToObject(img,"changePwdResult","true");
//		  cJSON_AddStringToObject(img,"UserPwdValues",newLogPwdBuf);
		}		
		else if(resetThisUserLogPwdStatus==FAULT)         //û���ҵ���ǰ��Ҫ�����µĿ���������û������ش���
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
*  Description                      :    �������õ�ǰ�û��ĵ�¼����
                                        

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
		User resetUserLogPwd={0};              //��¼�û��ṹ����Ϣ
	  cJSON *item=NULL;
		int32_t findThisUserStatus=0;
		char newLogPwdValues[OPENPWDLEN]={0};		
		
		item=cJSON_GetObjectItem(object,"UserNameValues");
	  memcpy(resetUserLogPwd.user_name,item->valuestring,sizeof(resetUserLogPwd.user_name));
	
	  item=cJSON_GetObjectItem(object,"UserPwdValues");
	  memcpy(resetUserLogPwd.log_in_word,item->valuestring,sizeof(resetUserLogPwd.log_in_word));
		
		item=cJSON_GetObjectItem(object,"NewPwdValues");//��ȡ�µĿ�������
		memcpy(newLogPwdValues,item->valuestring,sizeof(newLogPwdValues));
		
		/***************Ѱ�ҵ�ǰ��Ҫ��¼���û�����Ϣ����֤****************/
		findThisUserStatus=FindThisUserInAllUserBase_Name_LogPwd(resetUserLogPwd.user_name,resetUserLogPwd.log_in_word);
		SendResetThisUserResetLogPwdResult(findThisUserStatus,newLogPwdValues,sizeof(newLogPwdValues));
		
		return 0;	
}

