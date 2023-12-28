#ifndef	__USART_H
#define __USART_H
#include "stm32f10x.h"
#include <stdio.h>

void USART_Cofig(u32 bound, USART_TypeDef * usartx);
void USART_SendByte(USART_TypeDef* pUSARTx, uint8_t data);				//发送单字节
void USART_SendWord(USART_TypeDef* pUSARTx, uint16_t data);				//发送单字
void USART_SendArray(USART_TypeDef* pUSARTx, uint8_t *array, uint8_t num);	//发送字节数组
void USART_SendString(USART_TypeDef* pUSARTx, char *str);				//发送字符串
int fputc(int ch, FILE *f);												//重定义fputc
int fgetc(FILE *f);														//重定义fgetc









#endif
