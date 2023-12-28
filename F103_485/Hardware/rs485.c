#include "rs485.h"
#include "systick.h"
#include "nvic.h"
#include "usart.h"
#include "modbus.h"


//��ʱ���ж�����Rx_Data_len
//PD7����485�ķ��ͺͽ���ʹ��
//rs485��ʼ�����һ������Ϊ����ʹ�ܣ�����״̬���ڽ���״̬��ֻ�з��͵�ʱ���ʹ�ܷ��ͣ�


//modbus���ڷ���һ���ֽ�����
void Modbus_Send_Byte(u8 Modbus_byte)
{
	USART_SendData(USART2,Modbus_byte);
	while(USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET);
	USART_ClearFlag(USART2, USART_FLAG_TC); 
}

//485���ڳ�ʼ��
//��ʼ��IO ����2 
//bound:������
void Modbus_uart2_init(u32 bound){
    //GPIO�˿�����
		GPIO_InitTypeDef GPIO_InitStructure;					//GPIO�ṹ��ָ��
		USART_InitTypeDef USART_InitStructure;//���ڽṹ��ָ��
		NVIC_InitTypeDef NVIC_InitStructure;//�жϷ���ṹ��ָ��
	//1��ʹ�ܴ���ʱ�ӣ���������ʱ�� ����2���ص�APB1��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART2ʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOD,ENABLE);//ʹ�ܴ���ʱ�Ӻ��շ�ʹ��ʱ��
	//2����λ����	
		USART_DeInit(USART2);  //��λ����1
	
	//3�����ͽ������ŵ�����
		//USART2_TX   PA.2����ͼ ��֪����Ϊ���츴�������
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure); //��ʼ��PA2
	   
		//USART2_RX	  PA.3����ͼ��֪�������룩
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);  //��ʼ��PA3
		
		//485�շ���������PD7
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; //PD.7
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	//��ͨ���������
		GPIO_Init(GPIOD, &GPIO_InitStructure); //��ʼ��PD7

	//4��USART ��ʼ������
		USART_Cofig(bound,USART2);
 
	//5��Usart2 NVIC ����
		NVIC_Config(4);
   
	//6���������������ж�
		USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�����ж�
		
	//7��ʹ�ܴ���
		USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ��� 
		RS485_RX_ENABLE;								//ʹ�ܽ������ţ���̬�´��ڽ���״̬��
}


//modbus�����жϷ������
void USART2_IRQHandler(void)                
{
  u8 st,Res;
	st = USART_GetITStatus(USART2, USART_IT_RXNE);
	if(st == SET)//�����ж�
	{
		Res =USART_ReceiveData(USART2);	//��ȡ���յ�������
//	  USART_SendData(USART1, Res);//���ܵ�����֮�󷵻ظ�����1
	 if( modbus.reflag==1)  //�����ݰ����ڴ���
	  {
		   return ;
		}		
	  modbus.rcbuf[modbus.recount++] = Res;
		modbus.timout = 0;
		if(modbus.recount == 1)  //�Ѿ��յ��˵ڶ����ַ�����
		{
		  modbus.timrun = 1;  //����modbus��ʱ����ʱ
		}
	}	
} 
