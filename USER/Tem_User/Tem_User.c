#include "Tem_User/Tem_User.h"

/*
data:2016/10/18
name:Tem_set
function:��ʱ�û�ע��
developer��liuchuanjia
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
		
		item=cJSON_GetObjectItem(object,"UserNameValues");   //��ȡ������ͨ�û����û���
	  sprintf(Temset.user_name,"%s",item->valuestring);
		
	  item=cJSON_GetObjectItem(object,"UserPwdValues");    //��ȡ������ͨ�û��ĵ�¼����
	  sprintf(Temset.log_in_word,"%s",item->valuestring);
		
	  item=cJSON_GetObjectItem(object,"UserOpenPwdValues");//��ȡ������ͨ�û��Ŀ�������
	  sprintf(Temset.open_word,"%s",item->valuestring);
		
	  item=cJSON_GetObjectItem(object,"UserOpenCountValues");//��ȡ������ͨ�û��Ŀ�������
	  sprintf(count_buffer,"%s",item->valuestring);
		
		usernameIndex+=NAMELEN;                              //��Ҫ��֤�û�����ַ����
		indexTemuser = FindThisUserBase_Name(Temset.user_name);
    
		if(indexTemuser==FAULT)
		{
			status=SUCCESS;
		}
		else if(indexTemuser!=FAULT)
		{
//			indexTemuserName = indexTemuser+OPENPWDLEN;            //Ѱ�ҵ�ǰ���û��Ƿ���ڣ�����Ѿ����ھͲ�����������
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
		
    /********************��ȡ��ǰ��ʱ�û����õĸ���********************/
		AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf));		
		temuserAdd=atoi(temuserAddBuf);
		if(temuserAdd==0)
		{
			AT24C128_Save(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf));
		}
	  /********************������ʱ�û��������ж�********************/
		if(temuserAdd/(USERLEN+TEMCOUNTLEN)>TEMUSERSETLIMIT)
		{
			status=FAULT;                  //����Ԥ���õ��û����ޣ������������ʱ�û�
		}
//		printf("Tem_set:temuserAdd:%d\n",temuserAdd);		
      
		if(status==SUCCESS)
		{
			temuserAdd+=(USERLEN+TEMCOUNTLEN);//��ʱ�û���������
			temuserNum=(temuserAdd/(USERLEN+TEMCOUNTLEN));
			sprintf(temuserAddBuf,"%d",temuserAdd);
			AT24C128_Save(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf));
			
			AT24C128_Save(TEMOPENPWDAD,Temset.open_word,OPENPWDLEN);	
			AT24C128_Save(TEMNAMEAD,Temset.user_name,NAMELEN);//�����û���
			AT24C128_Save(TEMLOGAD,Temset.log_in_word,LOGPWDLEN);//�����¼����
			AT24C128_Save(LOGUSERNAME,Temset.user_name,sizeof(Temset.user_name));	
			AT24C128_Save(TEMCOUNTAD,count_buffer,sizeof(count_buffer));
		}	
		
    Userset_result(status);//�û���ӳɹ�������Ϣ
}

///*************************************************************************************
//*  Function Name                    :   FindThisTemUser

//*  Create Date                      :    2017/03/09
//*  Author/Corporation               :    Liu Chuanjia
//*  Description                      :    Ѱ�ҵ�ǰ����ʱ�û�
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
////	 char Log_buf[LOGPWDLEN]={0};//������û����͵�¼���룬�ȴ���֤

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
//			 AT24CXX_Read(index,Name_buf,NAMELEN);//��ȡ�Ѿ�������û���
////			 printf("Name_buf:%s\r\n",Name_buf);
////			 index+=NAMELEN;
////			 AT24CXX_Read(index,Log_buf,LOGPWDLEN);//��ȡ������û���¼����		
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

////			printf("index:%d\r\n",index);//�û���¼��ַ������Ϣ
////		  AT24CXX_Read(index,Openwd_Send,OPENPWDLEN);//��ȡ��������
////		printf("index:%s\r\n",Openwd_Send);
//		else
//		return index;
//}
/*************************************************************************************
*  Function Name                    :   FindThisTemUser

*  Create Date                      :    2017/03/09
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    Ѱ�ҵ�ǰ����ʱ�û�
                                        

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
	 char Log_buf[LOGPWDLEN]={0};//������û����͵�¼���룬�ȴ���֤

   char temuserAddBuf[LIMITLENOPENRCORD/10]={0}; 
	 
	 int verify_name=1,verify_log=1;	 	 
	 
		index=TEMBASEAD;
		AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf)); 
		temuserAdd=atoi(temuserAddBuf);				
		for (;index<TEMLOGAD;)
		{  		
			 index+=OPENPWDLEN;
			 AT24CXX_Read(index,Name_buf,NAMELEN);//��ȡ�Ѿ�������û���
//			 printf("Name_buf:%s\r\n",Name_buf);
			 index+=NAMELEN;
			 AT24CXX_Read(index,Log_buf,LOGPWDLEN);//��ȡ������û���¼����		
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
