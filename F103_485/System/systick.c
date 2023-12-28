#include "systick.h"

static __IO u32 TimingDelay;
static u8  fac_us=0;//us延时倍乘数
static u16 fac_ms=0;//ms延时倍乘数

//启动系统滴答定时器 SysTick
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms中断一次
	 * SystemFrequency / 100000	 10us中断一次
	 * SystemFrequency / 1000000 1us中断一次
	 */
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);	//选择外部时钟  HCLK/8
	fac_us=SystemCoreClock/8000000;	//为系统时钟的1/8 
	fac_ms=(u16)fac_us*1000;//非ucos下,代表每个ms需要的systick时钟数   
	
}


/********us延时程序，1us为一个单位**********
**************************************/
void Delay_us( u32 nus)
{ 
	u32 temp;	    	 
	SysTick->LOAD=nus*fac_us; //时间加载	  		 
	SysTick->VAL=0x00;        //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数	 
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	 
}



/*****ms延时程序,1ms为一个单位**********
************************************/
void Delay_ms(u16 nms)
{ 
	u32 temp;		   
	SysTick->LOAD=(u32)nms*fac_ms;//时间加载(SysTick->LOAD为24bit)
	SysTick->VAL =0x00;           //清空计数器
	SysTick->CTRL|=SysTick_CTRL_ENABLE_Msk ;          //开始倒数  
	do
	{
		temp=SysTick->CTRL;
	}
	while(temp&0x01&&!(temp&(1<<16)));//等待时间到达   
	SysTick->CTRL&=~SysTick_CTRL_ENABLE_Msk;       //关闭计数器
	SysTick->VAL =0X00;       //清空计数器	
}



/*************1s延时程序**************
************************************/
void Delay_s(unsigned int ms)
{ 
	unsigned char i;
	for(i=0;i<ms;ms--)
		{
		Delay_ms(1000);
		}
}



/**************获取节拍程序************
***********************************/
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{ 
		TimingDelay--;
	}
}




