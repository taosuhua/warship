#ifndef	__RS485_H
#define __RS485_H
#include "stm32f10x.h"


#define RS485_RX_ENABLE GPIO_ResetBits(GPIOD,GPIO_Pin_7) //����ʹ�ܡ��͵�ƽ��Ч
#define RS485_TX_ENABLE GPIO_SetBits(GPIOD,GPIO_Pin_7) //����ʹ�ܡ��ߵ�ƽ��Ч

void Modbus_uart2_init(u32 bound);//��ʼ������
void Modbus_Send_Byte(u8 Modbus_byte);//����һ���ֽں���


#endif
