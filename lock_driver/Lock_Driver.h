#ifndef _LOCK_DRIVER_H_
#define _LOCK_DRIVER_H_
#include "JSON_Service/cJSON.h"
#include "stm32f10x.h"

//地址篇//将三个用户分为三个区0~100管理员 1000~2000普通用户 2000~3000临时用户
extern uint32_t comuserAdd;   //普通用户增加索引
extern uint32_t temuserAdd;    //临时用户增加索引
extern uint32_t usernameIndex;
extern uint32_t openuserIndex;
extern uint32_t temuserNum; 
extern uint32_t comuserNum;
//temuserNum
//数据帧长度
#define NAMELEN 10
#define LOGPWDLEN 10
#define OPENPWDLEN 10
#define TIMEQUANTUMLEN 10
#define IMEILEN 15     
#define TIMELEN 20
#define OPENRECORDLEN (TIMELEN+NAMELEN)
//国际标识的长度
#define TEMCOUNTLEN 3
#define LOGDATALEN      (NAMELEN+LOGPWDLEN)
#define USERLEN         (NAMELEN+LOGPWDLEN+OPENPWDLEN+IMEILEN)      //用户一帧数据的长度  

#define ADMINADDRESS  0
#define ADMINOPENPWDAD   ADMINADDRESS                //管理员开门密码基地址//以后需要改掉
#define ADNAMEAD        (ADMINOPENPWDAD+OPENPWDLEN) //管理员用户名地址
#define ADLOGAD         (ADNAMEAD+NAMELEN)          //管理员登录密码地址
#define ADIMEIAD        (ADNAMEAD+ADLOGAD)          //管理员国际标识码的地址

#define COMBASEAD      1000
#define COMADDRESS    (COMBASEAD-USERLEN+comuserAdd)
#define COMOPENPWDAD   COMADDRESS                   //普通用户的开门密码
#define COMNAMEAD     (COMOPENPWDAD+OPENPWDLEN)
#define COMLOGAD      (COMNAMEAD+NAMELEN)
#define COMIMEI       (COMLOGAD+LOGPWDLEN)
#define COMENDAD      (COMIMEI+IMEILEN)

#define TEMBASEAD       2000
#define TEMADDRESS     (TEMBASEAD-USERLEN-TEMCOUNTLEN+temuserAdd)         
#define TEMOPENPWDAD    TEMADDRESS                       //临时用户开门密码      
#define TEMNAMEAD      (TEMOPENPWDAD+OPENPWDLEN)
#define TEMLOGAD       (TEMNAMEAD+NAMELEN)
#define TEMIMEI        (TEMLOGAD+LOGPWDLEN)
#define TEMCOUNTAD     (TEMIMEI+IMEILEN)
#define TEMENDAD        3000

#define LOGNAMEBASEAD     4000 
#define LOGUSERNAME       (LOGNAMEBASEAD+usernameIndex)     //保存用户名的地址
#define LOGPWDBASEAD      4500
#define LOGPWDAD          (LOGPWDBASEAD+usernameIndex)//忘了是在哪些方面有应用？

//索引篇
#define INDEXCOMUSERADD      5000
#define INDEXTEMUSERADD      5010
//#define INDEXTEMCOUNTTEMAD   5020
//#define INDEXTEMCOUNTTEMADAD 5030
#define INDEXOPENUSER        5100

#define OPENRECORDBASEAD   (6000-NAMELEN)
#define USERNAMEOPENRE    (OPENRECORDBASEAD+openuserIndex) //保存开门用户名的地址,用于查询+30
#define TIMEOPENRE        (USERNAMEOPENRE+10)         //保存开门的时间的地址，用于查询
#define OPENRECORDONEJSONNUM 4
#define CHECKLISTONEJSONNUM  3
//指令篇
#define OPENED               1
#define CLOSED               2

#define LOGVEYIFYSIGN        3
#define LOGPWDCHANGESIGN     5
#define OPENPWDCHANGESIGN    6

#define SUCCESS              7
#define FAULT                8

#define ALLCOMUSERNUM    comuserNum
#define ALLTEMUSERNUM    temuserNum
#define ALLUSERNUM      (comuserNum+temuserNum+1)//现存用户总个数
#define ALLRECORDNUM    (openuserIndex/30)//现存的所有的开锁记录的条数
#define LIMITLENOPENRCORD    1000
//#define TEMUSERNUM           10
#define COMUSERSETLIMIT        5
#define TEMUSERSETLIMIT        5
#define RXBUFLEN               200

#define RECORDCHECK           0
#define DELETERECORE          1


#define USART1IT              2
//消息flag宏定义
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
