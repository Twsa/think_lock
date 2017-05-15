#ifndef _LOCK_DRIVER_H_
#define _LOCK_DRIVER_H_
#include "JSON_Service/cJSON.h"
#include "stm32f10x.h"

//��ַƪ//�������û���Ϊ������0~100����Ա 1000~2000��ͨ�û� 2000~3000��ʱ�û�
extern uint32_t comuserAdd;   //��ͨ�û���������
extern uint32_t temuserAdd;    //��ʱ�û���������
extern uint32_t usernameIndex;
extern uint32_t openuserIndex;
extern uint32_t temuserNum; 
extern uint32_t comuserNum;
//temuserNum
//����֡����
#define NAMELEN 10
#define LOGPWDLEN 10
#define OPENPWDLEN 10
#define TIMEQUANTUMLEN 10
#define IMEILEN 15     
#define TIMELEN 20
#define OPENRECORDLEN (TIMELEN+NAMELEN)
//���ʱ�ʶ�ĳ���
#define TEMCOUNTLEN 3
#define LOGDATALEN      (NAMELEN+LOGPWDLEN)
#define USERLEN         (NAMELEN+LOGPWDLEN+OPENPWDLEN+IMEILEN)      //�û�һ֡���ݵĳ���  

#define ADMINADDRESS  0
#define ADMINOPENPWDAD   ADMINADDRESS                //����Ա�����������ַ//�Ժ���Ҫ�ĵ�
#define ADNAMEAD        (ADMINOPENPWDAD+OPENPWDLEN) //����Ա�û�����ַ
#define ADLOGAD         (ADNAMEAD+NAMELEN)          //����Ա��¼�����ַ
#define ADIMEIAD        (ADNAMEAD+ADLOGAD)          //����Ա���ʱ�ʶ��ĵ�ַ

#define COMBASEAD      1000
#define COMADDRESS    (COMBASEAD-USERLEN+comuserAdd)
#define COMOPENPWDAD   COMADDRESS                   //��ͨ�û��Ŀ�������
#define COMNAMEAD     (COMOPENPWDAD+OPENPWDLEN)
#define COMLOGAD      (COMNAMEAD+NAMELEN)
#define COMIMEI       (COMLOGAD+LOGPWDLEN)
#define COMENDAD      (COMIMEI+IMEILEN)

#define TEMBASEAD       2000
#define TEMADDRESS     (TEMBASEAD-USERLEN-TEMCOUNTLEN+temuserAdd)         
#define TEMOPENPWDAD    TEMADDRESS                       //��ʱ�û���������      
#define TEMNAMEAD      (TEMOPENPWDAD+OPENPWDLEN)
#define TEMLOGAD       (TEMNAMEAD+NAMELEN)
#define TEMIMEI        (TEMLOGAD+LOGPWDLEN)
#define TEMCOUNTAD     (TEMIMEI+IMEILEN)
#define TEMENDAD        3000

#define LOGNAMEBASEAD     4000 
#define LOGUSERNAME       (LOGNAMEBASEAD+usernameIndex)     //�����û����ĵ�ַ
#define LOGPWDBASEAD      4500
#define LOGPWDAD          (LOGPWDBASEAD+usernameIndex)//����������Щ������Ӧ�ã�

//����ƪ
#define INDEXCOMUSERADD      5000
#define INDEXTEMUSERADD      5010
//#define INDEXTEMCOUNTTEMAD   5020
//#define INDEXTEMCOUNTTEMADAD 5030
#define INDEXOPENUSER        5100

#define OPENRECORDBASEAD   (6000-NAMELEN)
#define USERNAMEOPENRE    (OPENRECORDBASEAD+openuserIndex) //���濪���û����ĵ�ַ,���ڲ�ѯ+30
#define TIMEOPENRE        (USERNAMEOPENRE+10)         //���濪�ŵ�ʱ��ĵ�ַ�����ڲ�ѯ
#define OPENRECORDONEJSONNUM 4
#define CHECKLISTONEJSONNUM  3
//ָ��ƪ
#define OPENED               1
#define CLOSED               2

#define LOGVEYIFYSIGN        3
#define LOGPWDCHANGESIGN     5
#define OPENPWDCHANGESIGN    6

#define SUCCESS              7
#define FAULT                8

#define ALLCOMUSERNUM    comuserNum
#define ALLTEMUSERNUM    temuserNum
#define ALLUSERNUM      (comuserNum+temuserNum+1)//�ִ��û��ܸ���
#define ALLRECORDNUM    (openuserIndex/30)//�ִ�����еĿ�����¼������
#define LIMITLENOPENRCORD    1000
//#define TEMUSERNUM           10
#define COMUSERSETLIMIT        5
#define TEMUSERSETLIMIT        5
#define RXBUFLEN               200

#define RECORDCHECK           0
#define DELETERECORE          1


#define USART1IT              2
//��Ϣflag�궨��
#define RSAHANDLER            1
#define USELOG                3
#define OPENDOOR              5
#define CHECKUSERLIST         7
#define SETCOMUSER            9
#define DETUSER               11
#define CHECKOPENRECORD       13
#define RESETOPENPWD          15
#define SETTEMUSER            17
#define SETADMINUSER          18
#define RESETLOGPWD           20
#define CHANGETEMCOUNT        22
#define DELETERECOREFLAG      30

#endif
