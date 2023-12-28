#include "nvic.h"

/*
中断分配说明，优先级由高向低排
顺序       类型            作用                   抢占优先级      响应优先级
 1         EXTI1        编码器Z相归0                 0                 0
 2         TIM4         定时器的编码器模式             1                 1
 3         USART1       串口通信中断                  2                 0
 4         USART2       RS485 Modbus					 2                 1
*/

//1.-----Z信号归零  PA1
void PA_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure1)
{
  
	NVIC_InitStructure1.NVIC_IRQChannel = EXTI1_IRQn;
	NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure1);
}


//2----定时器的编码器接口模式,对编码器进行计数
void TIM4_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure2)
{
	NVIC_InitStructure2.NVIC_IRQChannel = TIM4_IRQn; 
	NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure2);

}


//3----串口1通信
void USART1_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure3)
{
	NVIC_InitStructure3.NVIC_IRQChannel = USART1_IRQn; 
	NVIC_InitStructure3.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure3.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure3.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure3);
}

//4----USART2通信，用于RS485 Modbus通讯
void USART2_NVIC_Config(NVIC_InitTypeDef NVIC_InitStructure4)
{
	NVIC_InitStructure4.NVIC_IRQChannel = USART2_IRQn; 
	NVIC_InitStructure4.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure4.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure4.NVIC_IRQChannelCmd = ENABLE; 
	NVIC_Init(&NVIC_InitStructure4);
}


void NVIC_Config(u8 Interrupt_flag)
{

  	NVIC_InitTypeDef   NVIC_InitStructure;
	/* 配置中断使用组合  抢占式3位(0-7)，响应式1位(0-1) */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

	if(Interrupt_flag == 1)
	{
		/*外部中断1，Z相归零----优先级最高 0 0*/
		PA_NVIC_Config(NVIC_InitStructure);
	}
	
	if(Interrupt_flag == 2)
	{	
		/*TIM4中断，编码器解码----优先级 1  1*/
		TIM4_NVIC_Config(NVIC_InitStructure);
	}
	if(Interrupt_flag == 3)
	{
		/* USART1中断，数据接收----优先级 2  1*/
		USART1_NVIC_Config(NVIC_InitStructure);
	}
	if(Interrupt_flag == 4)
	{
		/* USART2中断，数据接收----优先级 2  2*/
		USART2_NVIC_Config(NVIC_InitStructure);
	}
}




