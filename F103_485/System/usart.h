#ifndef	__USART_H
#define __USART_H
#include "stm32f10x.h"
#include <stdio.h>

void USART_Cofig(u32 bound, USART_TypeDef * usartx);
void USART_SendByte(USART_TypeDef* pUSARTx, uint8_t data);				//���͵��ֽ�
void USART_SendWord(USART_TypeDef* pUSARTx, uint16_t data);				//���͵���
void USART_SendArray(USART_TypeDef* pUSARTx, uint8_t *array, uint8_t num);	//�����ֽ�����
void USART_SendString(USART_TypeDef* pUSARTx, char *str);				//�����ַ���
int fputc(int ch, FILE *f);												//�ض���fputc
int fgetc(FILE *f);														//�ض���fgetc









#endif
