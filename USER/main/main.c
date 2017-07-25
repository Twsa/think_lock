#include "main/main.h"

char MISI_data[15];
uint8_t ClearRecordandUser(void);
void HSIClock_Init(uint8_t SYSCLK);
void Arc4Test(void);
void RSA_Test(void);

int main()
{ 
  SystemInit();       //ϵͳʱ�ӳ�ʼ��
//	HSIClock_Init(64);
//  ClearRecord();
  USART1_Config();
  Tim2_Configure();
  AT24CXX_Init();
  delay_init(72);
//	AT24C128_Save(ADMINOPENPWDAD,"123",LOGPWDLEN);//�����¼����	
//	AT24C128_Save(ADNAMEAD,"Zhu",LOGPWDLEN);//�����¼����	
//	AT24C128_Save(ADLOGAD,"Zhu",LOGPWDLEN);//�����¼����	
//	AT24C128_Read(ADMINOPENPWDAD,10);		
//  RSA_Test();	
	while(1)
	{	
	  JSON_Handle();
	}
}



//��¼ʱ����һ����Ϣ
//�˳���¼ʱ����һ����Ϣ
//	openIndex abc={0};

void Arc4Test(void)
{
 int i=0;
	uint8_t PainText[11]="liuchuanjia";
	uint8_t EncryptText[sizeof(PainText)]={0xD7,0x9A,0x4C,0xB7,0x61,0xC4,
	                                       0xBF,0xC9,0x9A,0x5A,0x6B};
//	uint8_t DecryptText[sizeof(PainText)]={0};
  uint8_t MessageOutput[11]={0};
	uint8_t arc4Key[8]="12345678";
	
	Arc4Encrypt(arc4Key,sizeof(arc4Key),PainText,sizeof(PainText),MessageOutput);
	for(;i<sizeof(PainText);i++)
	{
		printf("%x",MessageOutput[i]);
	}
	printf("\r\n");
	Arc4Decrypt(arc4Key,sizeof(arc4Key),EncryptText,sizeof(EncryptText),MessageOutput);
	printf("%s\r\n",MessageOutput);
}

void Str2HexPainTest(void)
{
	uint8_t String[73]="303132333435363738396162636465666768696a6b6c6d6e6f707172737475767778797a";
	uint8_t Hex[36]={0};

	Str2HexPain((uint8_t *)String ,(uint8_t *)Hex,(uint32_t)(sizeof(String)-1));
	for(int i=0;i<sizeof(Hex);i++)
	{
		printf("%x\r\n",Hex[i]);
	}
}
//�ڲ�����ʱ��HSIʱ������
void HSIClock_Init(uint8_t SYSCLK)
{
	uint32_t RCC_PLLMul_Index=0;
	
	RCC_DeInit(); 
	RCC_HSICmd(ENABLE);
	while(RCC_GetFlagStatus(RCC_FLAG_HSIRDY)==RESET)
	{
		
	}
	if(1)
	{
//		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
//		FLASH_SetLatency(FLASH_Latency_2);
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//AHBʱ��Ϊϵͳʱ��SYSCLK
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		RCC_PCLK1Config(RCC_HCLK_Div2);//APB1ʱ��ΪHCK/2,����HCLKΪAHBʱ��
		//APB2 ʱ��ΪHCLK
		//����PLLʱ��Դ�ͱ�Ƶϵ��
//		 #define RCC_PLLMul_2                    ((uint32_t)0x00000000)
// #define RCC_PLLMul_3                    ((uint32_t)0x00040000)
// #define RCC_PLLMul_4                    ((uint32_t)0x00080000)
// #define RCC_PLLMul_5                    ((uint32_t)0x000C0000)
// #define RCC_PLLMul_6                    ((uint32_t)0x00100000)
// #define RCC_PLLMul_7                    ((uint32_t)0x00140000)
// #define RCC_PLLMul_8                    ((uint32_t)0x00180000)
// #define RCC_PLLMul_9                    ((uint32_t)0x001C0000)
// #define RCC_PLLMul_10                   ((uint32_t)0x00200000)
// #define RCC_PLLMul_11                   ((uint32_t)0x00240000)
// #define RCC_PLLMul_12                   ((uint32_t)0x00280000)
// #define RCC_PLLMul_13                   ((uint32_t)0x002C0000)
// #define RCC_PLLMul_14                   ((uint32_t)0x00300000)
// #define RCC_PLLMul_15                   ((uint32_t)0x00340000)
// #define RCC_PLLMul_16                   ((uint32_t)0x00380000)
 switch( SYSCLK )
 {
	 case 8 : RCC_PLLMul_Index = RCC_PLLMul_2;break;
	 case 64: RCC_PLLMul_Index = RCC_PLLMul_16;break;
	 default :break;
 };
 
 RCC_PLLConfig(RCC_PLLSource_HSI_Div2,RCC_PLLMul_Index);
		//ʧ�ܻ���ʹ��PLL,�����������ȡ ENABLE����DISABLE
		RCC_PLLCmd(ENABLE);    //���������ϵͳʱ�ӣ�������ܱ�ʧ��
		//�ȴ�ָ����RCC��־λ���óɹ� �ȴ�PLL��ʼ���ɹ�
		
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{
		}
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//����ϵͳʱ�ӣ�SYSCLK�� ����PLLΪϵͳʱ��Դ
	  //0x00: HSI��Ϊϵͳʱ��
		//0x04��HSE��Ϊϵͳʱ��
		//0x08: PLL��Ϊϵͳʱ��
		while(RCC_GetSYSCLKSource() !=0x08)
		{
	      
		}
	}
	//ʱ��������
}

void RSA_Test(void)
{
	#define RSABITLEN  512
	#define RSAENCRYPTLEN (RSABITLEN/4+1)

	uint8_t PainText[]="Hello World";
	uint8_t RSA_Encrypt_Text[RSAENCRYPTLEN]={0};
	uint8_t Modulus[RSAENCRYPTLEN]=
	"a5c3ccc0fcd6a7b8310aab8f177d721abde7b035890a2439555e6a54893626ab1860503aea2bc932f7d91f727a4cb07ad0143047bc6beb42eb9ba37afaa51d61"
	;
	  RSA_Public_Encrypt(PainText,
	                         sizeof(PainText),
														 Modulus,
	                         RSA_Encrypt_Text,
													 RSABITLEN);
  printf("RSA_Encrypt_Text:%s\r\n",RSA_Encrypt_Text);	
}
void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
	/*Ҳ���Կչ�һ���ƽ��п���*/
   printf("Error!\n");
}
