#include "main/main.h"

char MISI_data[15];
uint8_t ClearRecordandUser(void);
void HSIClock_Init(uint8_t SYSCLK);
void Arc4Test(void);
void RSA_Test(void);

int main()
{ 
  SystemInit();       //系统时钟初始化
//	HSIClock_Init(64);
//  ClearRecord();
  USART1_Config();
  Tim2_Configure();
  AT24CXX_Init();
  delay_init(72);
//	AT24C128_Save(ADMINOPENPWDAD,"123",LOGPWDLEN);//保存登录密码	
//	AT24C128_Save(ADNAMEAD,"Zhu",LOGPWDLEN);//保存登录密码	
//	AT24C128_Save(ADLOGAD,"Zhu",LOGPWDLEN);//保存登录密码	
//	AT24C128_Read(ADMINOPENPWDAD,10);		
//  RSA_Test();	
	while(1)
	{	
	  JSON_Handle();
	}
}



//登录时给我一个消息
//退出登录时给我一个消息
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
//内部高速时钟HSI时钟配置
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
		RCC_HCLKConfig(RCC_SYSCLK_Div1);//AHB时钟为系统时钟SYSCLK
		RCC_PCLK2Config(RCC_HCLK_Div1); 
		RCC_PCLK1Config(RCC_HCLK_Div2);//APB1时钟为HCK/2,其中HCLK为AHB时钟
		//APB2 时钟为HCLK
		//设置PLL时钟源和倍频系数
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
		//失能或者使能PLL,这个参数可以取 ENABLE或者DISABLE
		RCC_PLLCmd(ENABLE);    //如果被用于系统时钟，这个不能被失能
		//等待指定的RCC标志位设置成功 等待PLL初始化成功
		
		while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY)==RESET)
		{
		}
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//设置系统时钟（SYSCLK） 设置PLL为系统时钟源
	  //0x00: HSI作为系统时钟
		//0x04：HSE作为系统时钟
		//0x08: PLL作为系统时钟
		while(RCC_GetSYSCLKSource() !=0x08)
		{
	      
		}
	}
	//时钟配置区
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
	/*也可以空过一个灯进行控制*/
   printf("Error!\n");
}
