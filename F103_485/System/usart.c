#include "usart.h"

//USART��ʼ������
void USART_Cofig(u32 bound, USART_TypeDef * usartx)									//�������ú���
{
	USART_InitTypeDef USART_InitStructure;											//���ڽṹ��ָ��	
	USART_InitStructure.USART_BaudRate = bound;										//һ������Ϊ9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;						//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;							//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;								//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;					//�շ�ģʽ

	USART_Init(usartx, &USART_InitStructure); 										//��ʼ������
}	






