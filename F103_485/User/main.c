#include "systick.h"
#include "usart.h"
#include "rs485.h"
#include "modbus.h"
#include "modbus_tim.h"



int main(void)
{
	SysTick_Init();
	Modbus_uart2_init(9600);
	Modbus_TIME3_Init(7200-1,10-1);//��ʱ����ʼ������1����װ����������2�Ƿ�Ƶϵ��//1ms�ж�һ��
	Modbus_Config();					//MODBUS��ʼ��--��������Ϊ�ӻ��豸��ַ������Ҫƥ��Ĵӻ���ַ
	while(1){
//		Modbus_Event();
//		Delay_ms(100);

	}

}
