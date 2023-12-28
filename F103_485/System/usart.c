#include "usart.h"

//USART初始化配置
void USART_Cofig(u32 bound, USART_TypeDef * usartx)									//串口配置函数
{
	USART_InitTypeDef USART_InitStructure;											//串口结构体指针	
	USART_InitStructure.USART_BaudRate = bound;										//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;								//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//收发模式

	USART_Init(usartx, &USART_InitStructure); 										//初始化串口
}	






