#include "Administrator/Admin.h"


uint32_t comuserAdd=0;//�û���������
uint32_t temuserAdd=0;
uint32_t comuserNum=0;//��ǰ��ͨ�û�����
uint32_t temuserNum=0;//��ǰ��ʱ�û��ĸ���
extern char STM32_ID[24];//��Ƭ����ID

/*************************************************************************************
*  Function Name                    :    Admin_set

*  Create Date                      :    2016/10/18
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ����Ա�û�����


*  Param                            :    None

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016.10.18  Liu Chuanjia     None           None

**************************************************************************************/
uint8_t Admin_set(cJSON *object)         
{
	  User Admin={0};
	  cJSON *item=NULL,*root=NULL,*flag=NULL; 
	  char *out=NULL;
		
		item=cJSON_GetObjectItem(object,"UserNameValues");
	  sprintf(Admin.user_name,"%s",item->valuestring);

		item=cJSON_GetObjectItem(object,"UserPwdValues");
	  sprintf(Admin.log_in_word,"%s",item->valuestring);
		
		item=cJSON_GetObjectItem(object,"UserOpenPwdValues");
	  sprintf(Admin.open_word,"%s",item->valuestring);
		
	  item=cJSON_GetObjectItem(object,"IMEI");
	  sprintf(Admin.IMEI,"%s",item->valuestring);
		
		AT24C128_Save(ADMINOPENPWDAD,Admin.open_word,OPENPWDLEN);//���濪������		
    AT24C128_Save(ADNAMEAD,Admin.user_name,NAMELEN);//�����û���
		AT24C128_Save(ADLOGAD,Admin.log_in_word,LOGPWDLEN);//�����¼����
    AT24C128_Save(ADIMEIAD,Admin.IMEI,IMEILEN);//������ʱ�ʶ
		AT24C128_Save(LOGNAMEBASEAD,Admin.user_name,sizeof(Admin.user_name));
		
    Get_ChipID(); //��ȡ��Ƭ��ID
    AT24C128_ID();//����ID
		root=cJSON_CreateObject();             //��������ṹ��
	  cJSON_AddItemToObject(root, "data", flag=cJSON_CreateObject());	
	  cJSON_AddStringToObject(flag,"STM32_ID",STM32_ID);
    cJSON_AddStringToObject(flag,"addResult","true");
	  cJSON_AddStringToObject(root,"flag","19");
	  out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);
	  free(out);
		return 0;
}

/*************************************************************************************
*  Function Name                    :    Check_Listing

*  Create Date                      :    2016/10/31
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    �����û��󷵻ص���Ϣ


*  Param                            :    None

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016.10.31  Liu Chuanjia     None           None

**************************************************************************************/
uint8_t Userset_result(uint8_t status)
{
	cJSON *flag,*root;
	char *out;
	root=cJSON_CreateObject();            //��������ṹ��
	cJSON_AddItemToObject(root, "data", flag=cJSON_CreateObject());	
	
	if (status== SUCCESS)
	{
			cJSON_AddStringToObject(flag,"addResult","true");//����û��ɹ�
	}
	else if(status== FAULT)
	{
		cJSON_AddStringToObject(flag,"addResult","false");//����û�ʧ��
	}
	cJSON_AddStringToObject(root,"flag","10");
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);
	free(out);
	return 0;
}
/*************************************************************************************
*  Function Name                    :   static myStringCompare

*  Create Date                      :    2017/01/05
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ��һ�����ȣ��ַ����Ƚ�
                                        

*  Param                            :    const char* Source,const char* Destination,const uint8_t lenNeedtoCompare

*  Return Code                      :    int8_t equal:0 С�ڣ�-1�����ڣ�1
																			
*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017.1.05  Liu Chuanjia     None           None

**************************************************************************************/
static int8_t myStringCompare(const char* Source,const char* Destination,const uint8_t lenNeedtoCompare)
{
	uint8_t lenNeedtoCompareTem=0;lenNeedtoCompareTem=lenNeedtoCompare;
	 
	for(lenNeedtoCompareTem=0;lenNeedtoCompareTem<lenNeedtoCompare;lenNeedtoCompareTem++)
	{
		if(*(Source+lenNeedtoCompareTem)==*(Destination+lenNeedtoCompareTem))
     continue;
		else if(*(Source+lenNeedtoCompareTem)<*(Destination+lenNeedtoCompareTem))
			return -1;
		else if(*(Source+lenNeedtoCompareTem)>*(Destination+lenNeedtoCompareTem))
			return 1;
	}
	return 0;
}

/*************************************************************************************
*  Function Name                    :    OpenrecordTimequantum

*  Create Date                      :    2017/01/05
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ��ʱ����в�ѯ������¼
                                        

*  Param                            :    

*  Return Code                      :    openIndex
																			
*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016.10.29  Liu Chuanjia     None           None

**************************************************************************************/
static openIndex OpenrecordTimequantum(const char* const firstDay,const char* const lastDay)
{
	openIndex openRecordQuantum={0};
	
	int8_t checktimeStatus=0;
	char opentimeBuf[TIMELEN]={0};
	char index_opentimeQuantum[3]={0};
	int8_t assertCase=0;
	
	openRecordQuantum.index_time=(OPENRECORDBASEAD+NAMELEN);
	
	AT24CXX_Read(INDEXOPENUSER,index_opentimeQuantum,sizeof(index_opentimeQuantum));
  openuserIndex=atoi(index_opentimeQuantum);                   
	
	AT24CXX_Read(OPENRECORDBASEAD+NAMELEN,opentimeBuf,TIMELEN);	
  checktimeStatus=myStringCompare(firstDay,opentimeBuf,TIMEQUANTUMLEN);
//	printf("opentimeBuf:%s\n",opentimeBuf);
    
	  while(checktimeStatus>0 && openRecordQuantum.index_time<=TIMEOPENRE)
		{
			AT24CXX_Read(openRecordQuantum.index_time,opentimeBuf,TIMELEN);	
			checktimeStatus=myStringCompare(firstDay,opentimeBuf,TIMEQUANTUMLEN);
			if(checktimeStatus == 0)
				break;
			if(openRecordQuantum.index_time<TIMEOPENRE)
			openRecordQuantum.index_time+=OPENRECORDLEN;                //ָ��ָ��ǰλ����ɵ�ǰ������Ȼ��ָ����һ��λ��
			else if(openRecordQuantum.index_time==TIMEOPENRE) break;		//��ָ��ͣ��ĩβ��ַ
		}	
		openRecordQuantum.index_startTime=openRecordQuantum.index_time;
//		printf("index_firstDay:%d\n",openRecordQuantum.index_startTime);
		
		AT24CXX_Read(TIMEOPENRE-OPENRECORDLEN,opentimeBuf,TIMELEN);	
		checktimeStatus=myStringCompare(lastDay,opentimeBuf,TIMEQUANTUMLEN);
//		printf("opentimeBuf:%s\n",opentimeBuf);
//		printf("checktimeStatus:%d\n",checktimeStatus);
		
    if(checktimeStatus>0)
		{
			openRecordQuantum.index_endTime=TIMEOPENRE-OPENRECORDLEN;	
		}
		else if(checktimeStatus<=0)
		{
			checktimeStatus=0;
			while(checktimeStatus>=0 && openRecordQuantum.index_time<=TIMEOPENRE)
			{
				AT24CXX_Read(openRecordQuantum.index_time,opentimeBuf,TIMELEN);	
				checktimeStatus=myStringCompare(lastDay,opentimeBuf,TIMEQUANTUMLEN);
				if(openRecordQuantum.index_time<TIMEOPENRE)
				openRecordQuantum.index_time+=OPENRECORDLEN;
				else if(openRecordQuantum.index_time==TIMEOPENRE) 
				{
					openRecordQuantum.index_time+=OPENRECORDLEN;					
					  break;
				}
		
			}	
			 openRecordQuantum.index_endTime=openRecordQuantum.index_time-OPENRECORDLEN;	
	  }
		
		checktimeStatus = myStringCompare(firstDay,lastDay,TIMEQUANTUMLEN);
		if (checktimeStatus==0)
		{
			assertCase=1;
//			openRecordQuantum.index_startTime -=OPENRECORDLEN;
			openRecordQuantum.index_endTime +=OPENRECORDLEN; 
		}
		
		
		AT24CXX_Read( (TIMEOPENRE-OPENRECORDLEN),opentimeBuf,TIMELEN);		
	  checktimeStatus = myStringCompare(lastDay,opentimeBuf,TIMEQUANTUMLEN);
//		printf("abc:%s %d %d\n",opentimeBuf,TIMEOPENRE,checktimeStatus);
		if( assertCase == 0)
		{
			assertCase=2;
			if (checktimeStatus==0)
			{
				openRecordQuantum.index_endTime +=OPENRECORDLEN;
		  }
			else if(checktimeStatus> 0)
			{
				openRecordQuantum.index_endTime +=2 * OPENRECORDLEN;
			}
		}
//		printf("index_firstDay:%d\n",openRecordQuantum.index_startTime);
//		printf("index_lastDay:%d\n",openRecordQuantum.index_endTime);
		
//		if (detRecordHalder == RECORDCHECK)
//		{
			openRecordQuantum.index_len=(openRecordQuantum.index_endTime-openRecordQuantum.index_startTime)/OPENRECORDLEN-1;
	 
		   if (openRecordQuantum.index_len<=0)
	     openRecordQuantum.index_len=0;
//		}
//		else if(detRecordHalder == DELETERECORE)
//	  {
//			AT24CXX_Read(openRecordQuantum.index_startTime,opentimeBuf,TIMELEN);	
//       printf("index_firstDay:%s\n",opentimeBuf);
//			AT24CXX_Read(openRecordQuantum.index_endTime,opentimeBuf,TIMELEN);
//		   printf("index_lastDay:%s\n",opentimeBuf);
//		}
	 
	 return openRecordQuantum;
}

/*************************************************************************************
*  Function Name                    :    OpenDataJson

*  Create Date                      :    2016/12/31
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ����Ա����鿴������¼��Json��ʽ���͵Ĳ���


*  Param                            :    openLockHistoryNumb,thisHistoryNumb,index_name,index_time
                                         ��¼������ ����ǰJSON�����ļ�¼���������û�������������ʱ������

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016.12.31  Liu Chuanjia     None           None

**************************************************************************************/
static uint8_t OpenDataJson(const uint32_t openLockHistoryNumb,const uint32_t thisHistoryNumb,
	                           uint32_t checkRecordNumTemp,openIndex OpenrecordIndex)
{
  cJSON *root=NULL,*img=NULL,*flg=NULL; 
	char *out=NULL;
	char checkAccountNumBuf[4]={0};
	uint32_t checkJsonlen=0;
	char Username_Buf[NAMELEN]={0};
	char Time_Buf[TIMELEN]={0};
	
	root=cJSON_CreateObject();
	sprintf(checkAccountNumBuf,"%d",openLockHistoryNumb);
	cJSON_AddStringToObject(root,"openLockHistoryNumb",checkAccountNumBuf);
  sprintf(checkAccountNumBuf,"%d",thisHistoryNumb);
	cJSON_AddStringToObject(root,"thisHistoryNumb",checkAccountNumBuf);
	cJSON_AddItemToObject(root, "data", img=cJSON_CreateArray());	
	
	for(checkJsonlen=0;checkJsonlen<thisHistoryNumb;checkJsonlen++)
	{
		printf("thisHistoryNumb:%d\r\n",thisHistoryNumb);
//		printf("OpenrecordIndex.index_name:%d\n",OpenrecordIndex.index_name);
		AT24CXX_Read(OpenrecordIndex.index_name,Username_Buf,NAMELEN);
		AT24CXX_Read(OpenrecordIndex.index_time,Time_Buf,TIMELEN);
		
		cJSON_AddItemToArray(img,flg=cJSON_CreateObject());
		cJSON_AddStringToObject(flg,"UserNameValues",Username_Buf);
		cJSON_AddStringToObject(flg,"Time",Time_Buf);
		OpenrecordIndex.index_name+=OPENRECORDLEN;
		OpenrecordIndex.index_time=(OpenrecordIndex.index_name+NAMELEN);
		checkRecordNumTemp--;
	}
		cJSON_AddStringToObject(root,"flag","14");
		out=cJSON_Print(root);	cJSON_Delete(root);printf("%s\n",out);
		free(out);
	 
	 if(checkRecordNumTemp>0)
	 {
		 if(checkRecordNumTemp>=thisHistoryNumb)
		 {
		   return OpenDataJson(openLockHistoryNumb,thisHistoryNumb,
													checkRecordNumTemp,OpenrecordIndex);
		 }
		 else if(checkRecordNumTemp<thisHistoryNumb)
		 {
		  return OpenDataJson(openLockHistoryNumb,checkRecordNumTemp,
													checkRecordNumTemp,OpenrecordIndex);		 
		 }
	 }
	   return 0;
}

/*************************************************************************************
*  Function Name                    :    Opendata_A

*  Create Date                      :    2016/10/26
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ����Ա����鿴������¼


*  Param                            :    None

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016.10.26  Liu Chuanjia     None           None

**************************************************************************************/
extern uint32_t openuserIndex;

uint8_t  Opendata_A(cJSON *object)
{
	cJSON *item=NULL; 
  uint32_t checkRecordNum=0;
	uint32_t checkRecordNumTemp=0;
	
	char firstdayBuf[TIMEQUANTUMLEN]={0};
	char lastdayBuf[TIMEQUANTUMLEN]={0};
//  char *first="2017/01/07";
//  char *last="2017/01/07";
 
	openIndex openrecordIndex={0};
		
	item=cJSON_GetObjectItem(object,"firstDay");//��ȡfirstDay
	sprintf(firstdayBuf,"%s",item->valuestring);
	item=cJSON_GetObjectItem(object,"lastDay");//��ȡlastDay
	sprintf(lastdayBuf,"%s",item->valuestring);
	
	openrecordIndex=OpenrecordTimequantum(firstdayBuf,lastdayBuf);
	
//	openrecordIndex.index_name=OPENRECORDBASEAD;
//	openrecordIndex.index_time=(OPENRECORDBASEAD+NAMELEN);
	
	openrecordIndex.index_name=(openrecordIndex.index_startTime-NAMELEN);
	openrecordIndex.index_time=openrecordIndex.index_startTime;

//	AT24CXX_Read(INDEXOPENUSER,checkAccountNumBuf,sizeof(checkAccountNumBuf));
//  openuserIndex=atoi(checkAccountNumBuf);//����ȡ�����û���ַ
//	checkRecordNum=ALLRECORDNUM;
	checkRecordNum=openrecordIndex.index_len;
	checkRecordNumTemp=checkRecordNum;
	
	if(checkRecordNum<=OPENRECORDONEJSONNUM)
	{
		OpenDataJson(checkRecordNum,checkRecordNum,checkRecordNumTemp,openrecordIndex);
	}//����ܼ�¼��������ָ����Χ�ĸ���
	else if(checkRecordNum>OPENRECORDONEJSONNUM)
	{
			OpenDataJson(checkRecordNum,OPENRECORDONEJSONNUM,checkRecordNumTemp,openrecordIndex);
	}
	return 0;
}


/*************************************************************************************
*  Function Name                    :    DeleteRecord

*  Create Date                      :    2017/02/11
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ����Աɾ��������¼


*  Param                            :    None

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/02/11 Liu Chuanjia     None           None

**************************************************************************************/
uint8_t DeleteRecord(cJSON *object)
{
	
	cJSON *item=NULL,*flag=NULL,*root=NULL; 
	char *out=NULL;
	
	int32_t restLen =0;
	char firstdayBuf[TIMEQUANTUMLEN]={0};
	char lastdayBuf[TIMEQUANTUMLEN]={0};
	char openuserIndexBuf[LIMITLENOPENRCORD/10]={0};//�����û���ַ����
	char *restOpenrecBuf=NULL;	
//  int8_t checktimeStatus =2;
	openIndex openrecordIndex={0};
	item=cJSON_GetObjectItem(object,"firstDay");//��ȡfirstDay
	sprintf(firstdayBuf,"%s",item->valuestring);
	item=cJSON_GetObjectItem(object,"lastDay");//��ȡlastDay
	sprintf(lastdayBuf,"%s",item->valuestring);
	
  
	openrecordIndex=OpenrecordTimequantum(firstdayBuf,lastdayBuf);
	
	/*************���ո��������ڣ����м�¼��������*****************/
	AT24CXX_Read(INDEXOPENUSER,openuserIndexBuf,sizeof(openuserIndexBuf));
	openuserIndex=atoi(openuserIndexBuf);
	
//	checktimeStatus = myStringCompare(firstdayBuf,lastdayBuf,TIMEQUANTUMLEN);
  
	restLen = openuserIndex-( OPENRECORDLEN * openrecordIndex.index_len );
//	printf("restOpenrecBuf:%d rest:%d\n",openuserIndex,openuserIndex-( OPENRECORDLEN * openrecordIndex.index_len ) );

	if(restLen>0)
	{
			restOpenrecBuf=(char*)malloc( restLen );
	}
	else if(restLen<=0)
	{
		restLen = 0;
		restOpenrecBuf = (char*)malloc( restLen );
	}
	 
	//����ʣ��Ŀ�����¼�Ļ����ڴ� ,�ڴ���������
//  printf("restOpenrecBuf:%d rest:%d\n",openuserIndex,openuserIndex-( OPENRECORDLEN * openrecordIndex.index_len ) );
	
	
	if(openuserIndex==0)
	{
	  AT24C128_Save(INDEXOPENUSER,openuserIndexBuf,sizeof(openuserIndexBuf));			
	}
	openuserIndex-=( OPENRECORDLEN * openrecordIndex.index_len );          
	sprintf(openuserIndexBuf,"%d",openuserIndex);
	AT24C128_Save(INDEXOPENUSER,openuserIndexBuf,sizeof(openuserIndexBuf));
	
	/*******************������ļ�¼��ǰ��Ų****************************/
	AT24CXX_Read(openrecordIndex.index_endTime,restOpenrecBuf,restLen);
//	printf("restOpenrecBuf1:%s restLen:%d\n",restOpenrecBuf,restLen);


	root=cJSON_CreateObject();     	
	cJSON_AddItemToObject(root, "data", flag=cJSON_CreateObject());	
	cJSON_AddStringToObject(flag,"deleteResult","true");
	cJSON_AddStringToObject(root,"flag","31");
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);
	free(out);	
	
	AT24C128_Save(openrecordIndex.index_startTime,restOpenrecBuf,restLen);
//	printf("openrecordIndex.index_startTime:%d\n",openrecordIndex.index_startTime);
//	printf("openrecordIndex.index_endTime:%d\n",openrecordIndex.index_endTime);
	free(restOpenrecBuf);	
	return 0;
}
/*************************************************************************************
*  Function Name                    :    CheckListUserVerify

*  Create Date                      :    2017/1/2
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    �ڲ�ѯ�û�ʱȷ����ʱ�û�������ͨ�û��Ƿ����ã�
                                         ��������ã�����и��û���ַ�����������������û���


*  Param                            :    checkList

*  Return Code                      :    checkList

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016.1.2    Liu Chuanjia     None           None

**************************************************************************************/
static checkList CheckListUserVerify(checkList checkIndex)
{
	char comuserAddBuf[3]={0};
	
  AT24CXX_Read(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf));
  comuserAdd=atoi(comuserAddBuf);                //����ȡ�����û���ַ
	
	if (checkIndex.index>=checkIndex.index_change)
	{
		if (checkIndex.index>=ADLOGAD && checkIndex.index<COMLOGAD)
		{
			checkIndex.index=COMBASEAD;
			checkIndex.index_change=COMLOGAD;
			if (comuserAdd==0)                           //�����ͨ�û�δ������
			{
				checkIndex.index=COMLOGAD;
			}
		}
		 if(checkIndex.index>=COMLOGAD&&checkIndex.index<TEMLOGAD)
		{
			checkIndex.index=TEMBASEAD;
			checkIndex.index_change=TEMLOGAD;	
		}
	}                               //�����һ���û������ã���������ˣ���������һ���û�
		return checkIndex;
}

/*************************************************************************************
*  Function Name                    :    CheckListJson

*  Create Date                      :    2017/1/1
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ����Ա�û�����鿴�û��б������û��б��͸�App


*  Param                            :    checkListNumb,thisCheckListNumb,checkListNumbTemp. checkIndex

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017.1.1  Liu Chuanjia     None           None

**************************************************************************************/
static uint8_t CheckListJson(const uint32_t checkListNumb,const uint32_t thisCheckListNumb,
	                             uint32_t checkListNumbTemp,checkList checkIndex)
{
	cJSON *root=NULL,*img=NULL,*flg=NULL; 
	char *out=NULL;		
	
  char temCount[2]={0};
	char Open_pwd[OPENPWDLEN]={0};	
  char Username[NAMELEN]={0};
	char Log_Pwd[LOGPWDLEN]={0};
	char checkUserListBuf[4]={0};
	
  uint32_t checkListlen=0;
		
		root=cJSON_CreateObject();
	  sprintf(checkUserListBuf,"%d",checkListNumb);
		cJSON_AddStringToObject(root,"checkListNumb",checkUserListBuf);
	  sprintf(checkUserListBuf,"%d",thisCheckListNumb);
		cJSON_AddStringToObject(root,"thisCheckListNumb",checkUserListBuf);
	
		cJSON_AddItemToObject(root, "data", img=cJSON_CreateArray());		
		for (checkListlen=0;checkListlen<thisCheckListNumb;checkListlen++)
		{					
				AT24CXX_Read(checkIndex.index,Open_pwd,OPENPWDLEN);
				checkIndex.index+=OPENPWDLEN;
				AT24CXX_Read(checkIndex.index,Username,NAMELEN);
				checkIndex.index+=NAMELEN;
				AT24CXX_Read(checkIndex.index,Log_Pwd,LOGPWDLEN);       //��ȡ��Ϣ��������
				checkIndex.index+=(LOGPWDLEN+IMEILEN);
			
//				printf("checkIndex.COMENDAD:%d\n",COMENDAD);
			  if(checkIndex.index>=COMENDAD&&checkIndex.index<TEMENDAD)
				checkIndex.index+=TEMCOUNTLEN;
				
//				printf("checkIndex.index1:%d\n",checkIndex.index);

				cJSON_AddItemToArray(img,flg=cJSON_CreateObject());
				cJSON_AddStringToObject(flg,"UserNameValues",Username);
				cJSON_AddStringToObject(flg,"UserPwdValues",Log_Pwd);
				cJSON_AddStringToObject(flg,"UserOpenPwdValues",Open_pwd);
				
				if(checkIndex.index>=0&&checkIndex.index<COMBASEAD)cJSON_AddStringToObject(flg,"userType","1");
				else if(checkIndex.index>=COMBASEAD&&checkIndex.index<TEMBASEAD)cJSON_AddStringToObject(flg,"userType","2");
				else if(checkIndex.index>=TEMBASEAD&&checkIndex.index<=TEMENDAD)
				{
					AT24CXX_Read(checkIndex.index-TEMCOUNTLEN,temCount,sizeof(temCount));  
					cJSON_AddStringToObject(flg,"userType","3");//�����û�����
					cJSON_AddStringToObject(flg,"UserOpenCountValues",temCount);
				}
				checkIndex=CheckListUserVerify(checkIndex);
				checkListNumbTemp--;
			}
		cJSON_AddStringToObject(root,"flag","8");
		out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);
		free(out);
			
	 if(checkListNumbTemp>0)
	 {
		 if(checkListNumbTemp>=thisCheckListNumb)
		 {
		   return CheckListJson(checkListNumb,thisCheckListNumb,
	                             checkListNumbTemp,checkIndex);
		 }
		 else if(checkListNumbTemp<thisCheckListNumb)
		 {
		   return CheckListJson(checkListNumb,checkListNumbTemp,
	                             checkListNumbTemp,checkIndex);		 
		 }
	 }
	   return 0;	
}
/*************************************************************************************
*  Function Name                    :    Check_Listing

*  Create Date                      :    2016/10/27
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ����Ա�û�����鿴�û��б������û��б��͸�App

*  Param                            :    checkListNumb,thisCheckListNumb,checkListNumbTemp. checkIndex

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016.10.27  Liu Chuanjia     None           None

**************************************************************************************/
uint8_t Check_Listing(void)
{ 
  checkList checkIndex={0};
	uint32_t checkListNumb=0;
	uint32_t checkListNumbTemp=0;
	
	char comuserAddBuf[3]={0};
	char temuserAddBuf[3]={0};

	checkIndex.index=ADMINADDRESS;//�ӻ���ַ��ʼ��ȡ�����û���Ϣ
	checkIndex.index_change=ADLOGAD;
	

  AT24CXX_Read(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf));
  comuserAdd=atoi(comuserAddBuf);//����ȡ�����û���ַ
  comuserNum=(comuserAdd/USERLEN); //�û��ĸ���
	AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf));		
	temuserAdd=atoi(temuserAddBuf);//��ȡ�û�����Ŀ
	temuserNum=(temuserAdd/(USERLEN+TEMCOUNTLEN));
		
	checkListNumb=ALLUSERNUM;
	checkListNumbTemp=checkListNumb;
	
	if(ALLUSERNUM<=CHECKLISTONEJSONNUM)
	{
		CheckListJson(checkListNumb,checkListNumb,checkListNumbTemp,checkIndex);
	}//����ܼ�¼��������ָ����Χ�ĸ���
	else if(ALLUSERNUM>CHECKLISTONEJSONNUM)
	{
		 CheckListJson(checkListNumb,CHECKLISTONEJSONNUM,checkListNumbTemp,checkIndex);
	}
	return 0;
}
/*************************************************************************************
*  Function Name                    :    Logpwd_change

*  Create Date                      :    2016/10/29
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ɾ���û�

*  Param                            :    cJSON *object

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016.10.29  Liu Chuanjia     None           None

**************************************************************************************/
int det_status=FAULT;
uint8_t User_det(cJSON *object)//��Ҫ�ѱ�ɾ�����û�������ǰŲ
{
		User Delete; 
	  cJSON *item;
		uint32_t index_det=0;             //index��ָ��������ĵ�ַ		
		uint32_t verify_name=1,verify_log=1;	 

	 char Name_buf[NAMELEN]={0};
	 char Log_buf[LOGPWDLEN]={0};//������û����͵�¼���룬�ȴ���֤
	 
		item=cJSON_GetObjectItem(object,"UserNameValues");
//	  memcpy(Delete.user_name,item->valuestring,sizeof(Delete.user_name));
	  sprintf(Delete.user_name,"%s",item->valuestring);
	 
	  item=cJSON_GetObjectItem(object,"UserPwdValues");
//	  memcpy(Delete.log_in_word,item->valuestring,sizeof(Delete.log_in_word));
	  sprintf(Delete.log_in_word,"%s",item->valuestring);
	 
	  /*���½����ַ����Ƚ��ˣ���֤�û���¼�û����������û�����*/
    index_det=ADMINADDRESS;
	 
		for (;index_det<ADLOGAD;)
		{  
			 index_det+=OPENPWDLEN;
	     AT24CXX_Read(index_det,Name_buf,NAMELEN);//��ȡ�Ѿ�������û���
			 index_det+=NAMELEN;
			 AT24CXX_Read(index_det,Log_buf,LOGPWDLEN);//��ȡ������û���¼����
       index_det=(index_det-NAMELEN-OPENPWDLEN);
			
			 verify_name=strcmp(Name_buf,Delete.user_name);	
			 verify_log=strcmp(Log_buf,Delete.log_in_word);
			 if (verify_name==0&&verify_log==0)
				 break;
				index_det+=USERLEN;
		}
		if (verify_name!=0 || verify_log!=0)
		{
			index_det=COMBASEAD;                                //��ͨ�û���Ϣ����ַ
			for (;index_det<=COMLOGAD;)
			{  
				 index_det+=OPENPWDLEN;
				 AT24CXX_Read(index_det,Name_buf,NAMELEN);//��ȡ�Ѿ�������û�?
				 index_det+=NAMELEN;
			   AT24CXX_Read(index_det,Log_buf,LOGPWDLEN);//��ȡ������û���¼����
				index_det=(index_det-NAMELEN-OPENPWDLEN);
				
				verify_name=strcmp(Name_buf,Delete.user_name);	
			  verify_log=strcmp(Log_buf,Delete.log_in_word);
				 if (verify_name==0&&verify_log==0)
					 break;
					index_det+=USERLEN;
			}
		}
		if (verify_name!=0 || verify_log!=0)
		{
			index_det=TEMBASEAD;
			for (;index_det<TEMLOGAD;)
			{  
				 index_det+=OPENPWDLEN;
				 AT24CXX_Read(index_det,Name_buf,NAMELEN);//��ȡ�Ѿ�������û���
				 index_det+=NAMELEN;
			   AT24CXX_Read(index_det,Log_buf,LOGPWDLEN);//��ȡ������û���¼����
         index_det=(index_det-NAMELEN-OPENPWDLEN);
				
				 verify_name=strcmp(Name_buf,Delete.user_name);	
			   verify_log=strcmp(Log_buf,Delete.log_in_word);
				 if (verify_name==0&&verify_log==0)
					 break;
					index_det+=(USERLEN+TEMCOUNTLEN);
			}				
		}
	  if (verify_name!=0 || verify_log!=0)
    det_status=FAULT;
		else if (verify_name==0&&verify_log==0)
		{
			det_status=Detleting(index_det);
		}
		detResult_send(det_status);
		return 0;
}

/*************************************************************************************
*  Function Name                    :    detResult_send

*  Create Date                      :    2016/10/29
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    ����ɾ���û����

*  Param                            :    status_det

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016.10.29  Liu Chuanjia     None           None

**************************************************************************************/
uint8_t detResult_send (int status_det)
{
  char *out;//�����͵�JSON
	cJSON *flag,*root;
	root=cJSON_CreateObject();                         //��������ṹ��
	
	cJSON_AddItemToObject(root, "data", flag=cJSON_CreateObject());
	
  if (status_det==SUCCESS)	
	  cJSON_AddStringToObject(flag,"detResult","true");//ɾ���ý�����ɹ�
	else if(status_det==FAULT)
		cJSON_AddStringToObject(flag,"detResult","false");//ɾ���û������ʧ��
	
	cJSON_AddStringToObject(root,"flag","12");
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);
	free(out);
	return 0;
}

/*
function:ɾ���û�������
name:Detleting
data:2016/10/29
developer��liuchuanjia 
param :detNamad    ,��ɾ���û�����ַ
return:����ɾ����״̬ true :0;false:1
*/
int Detleting(long detUser_base)
{
	  User det_user;
		long detUser_index=0;                     
		long member=0;
		char Temcon_buf[TEMCOUNTLEN]={0};
	  char comuserAddBuf[3]={0};
	  char temuserAddBuf[3]={0};
		
		detUser_index=detUser_base;   
	
	  if(detUser_index>=COMBASEAD&&detUser_index<TEMBASEAD)            //����ַ�ӿ������뿪ʼ
		{
			for (;detUser_index<=COMLOGAD;)
			{  
				 detUser_index+=USERLEN;
				 
				 AT24CXX_Read(detUser_index,det_user.open_word,OPENPWDLEN);  //��ȡ�Ѿ�������û���¼����
				 AT24C128_Save(detUser_index-USERLEN,det_user.open_word,OPENPWDLEN);//���������´洢		
				
				 member=detUser_index+OPENPWDLEN;
				 AT24CXX_Read(member,det_user.user_name,NAMELEN);              //��ȡ�Ѿ�������û���¼����
				 AT24C128_Save(member-USERLEN,det_user.user_name,NAMELEN);//���������´洢

				 member=member+NAMELEN;
				 AT24CXX_Read(member,det_user.log_in_word,LOGPWDLEN);              //��ȡ�Ѿ�������û���¼����  
				 AT24C128_Save(member-USERLEN,det_user.log_in_word,LOGPWDLEN);//���������´洢

         member=detUser_index+LOGPWDLEN;
			   AT24CXX_Read(member,det_user.IMEI,IMEILEN);              //��ȡ�Ѿ�������û���¼����  
         AT24C128_Save(member-USERLEN,det_user.IMEI,IMEILEN);        //���������´洢	
				
	       AT24CXX_Read(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf));
         comuserAdd=atoi(comuserAddBuf);//����ȡ�����û���ַ
         comuserAdd-=USERLEN;//�û���������һ��
			   comuserNum=(comuserAdd/USERLEN); //�û��ĸ���
	       sprintf(comuserAddBuf,"%d",comuserAdd);
	       AT24C128_Save(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf));
			}
		}
	  else if(detUser_index>=TEMBASEAD&&detUser_index<TEMENDAD)
		{
			for (;detUser_index<=TEMLOGAD;)
			{  
				 detUser_index+=(USERLEN+TEMCOUNTLEN);
				 
				 AT24CXX_Read(detUser_index,det_user.open_word,OPENPWDLEN);  //��ȡ�Ѿ�������û���¼����
				 AT24C128_Save(detUser_index-(USERLEN+TEMCOUNTLEN),det_user.open_word,OPENPWDLEN);//���������´洢		
				
				 member=detUser_index+OPENPWDLEN;
				 AT24CXX_Read(member,det_user.user_name,NAMELEN);              //��ȡ�Ѿ�������û���¼����
				 AT24C128_Save(member-(USERLEN+TEMCOUNTLEN),det_user.user_name,NAMELEN);//���������´洢

				 member=member+NAMELEN;
				 AT24CXX_Read(member,det_user.log_in_word,LOGPWDLEN);              //��ȡ�Ѿ�������û���¼����  
				 AT24C128_Save(member-(USERLEN+TEMCOUNTLEN),det_user.log_in_word,LOGPWDLEN);//���������´洢		
         
         member=member+LOGPWDLEN;
			   AT24CXX_Read(member,det_user.IMEI,IMEILEN);              //��ȡ�Ѿ�������û���¼����  
         AT24C128_Save(member-(USERLEN+TEMCOUNTLEN),det_user.IMEI,IMEILEN);        //���������´洢		
				
				 member=member+IMEILEN;
			   AT24CXX_Read(member,Temcon_buf,TEMCOUNTLEN);              //��ȡ�Ѿ�������û���¼����  
         AT24C128_Save(member-(USERLEN+TEMCOUNTLEN),Temcon_buf,TEMCOUNTLEN);        //���������´洢	
				
				 AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf));		
	       temuserAdd=atoi(temuserAddBuf);
         temuserAdd-=(USERLEN+TEMCOUNTLEN);//��ʱ�û���������һ��
		     temuserNum=(temuserAdd/(USERLEN+TEMCOUNTLEN));
	       sprintf(temuserAddBuf,"%d",temuserAdd);
	       AT24C128_Save(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf));	
			}
		}
		
	 det_status=SUCCESS;
   return det_status;		
}

/*************************************************************************************
*  Function Name                    :    Change_temcount

*  Create Date                      :    2016/11/06
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    �ı���ʱ�û��Ĵ���

*  Param                            :    cJSON *object

*  Return Code                      :    None

*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2016.11.06  Liu Chuanjia     None           None

**************************************************************************************/
uint8_t Change_temcount(cJSON *object)
{
  	User Chtem_count; 
	  cJSON *item,*flag,*root;char *out;
	 uint32_t indexChangeTemUser=0;  
	 uint32_t indexChangeTemCount=0;           //index��ָ��������ĵ�ַ		

	 char Temcou_buf[TEMCOUNTLEN+1]={0};
	 
	 
		item=cJSON_GetObjectItem(object,"UserNameValues");
//	  memcpy(Chtem_count.user_name,item->valuestring,sizeof(Chtem_count.user_name));
	  sprintf(Chtem_count.user_name,"%s",item->valuestring);

	 item=cJSON_GetObjectItem(object,"UserPwdValues");
//	 memcpy(Chtem_count.log_in_word,item->valuestring,sizeof(Chtem_count.log_in_word));
	 sprintf(Chtem_count.log_in_word,"%s",item->valuestring);
	 
	 item=cJSON_GetObjectItem(object,"UserOpenCountValues");
//	 memcpy(Temcou_buf,item->valuestring,sizeof(Temcou_buf));
	 sprintf(Temcou_buf,"%s",item->valuestring);
		
    /****************************Ѱ�ҵ�ǰ��Ҫ���޸Ŀ�����¼����ʱ�û�********************************/
    indexChangeTemUser = FindThisTemUserBase_Name_Logpwd(Chtem_count.user_name,Chtem_count.log_in_word);
	 
		indexChangeTemCount = indexChangeTemUser+USERLEN;
	  root=cJSON_CreateObject();            //��������ṹ��
		cJSON_AddItemToObject(root, "data", flag=cJSON_CreateObject());	
	 
	  if (indexChangeTemUser==FAULT)           //δ�ҵ���ǰ���������û�
		{
			cJSON_AddStringToObject(flag,"ChangeOpenCountResult","false");
		}
		else if(indexChangeTemUser!=FAULT)          //�ҵ��˵�ǰ�������û������ݵ�ǰ�û��ĵ�ַ�޸Ŀ���   
		{	
			AT24C128_Save(indexChangeTemCount,Temcou_buf,TEMCOUNTLEN);
			cJSON_AddStringToObject(flag,"ChangeOpenCountResult","true");
		}
		cJSON_AddStringToObject(root,"flag","23");
		out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);
		free(out);
		return 0;
}
/*************************************************************************************
*  Function Name                    :   FindThisUserBase_Name

*  Create Date                      :    2017/03/09
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    Ѱ�ҵ�ǰ����ͨ�û�
                                        

*  Param                            :    char *RX_name
*  Return Code                      :    None
																			
*  Global Variable                  :    None
*  File Static Variable             :    None
*  Function Static Variable         :    None
																			 
*--------------------------------Revision History--------------------------------------
*  No         Version         Date        RevisedBy        Item         Description  
*  1          V1.0            2017/03/09 Liu Chuanjia     None           None

**************************************************************************************/
uint32_t FindThisUserBase_Name(char *RX_name)
{
	 int index=0;
	 char Name_buf[NAMELEN]={0};
//	 char Log_buf[LOGPWDLEN]={0};//������û����͵�¼���룬�ȴ���֤
	 char comuserAddBuf[LIMITLENOPENRCORD/10]={0}; 
	 char temuserAddBuf[LIMITLENOPENRCORD/10]={0}; 
	 
	 int verify_name=1;
//		 verify_log=1;	 	 
	 
	index=COMBASEAD;      //��ͨ�û���Ϣ����ַ
	AT24CXX_Read(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf)); 
	 
	 /************����ͨ�û�������*********************/
	comuserAdd=atoi(comuserAddBuf);			
	for (;index<COMLOGAD;)
	{  
		 index+=OPENPWDLEN;
		 AT24CXX_Read(index,Name_buf,NAMELEN);//��ȡ�Ѿ�������û�?
//		 index+=NAMELEN;
//		 AT24CXX_Read(index,Log_buf,LOGPWDLEN);//��ȡ������û���¼����
		 index=(index-OPENPWDLEN);
		 verify_name=strcmp(Name_buf,RX_name);	
//		 verify_log=strcmp(Log_buf,RX_log);
		 if (verify_name==0)
			 break;
			index+=USERLEN;		
	 }
	  /************����ʱ�û�������*********************/
	 	if (verify_name!=0)
		{			
			index=TEMBASEAD;
		  AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf)); 
		  temuserAdd=atoi(temuserAddBuf);				
		  for (;index<TEMLOGAD;)
		  {  		
			  index+=OPENPWDLEN;
			  AT24CXX_Read(index,Name_buf,NAMELEN);//��ȡ�Ѿ�������û���
//			 printf("Name_buf:%s\r\n",Name_buf);
//			 index+=NAMELEN;
//			 AT24CXX_Read(index,Log_buf,LOGPWDLEN);//��ȡ������û���¼����		
//       printf("Log_buf:%s\r\n",Log_buf);			
			 index=(index-OPENPWDLEN);
			 verify_name=strcmp(Name_buf,RX_name);	
//			 verify_log=strcmp(Log_buf,RX_log);
			 if (verify_name==0)
				 break;
			 index+=(USERLEN+TEMCOUNTLEN);
		  }	
		}
	  if (verify_name!=0)
		{			
       return FAULT;
		}
		else
		return index;
}
