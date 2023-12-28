#ifndef __MODBUS_H
#define __MODBUS_H
#include "stm32f10x.h"


#define SLAVE_ADDR	0x02
#define MASTER_ADDR	0x01


typedef struct{
	/*******Slave Mode***********/
	u8 myadd;			//本设备（从机）地址
	u8 rcbuf[100];		//modbus报文接收缓冲区
	u8 timout;			//modbus数据持续时间
	u8 recount;			//modbus端口接收到的数据个数
	u8 timrun;			//modbus定时器是否计时标志
	u8 reflag;			//modbus一帧数据授受完成标志位
	u8 sendbuf[100];	//modbus报文发送缓冲区
	
	/*******Master Mode***********/
	u8 Host_Txbuf[8];	//modbus发送数组
	u8 salve_add;		//从机设备地址
	u8 Host_send_flag;	//主机设备发送数据完结标志位；
	int Host_Sendtime;	//发送完一帧数据后时间计数
	u8 Host_time_flag;	//发送时间到标志位，=1表示达到发送时间
	u8 Host_End;		//接收数据后处理完结	
}MODBUS;

/************  SLAVE MODE ***************/
extern MODBUS modbus;
extern u16 Reg[];
void Modbus_Config(void);
void Modbus_Func3(void);//读寄存器数据
void Modbus_Func6(void);//往1个寄存器中写入数据
void Modbus_Func16(void);//往多个寄存器中写入数据
void Modbus_Event(void);





#endif
