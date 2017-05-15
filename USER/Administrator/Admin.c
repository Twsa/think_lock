#include "Administrator/Admin.h"


uint32_t comuserAdd=0;//用户增加索引
uint32_t temuserAdd=0;
uint32_t comuserNum=0;//当前普通用户个数
uint32_t temuserNum=0;//当前临时用户的个数
extern char STM32_ID[24];//单片机的ID

/*************************************************************************************
*  Function Name                    :    Admin_set

*  Create Date                      :    2016/10/18
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    管理员用户设置


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
		
		AT24C128_Save(ADMINOPENPWDAD,Admin.open_word,OPENPWDLEN);//保存开门密码		
    AT24C128_Save(ADNAMEAD,Admin.user_name,NAMELEN);//保存用户名
		AT24C128_Save(ADLOGAD,Admin.log_in_word,LOGPWDLEN);//保存登录密码
    AT24C128_Save(ADIMEIAD,Admin.IMEI,IMEILEN);//保存国际标识
		AT24C128_Save(LOGNAMEBASEAD,Admin.user_name,sizeof(Admin.user_name));
		
    Get_ChipID(); //获取单片机ID
    AT24C128_ID();//保存ID
		root=cJSON_CreateObject();             //创建对象结构体
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
*  Description                      :    更改用户后返回的信息


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
	root=cJSON_CreateObject();            //创建对象结构体
	cJSON_AddItemToObject(root, "data", flag=cJSON_CreateObject());	
	
	if (status== SUCCESS)
	{
			cJSON_AddStringToObject(flag,"addResult","true");//添加用户成功
	}
	else if(status== FAULT)
	{
		cJSON_AddStringToObject(flag,"addResult","false");//添加用户失败
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
*  Description                      :    按一定长度，字符串比较
                                        

*  Param                            :    const char* Source,const char* Destination,const uint8_t lenNeedtoCompare

*  Return Code                      :    int8_t equal:0 小于：-1；大于：1
																			
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
*  Description                      :    在时间段中查询开锁记录
                                        

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
			openRecordQuantum.index_time+=OPENRECORDLEN;                //指针指向当前位置完成当前操作，然后指向下一个位置
			else if(openRecordQuantum.index_time==TIMEOPENRE) break;		//让指针停在末尾地址
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
*  Description                      :    管理员申请查看开锁记录以Json格式发送的部分


*  Param                            :    openLockHistoryNumb,thisHistoryNumb,index_name,index_time
                                         记录总条数 ，当前JSON包含的记录的条数，用户名索引，开门时间索引

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
*  Description                      :    管理员申请查看开锁记录


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
		
	item=cJSON_GetObjectItem(object,"firstDay");//提取firstDay
	sprintf(firstdayBuf,"%s",item->valuestring);
	item=cJSON_GetObjectItem(object,"lastDay");//提取lastDay
	sprintf(lastdayBuf,"%s",item->valuestring);
	
	openrecordIndex=OpenrecordTimequantum(firstdayBuf,lastdayBuf);
	
//	openrecordIndex.index_name=OPENRECORDBASEAD;
//	openrecordIndex.index_time=(OPENRECORDBASEAD+NAMELEN);
	
	openrecordIndex.index_name=(openrecordIndex.index_startTime-NAMELEN);
	openrecordIndex.index_time=openrecordIndex.index_startTime;

//	AT24CXX_Read(INDEXOPENUSER,checkAccountNumBuf,sizeof(checkAccountNumBuf));
//  openuserIndex=atoi(checkAccountNumBuf);//丁读取开门用户地址
//	checkRecordNum=ALLRECORDNUM;
	checkRecordNum=openrecordIndex.index_len;
	checkRecordNumTemp=checkRecordNum;
	
	if(checkRecordNum<=OPENRECORDONEJSONNUM)
	{
		OpenDataJson(checkRecordNum,checkRecordNum,checkRecordNumTemp,openrecordIndex);
	}//如果总记录条数超过指定范围的个数
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
*  Description                      :    管理员删除开锁记录


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
	char openuserIndexBuf[LIMITLENOPENRCORD/10]={0};//开门用户地址缓存
	char *restOpenrecBuf=NULL;	
//  int8_t checktimeStatus =2;
	openIndex openrecordIndex={0};
	item=cJSON_GetObjectItem(object,"firstDay");//提取firstDay
	sprintf(firstdayBuf,"%s",item->valuestring);
	item=cJSON_GetObjectItem(object,"lastDay");//提取lastDay
	sprintf(lastdayBuf,"%s",item->valuestring);
	
  
	openrecordIndex=OpenrecordTimequantum(firstdayBuf,lastdayBuf);
	
	/*************按照给定的日期，所有记录索引减少*****************/
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
	 
	//分配剩余的开锁记录的缓存内存 ,内存上限限制
//  printf("restOpenrecBuf:%d rest:%d\n",openuserIndex,openuserIndex-( OPENRECORDLEN * openrecordIndex.index_len ) );
	
	
	if(openuserIndex==0)
	{
	  AT24C128_Save(INDEXOPENUSER,openuserIndexBuf,sizeof(openuserIndexBuf));			
	}
	openuserIndex-=( OPENRECORDLEN * openrecordIndex.index_len );          
	sprintf(openuserIndexBuf,"%d",openuserIndex);
	AT24C128_Save(INDEXOPENUSER,openuserIndexBuf,sizeof(openuserIndexBuf));
	
	/*******************将后面的记录往前面挪****************************/
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
*  Description                      :    在查询用户时确认临时用户或者普通用户是否被设置，
                                         如果被设置，则进行该用户地址索引，否则跳过该用户区


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
  comuserAdd=atoi(comuserAddBuf);                //丁读取开门用户地址
	
	if (checkIndex.index>=checkIndex.index_change)
	{
		if (checkIndex.index>=ADLOGAD && checkIndex.index<COMLOGAD)
		{
			checkIndex.index=COMBASEAD;
			checkIndex.index_change=COMLOGAD;
			if (comuserAdd==0)                           //如果普通用户未被设置
			{
				checkIndex.index=COMLOGAD;
			}
		}
		 if(checkIndex.index>=COMLOGAD&&checkIndex.index<TEMLOGAD)
		{
			checkIndex.index=TEMBASEAD;
			checkIndex.index_change=TEMLOGAD;	
		}
	}                               //检查下一阶用户被设置，如果设置了，则跳到下一阶用户
		return checkIndex;
}

/*************************************************************************************
*  Function Name                    :    CheckListJson

*  Create Date                      :    2017/1/1
*  Author/Corporation               :    Liu Chuanjia
*  Description                      :    管理员用户申请查看用户列表，并将用户列表发送给App


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
				AT24CXX_Read(checkIndex.index,Log_Pwd,LOGPWDLEN);       //获取信息及其数据
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
					cJSON_AddStringToObject(flg,"userType","3");//保存用户类型
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
*  Description                      :    管理员用户申请查看用户列表，并将用户列表发送给App

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

	checkIndex.index=ADMINADDRESS;//从基地址开始读取所有用户信息
	checkIndex.index_change=ADLOGAD;
	

  AT24CXX_Read(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf));
  comuserAdd=atoi(comuserAddBuf);//丁读取开门用户地址
  comuserNum=(comuserAdd/USERLEN); //用户的个数
	AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf));		
	temuserAdd=atoi(temuserAddBuf);//读取用户总数目
	temuserNum=(temuserAdd/(USERLEN+TEMCOUNTLEN));
		
	checkListNumb=ALLUSERNUM;
	checkListNumbTemp=checkListNumb;
	
	if(ALLUSERNUM<=CHECKLISTONEJSONNUM)
	{
		CheckListJson(checkListNumb,checkListNumb,checkListNumbTemp,checkIndex);
	}//如果总记录条数超过指定范围的个数
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
*  Description                      :    删除用户

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
uint8_t User_det(cJSON *object)//需要把被删除的用户数据往前挪
{
		User Delete; 
	  cJSON *item;
		uint32_t index_det=0;             //index是指开门密码的地址		
		uint32_t verify_name=1,verify_log=1;	 

	 char Name_buf[NAMELEN]={0};
	 char Log_buf[LOGPWDLEN]={0};//保存的用户名和登录密码，等待验证
	 
		item=cJSON_GetObjectItem(object,"UserNameValues");
//	  memcpy(Delete.user_name,item->valuestring,sizeof(Delete.user_name));
	  sprintf(Delete.user_name,"%s",item->valuestring);
	 
	  item=cJSON_GetObjectItem(object,"UserPwdValues");
//	  memcpy(Delete.log_in_word,item->valuestring,sizeof(Delete.log_in_word));
	  sprintf(Delete.log_in_word,"%s",item->valuestring);
	 
	  /*接下进行字符串比较了，验证用户登录用户，并换回用户类型*/
    index_det=ADMINADDRESS;
	 
		for (;index_det<ADLOGAD;)
		{  
			 index_det+=OPENPWDLEN;
	     AT24CXX_Read(index_det,Name_buf,NAMELEN);//读取已经保存的用户名
			 index_det+=NAMELEN;
			 AT24CXX_Read(index_det,Log_buf,LOGPWDLEN);//读取保存的用户登录密码
       index_det=(index_det-NAMELEN-OPENPWDLEN);
			
			 verify_name=strcmp(Name_buf,Delete.user_name);	
			 verify_log=strcmp(Log_buf,Delete.log_in_word);
			 if (verify_name==0&&verify_log==0)
				 break;
				index_det+=USERLEN;
		}
		if (verify_name!=0 || verify_log!=0)
		{
			index_det=COMBASEAD;                                //普通用户信息基地址
			for (;index_det<=COMLOGAD;)
			{  
				 index_det+=OPENPWDLEN;
				 AT24CXX_Read(index_det,Name_buf,NAMELEN);//读取已经保存的用户?
				 index_det+=NAMELEN;
			   AT24CXX_Read(index_det,Log_buf,LOGPWDLEN);//读取保存的用户登录密码
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
				 AT24CXX_Read(index_det,Name_buf,NAMELEN);//读取已经保存的用户名
				 index_det+=NAMELEN;
			   AT24CXX_Read(index_det,Log_buf,LOGPWDLEN);//读取保存的用户登录密码
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
*  Description                      :    发送删除用户结果

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
  char *out;//待发送的JSON
	cJSON *flag,*root;
	root=cJSON_CreateObject();                         //创建对象结构体
	
	cJSON_AddItemToObject(root, "data", flag=cJSON_CreateObject());
	
  if (status_det==SUCCESS)	
	  cJSON_AddStringToObject(flag,"detResult","true");//删除用结果：成功
	else if(status_det==FAULT)
		cJSON_AddStringToObject(flag,"detResult","false");//删除用户结果：失败
	
	cJSON_AddStringToObject(root,"flag","12");
	out=cJSON_Print(root);	cJSON_Delete(root);	printf("%s\n",out);
	free(out);
	return 0;
}

/*
function:删除用户进行中
name:Detleting
data:2016/10/29
developer：liuchuanjia 
param :detNamad    ,待删除用户名地址
return:返回删除的状态 true :0;false:1
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
	
	  if(detUser_index>=COMBASEAD&&detUser_index<TEMBASEAD)            //基地址从开门密码开始
		{
			for (;detUser_index<=COMLOGAD;)
			{  
				 detUser_index+=USERLEN;
				 
				 AT24CXX_Read(detUser_index,det_user.open_word,OPENPWDLEN);  //读取已经保存的用户登录数据
				 AT24C128_Save(detUser_index-USERLEN,det_user.open_word,OPENPWDLEN);//读出来重新存储		
				
				 member=detUser_index+OPENPWDLEN;
				 AT24CXX_Read(member,det_user.user_name,NAMELEN);              //读取已经保存的用户登录数据
				 AT24C128_Save(member-USERLEN,det_user.user_name,NAMELEN);//读出来重新存储

				 member=member+NAMELEN;
				 AT24CXX_Read(member,det_user.log_in_word,LOGPWDLEN);              //读取已经保存的用户登录数据  
				 AT24C128_Save(member-USERLEN,det_user.log_in_word,LOGPWDLEN);//读出来重新存储

         member=detUser_index+LOGPWDLEN;
			   AT24CXX_Read(member,det_user.IMEI,IMEILEN);              //读取已经保存的用户登录数据  
         AT24C128_Save(member-USERLEN,det_user.IMEI,IMEILEN);        //读出来重新存储	
				
	       AT24CXX_Read(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf));
         comuserAdd=atoi(comuserAddBuf);//丁读取开门用户地址
         comuserAdd-=USERLEN;//用户索引减少一个
			   comuserNum=(comuserAdd/USERLEN); //用户的个数
	       sprintf(comuserAddBuf,"%d",comuserAdd);
	       AT24C128_Save(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf));
			}
		}
	  else if(detUser_index>=TEMBASEAD&&detUser_index<TEMENDAD)
		{
			for (;detUser_index<=TEMLOGAD;)
			{  
				 detUser_index+=(USERLEN+TEMCOUNTLEN);
				 
				 AT24CXX_Read(detUser_index,det_user.open_word,OPENPWDLEN);  //读取已经保存的用户登录数据
				 AT24C128_Save(detUser_index-(USERLEN+TEMCOUNTLEN),det_user.open_word,OPENPWDLEN);//读出来重新存储		
				
				 member=detUser_index+OPENPWDLEN;
				 AT24CXX_Read(member,det_user.user_name,NAMELEN);              //读取已经保存的用户登录数据
				 AT24C128_Save(member-(USERLEN+TEMCOUNTLEN),det_user.user_name,NAMELEN);//读出来重新存储

				 member=member+NAMELEN;
				 AT24CXX_Read(member,det_user.log_in_word,LOGPWDLEN);              //读取已经保存的用户登录数据  
				 AT24C128_Save(member-(USERLEN+TEMCOUNTLEN),det_user.log_in_word,LOGPWDLEN);//读出来重新存储		
         
         member=member+LOGPWDLEN;
			   AT24CXX_Read(member,det_user.IMEI,IMEILEN);              //读取已经保存的用户登录数据  
         AT24C128_Save(member-(USERLEN+TEMCOUNTLEN),det_user.IMEI,IMEILEN);        //读出来重新存储		
				
				 member=member+IMEILEN;
			   AT24CXX_Read(member,Temcon_buf,TEMCOUNTLEN);              //读取已经保存的用户登录数据  
         AT24C128_Save(member-(USERLEN+TEMCOUNTLEN),Temcon_buf,TEMCOUNTLEN);        //读出来重新存储	
				
				 AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf));		
	       temuserAdd=atoi(temuserAddBuf);
         temuserAdd-=(USERLEN+TEMCOUNTLEN);//临时用户索引减少一个
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
*  Description                      :    改变临时用户的次数

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
	 uint32_t indexChangeTemCount=0;           //index是指开门密码的地址		

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
		
    /****************************寻找当前需要被修改开锁记录的临时用户********************************/
    indexChangeTemUser = FindThisTemUserBase_Name_Logpwd(Chtem_count.user_name,Chtem_count.log_in_word);
	 
		indexChangeTemCount = indexChangeTemUser+USERLEN;
	  root=cJSON_CreateObject();            //创建对象结构体
		cJSON_AddItemToObject(root, "data", flag=cJSON_CreateObject());	
	 
	  if (indexChangeTemUser==FAULT)           //未找到当前所描述的用户
		{
			cJSON_AddStringToObject(flag,"ChangeOpenCountResult","false");
		}
		else if(indexChangeTemUser!=FAULT)          //找到了当前描述的用户，根据当前用户的地址修改开锁   
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
*  Description                      :    寻找当前的普通用户
                                        

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
//	 char Log_buf[LOGPWDLEN]={0};//保存的用户名和登录密码，等待验证
	 char comuserAddBuf[LIMITLENOPENRCORD/10]={0}; 
	 char temuserAddBuf[LIMITLENOPENRCORD/10]={0}; 
	 
	 int verify_name=1;
//		 verify_log=1;	 	 
	 
	index=COMBASEAD;      //普通用户信息基地址
	AT24CXX_Read(INDEXCOMUSERADD,comuserAddBuf,sizeof(comuserAddBuf)); 
	 
	 /************在普通用户区中找*********************/
	comuserAdd=atoi(comuserAddBuf);			
	for (;index<COMLOGAD;)
	{  
		 index+=OPENPWDLEN;
		 AT24CXX_Read(index,Name_buf,NAMELEN);//读取已经保存的用户?
//		 index+=NAMELEN;
//		 AT24CXX_Read(index,Log_buf,LOGPWDLEN);//读取保存的用户登录密码
		 index=(index-OPENPWDLEN);
		 verify_name=strcmp(Name_buf,RX_name);	
//		 verify_log=strcmp(Log_buf,RX_log);
		 if (verify_name==0)
			 break;
			index+=USERLEN;		
	 }
	  /************在临时用户区中找*********************/
	 	if (verify_name!=0)
		{			
			index=TEMBASEAD;
		  AT24CXX_Read(INDEXTEMUSERADD,temuserAddBuf,sizeof(temuserAddBuf)); 
		  temuserAdd=atoi(temuserAddBuf);				
		  for (;index<TEMLOGAD;)
		  {  		
			  index+=OPENPWDLEN;
			  AT24CXX_Read(index,Name_buf,NAMELEN);//读取已经保存的用户名
//			 printf("Name_buf:%s\r\n",Name_buf);
//			 index+=NAMELEN;
//			 AT24CXX_Read(index,Log_buf,LOGPWDLEN);//读取保存的用户登录密码		
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
