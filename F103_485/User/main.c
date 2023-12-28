#include "systick.h"
#include "usart.h"
#include "rs485.h"
#include "modbus.h"
#include "modbus_tim.h"



int main(void)
{
	SysTick_Init();
	Modbus_uart2_init(9600);
	Modbus_TIME3_Init(7200-1,10-1);//定时器初始化参数1是重装载数，参数2是分频系数//1ms中断一次
	Modbus_Config();					//MODBUS初始化--本机做作为从机设备地址，本机要匹配的从机地址
	while(1){
//		Modbus_Event();
//		Delay_ms(100);

	}

}
