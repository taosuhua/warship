#include "rs485.h"
#include "systick.h"
#include "nvic.h"
#include "usart.h"
#include "modbus.h"


//定时器中定义了Rx_Data_len
//PD7控制485的发送和接收使能
//rs485初始化完毕一般配置为接收使能（正常状态处于接收状态，只有发送的时候才使能发送）


//modbus串口发送一个字节数据
void Modbus_Send_Byte(u8 Modbus_byte)
{
	USART_SendData(USART2,Modbus_byte);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	USART_ClearFlag(USART2, USART_FLAG_TC); 
}

//485串口初始化
//初始化IO 串口2 
//bound:波特率
void Modbus_uart2_init(u32 bound){
    //GPIO端口设置
		GPIO_InitTypeDef GPIO_InitStructure;					//GPIO结构体指针
		USART_InitTypeDef USART_InitStructure;//串口结构体指针
		NVIC_InitTypeDef NVIC_InitStructure;//中断分组结构体指针
	//1、使能串口时钟，串口引脚时钟 串口2挂载到APB1上
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//使能USART2时钟
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD,ENABLE);//使能串口时钟和收发使能时钟
	//2、复位串口	
		USART_DeInit(USART2);  //复位串口1
	
	//3、发送接收引脚的设置
		//USART2_TX   PA.2（由图 可知设置为推挽复用输出）
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
		GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
	   
		//USART2_RX	  PA.3（有图可知浮空输入）
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
		GPIO_Init(GPIOA, &GPIO_InitStructure);  //初始化PA3
		
		//485收发控制引脚PD7
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //PD.7
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//普通的推挽输出
		GPIO_Init(GPIOD, &GPIO_InitStructure); //初始化PD7

	//4、USART 初始化设置
		USART_Cofig(bound,USART2);
 
	//5、Usart2 NVIC 配置
		NVIC_Config(4);
   
	//6、开启接收数据中断
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断
		
	//7、使能串口
		USART_Cmd(USART2, ENABLE);                    //使能串口 
		RS485_RX_ENABLE;								//使能接收引脚（常态下处于接收状态）
}


//modbus串口中断服务程序
void USART2_IRQHandler(void)                
{
  u8 st,Res;
	st = USART_GetITStatus(USART2, USART_IT_RXNE);
	if(st == SET)//接收中断
	{
		Res =USART_ReceiveData(USART2);	//读取接收到的数据
//	  USART_SendData(USART1, Res);//接受到数据之后返回给串口1
	 if( modbus.reflag==1)  //有数据包正在处理
	  {
		   return ;
		}		
	  modbus.rcbuf[modbus.recount++] = Res;
		modbus.timout = 0;
		if(modbus.recount == 1)  //已经收到了第二个字符数据
		{
		  modbus.timrun = 1;  //开启modbus定时器计时
		}
	}	
} 
