#ifndef __MODBUS_H
#define __MODBUS_H
#include "stm32f10x.h"


#define SLAVE_ADDR	0x02
#define MASTER_ADDR	0x01


typedef struct{
	/*******Slave Mode***********/
	u8 myadd;			//���豸���ӻ�����ַ
	u8 rcbuf[100];		//modbus���Ľ��ջ�����
	u8 timout;			//modbus���ݳ���ʱ��
	u8 recount;			//modbus�˿ڽ��յ������ݸ���
	u8 timrun;			//modbus��ʱ���Ƿ��ʱ��־
	u8 reflag;			//modbusһ֡����������ɱ�־λ
	u8 sendbuf[100];	//modbus���ķ��ͻ�����
	
	/*******Master Mode***********/
	u8 Host_Txbuf[8];	//modbus��������
	u8 salve_add;		//�ӻ��豸��ַ
	u8 Host_send_flag;	//�����豸������������־λ��
	int Host_Sendtime;	//������һ֡���ݺ�ʱ�����
	u8 Host_time_flag;	//����ʱ�䵽��־λ��=1��ʾ�ﵽ����ʱ��
	u8 Host_End;		//�������ݺ������	
}MODBUS;

/************  SLAVE MODE ***************/
extern MODBUS modbus;
extern u16 Reg[];
void Modbus_Config(void);
void Modbus_Func3(void);//���Ĵ�������
void Modbus_Func6(void);//��1���Ĵ�����д������
void Modbus_Func16(void);//������Ĵ�����д������
void Modbus_Event(void);





#endif
