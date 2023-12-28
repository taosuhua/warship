#include "modbus_tim.h"
#include "modbus.h"

u8 sec_flag=0;//sec_flag=1ʱ��������
//int Host_Sendtime=0;
void Modbus_TIME3_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStruct;//�жϷ���ṹ��
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;//��ʱ�����Ի��ṹ��
	//1��ʹ�ܶ�ʱ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);//ʹ�ܶ�ʱ��3ʱ��
	//2����ʼ����ʱ��
	TIM_TimeBaseInitStruct.TIM_Period=arr;//�Զ�װ��ֵ(��ڲ���)
	TIM_TimeBaseInitStruct.TIM_Prescaler=psc;//Ԥ��Ƶϵ������ڲ�����
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up ;//���ϼ���
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1 ;
	TIM_TimeBaseInit( TIM3, &TIM_TimeBaseInitStruct);//����2�ǽṹ������
	
	//3��ʹ�ܶ�ʱ���ж�
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//Ҫʹ�ܶ�ʱ���ĸ����ж�
	
	//4���ж����ȼ�����
	NVIC_InitStruct.NVIC_IRQChannel= TIM3_IRQn ;//ͨ����f10x.h�ļ��п����ҵ�
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	
	//5��ʹ�ܶ�ʱ��
	 TIM_Cmd(TIM3, ENABLE);//ʹ�ܶ�ʱ��

}


// Modbus ��ʱ���жϺ��� 1ms�ж�һ��
//void Modbus_TIM_IRQHandler(void)  
void TIM3_IRQHandler(void)   //TIM3�ж�
{
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //���ָ����TIM�жϷ������:TIM �ж�Դ 
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //���TIMx���жϴ�����λ:TIM �ж�Դ 
		if(modbus.timrun != 0)//����ʱ�䣡=0����
		 {
		  modbus.timout++;
		  if(modbus.timout >=8)
		  {
		   modbus.timrun = 0;
			 modbus.reflag = 1;//�����������
		  }
			
		 }
		 modbus.Host_Sendtime++;//��������һ֡���ʱ�����
		 if(modbus.Host_Sendtime>1000)//���뷢����һ֡����1s��
			{
				//1sʱ�䵽
				modbus.Host_time_flag=1;//�������ݱ�־λ��1
				
			}
	}
}


