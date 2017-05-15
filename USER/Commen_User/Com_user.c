#include "Commen_User/Com_user.h"

/*
data:2016/10/18
name:Com_set
function:��ͨ�û�����
developer��liuchuanjia
*/
//extern long Open_name_openpwd;
extern uint32_t usernameIndex;
extern uint32_t comuserAdd;//�û���������
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
		
		item=cJSON_GetObjectItem(object,"UserNameValues");//��ȡ������ͨ�û����û���
	  sprintf(Comset.user_name,"%s",item->valuestring);

	  item=cJSON_GetObjectItem(object,"UserPwdValues");//��ȡ������ͨ�û��ĵ�¼����
	  sprintf(Comset.log_in_word,"%s",item->valuestring);
		
	  item=cJSON_GetObjectItem(object,"UserOpenPwdValues");//��ȡ������ͨ�û��Ŀ�������
	  sprintf(Comset.open_word,"%s",item->valuestring);
		usernameIndex+=NAMELEN;//��Ҫ��֤�û�����ַ����
		 
    indexComuser = FindThisUserBase_Name(Comset.user_name);

		if(indexComuser==FAULT)
		{
			status=SUCCESS;
		}
		else if(indexComuser!=FAULT)
		{
//			indexComuserName = indexComuser+OPENPWDLEN;            //Ѱ�ҵ�ǰ���û��Ƿ���ڣ�����Ѿ����ھͲ�����������
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
		
		/********************��ȡ��ǰ��ͨ�û����õĸ���********************/
		AT24CXX_Read(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf)); 
		comuserAdd=atoi(comuserAddBuf);
		if(comuserAdd==0)
		{
			AT24C128_Save(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf));
		}	 
	  /********************������ͨ�û��������ж�********************/
//		printf("comuserAdd/USERLEN:%d\n",comuserAdd/USERLEN);	
		if(((comuserAdd/USERLEN)+1)>COMUSERSETLIMIT)
		{
			status=FAULT;                  //����Ԥ���õ��û����ޣ������������ʱ�û�
		}
		
		if(status==SUCCESS)
		{ 
	//		printf("Com_set:comuserAdd:%d\n",comuserAdd);		
			comuserAdd+=USERLEN;
			comuserNum=(comuserAdd/USERLEN); //�û��ĸ���	
			sprintf(comuserAddBuf,"%d",comuserAdd);
			AT24C128_Save(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf));

			AT24C128_Save(COMOPENPWDAD,Comset.open_word,OPENPWDLEN);
			AT24C128_Save(COMNAMEAD,Comset.user_name,NAMELEN);//�����û���
			AT24C128_Save(COMLOGAD,Comset.log_in_word,LOGPWDLEN);//�����¼����	
			AT24C128_Save(LOGUSERNAME,Comset.user_name,NAMELEN);//b���濪���û���
		}	
	 Userset_result(status);//�û���ӳɹ�������Ϣ
}


/*************************************************************************************
*  Function Name                    :   FindThisComUser

*  Create Date                      :    2017/03/11
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ���õ�ǰ���û����͵�ǰ�û��Ŀ�������
                                         Ѱ�ҵ�ǰ����ͨ�û�
                                        

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
	 char Log_buf[LOGPWDLEN]={0};//������û����͵�¼���룬�ȴ���֤
	 char comuserAddBuf[LIMITLENOPENRCORD/10]={0}; 
	 
	 int verify_name=1,verify_log=1;	 	 
	 
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
	 }
	
	  if (verify_name!=0 || verify_log!=0)
		{			
       return FAULT;
		}
		else
		return index;
}
