//��֤�û����ͣ������û���Ϣ��֤��ִ�п������Ȼ�����û���Ϣ
//Ledָʾ��������Ƿ����
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
//  GPIO_SetBits(GPIOB,GPIO_Pin_1);	 //������ʱ
//	delay_ms(500);
//	GPIO_ResetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_0);
  GPIO_ResetBits(GPIOB, GPIO_Pin_1);
  GPIO_SetBits(GPIOB,GPIO_Pin_0);	 //������ʱ
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
//  GPIO_SetBits(GPIOB,GPIO_Pin_0);	 //������ʱ
//	delay_ms(500);
//  GPIO_ResetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_0);	

  GPIO_ResetBits(GPIOB, GPIO_Pin_0);
  GPIO_SetBits(GPIOB,GPIO_Pin_1);	 //������ʱ
	delay_ms(500);
	GPIO_ResetBits(GPIOB, GPIO_Pin_1 | GPIO_Pin_0);
	Opendoor_Staus=CLOSED;
	TIM_Cmd(TIM2, DISABLE);		
}

/*************************************************************************************
*  Function Name                    :   static myStringCopysomelen

*  Create Date                      :    2017/01/07
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ��һ�����ȣ��ַ�������
                                        

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
*  Description                      :    ����
                                        

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
		char openuserIndexBuf[LIMITLENOPENRCORD/10]={0};//�����û���ַ����
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
				Open_door();//��֤�ɹ�������������
				Menci_config();	
				
//			  printf("USERNAMEOPENRE:%d\r\n",USERNAMEOPENRE);
				AT24C128_Save(USERNAMEOPENRE,Open.user_name,NAMELEN);//����Ŷ�ɹ��������û���Ϣ֮�û�������
				AT24C128_Save(TIMEOPENRE,Open.time,TIMELEN);               //�����û����ŵ�ʱ��
			
				openuserIndex+=OPENRECORDLEN;         //�����û���Ϣ��ַ����
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
				indexTemUser=FindThisTemUserBase_Name_Logpwd(Open.user_name,Open.log_in_word);		 //	��ѯ��ǰ����ʱ�û�����ȡ��ǰ��ʱ�û��Ŀ��Ŵ�����Ϣ	
				if(indexTemUser!=FAULT)           //����ҵ��˵�ǰ��ʱ�û���ȡ��ǰ����Ŀ�������
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
				}                                 //���û���ҵ���ǰ�û�Ĭ�Ͽ�������Ϊ-1
				          
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
          
					/******����ǰ��ʣ��Ŀ������������ڵ�ǰ�û��ĵ�ַ***************/
				  AT24C128_Save(indexTemCount,temCount_buf,TEMCOUNTLEN);
					
						if(openuserIndex==0)
						{
							AT24C128_Save(INDEXOPENUSER,openuserIndexBuf,sizeof(openuserIndexBuf));			
						}
						
					cJSON_AddStringToObject(flag,"openResult","true");
					cJSON_AddStringToObject(root,"flag","6");
					out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);
					free(out);						
					Open_door();//��֤�ɹ�������������
			    Menci_config();	
						
					AT24C128_Save(USERNAMEOPENRE,Open.user_name,NAMELEN);//����Ŷ�ɹ��������û���Ϣ֮�û�������				
					AT24C128_Save(TIMEOPENRE,Open.time,TIMELEN);               //�����û����ŵ�ʱ��
						
					openuserIndex+=OPENRECORDLEN;         //�����û���Ϣ��ַ����
					sprintf(openuserIndexBuf,"%d",openuserIndex);
					AT24C128_Save(INDEXOPENUSER,openuserIndexBuf,sizeof(openuserIndexBuf));			
				}		
			}	
       return 0;			
}			
/*
data:2016/10/23
name:Menci_close
function:�Ŵſ���
developer��liuchuanjia
*/

void Menci_config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTAʱ��
	 	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA
}
