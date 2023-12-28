#include "modbus.h"
#include "modbus_crc.h"
#include "rs485.h"

MODBUS modbus;			//Modbus�ṹ�����




/******************* SLAVE MODE ************************/
u16 Reg[] = {
	0x0001,
	0x0002,
	0x0003,
	0x0004,
	0x0005,
	0x0006,
	0x0007,
	0x0008
};	
//ʹ��Modbus485����
static void Enable485(MODBUS* modbus, u8 i){
	u8 j;
	RS485_TX_ENABLE;//���ǿ���485����
	
	for(j=0;j<i;j++)//��������
	{
	  Modbus_Send_Byte(modbus->sendbuf[j]);	
	}
	RS485_RX_ENABLE;//�����ǹر�485����	
}

//modbus��ʼ������
void Modbus_Config(){
	modbus.myadd = SLAVE_ADDR;		//���屾�ӻ���ַΪ0x02;
	modbus.timrun = 0;			//modbus��ʱ��ֹͣ��ʱ
	modbus.salve_add = MASTER_ADDR;	//����Ҫƥ��Ĵӻ���ַ
}

// Modbus 3�Ź����뺯��
// Modbus ������ȡ�Ĵ���ֵ
void Modbus_Func3()
{
  u16 Regadd,Reglen,crc;
	u8 i,j;	
	//�õ�Ҫ��ȡ�Ĵ������׵�ַ
	Regadd = modbus.rcbuf[2]*256+modbus.rcbuf[3];//��ȡ���׵�ַ
	//�õ�Ҫ��ȡ�Ĵ��������ݳ���
	Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];//��ȡ�ļĴ�������
	//���ͻ�Ӧ���ݰ�
	i = 0;
	modbus.sendbuf[i++] = modbus.myadd;      //ID�ţ����ͱ����豸��ַ
	modbus.sendbuf[i++] = 0x03;              //���͹�����
  modbus.sendbuf[i++] = ((Reglen*2)%256);   //�����ֽڸ���
	for(j=0;j<Reglen;j++)                    //��������
	{
		//reg����ǰ����õ�16λ���飨ģ�¼Ĵ�����
	  modbus.sendbuf[i++] = Reg[Regadd+j]/256;//��λ����
		modbus.sendbuf[i++] = Reg[Regadd+j]%256;//��λ����
	}
	crc = Modbus_CRC16(modbus.sendbuf,i);    //����Ҫ�������ݵ�CRC
	modbus.sendbuf[i++] = crc/256;//У��λ��λ
	modbus.sendbuf[i++] = crc%256;//У��λ��λ
	//���ݰ�������
	// ��ʼ����Modbus����
//	Enable485(&modbus,i);
		RS485_TX_ENABLE;//���ǿ���485����
	
	for(j=0;j<i;j++)//��������
	{
	  Modbus_Send_Byte(modbus.sendbuf[j]);	
	}
	RS485_RX_ENABLE;//�����ǹر�485����
}

// Modbus 6�Ź����뺯��
// Modbus ����д��Ĵ���ֵ
void Modbus_Func6()  
{
  u16 Regadd;//��ַ16λ
	u16 val;//ֵ
	u16 i,crc,j;
	i=0;
  Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //�õ�Ҫ�޸ĵĵ�ַ 
	val=modbus.rcbuf[4]*256+modbus.rcbuf[5];     //�޸ĺ��ֵ��Ҫд������ݣ�
	Reg[Regadd]=val;  //�޸ı��豸��Ӧ�ļĴ���
	
	//����Ϊ��Ӧ����
	modbus.sendbuf[i++]=modbus.myadd;//���豸��ַ
  modbus.sendbuf[i++]=0x06;        //������ 
  modbus.sendbuf[i++]=Regadd/256;//д��ĵ�ַ
	modbus.sendbuf[i++]=Regadd%256;
	modbus.sendbuf[i++]=val/256;//д�����ֵ
	modbus.sendbuf[i++]=val%256;
	crc=Modbus_CRC16(modbus.sendbuf,i);//��ȡcrcУ��λ
	modbus.sendbuf[i++]=crc/256;  //crcУ��λ�������
	modbus.sendbuf[i++]=crc%256;
	//���ݷ��Ͱ�������
//	Enable485(&modbus,i);
		RS485_TX_ENABLE;//���ǿ���485����
	
	for(j=0;j<i;j++)//��������
	{
	  Modbus_Send_Byte(modbus.sendbuf[j]);	
	}
	RS485_RX_ENABLE;//�����ǹر�485����
}

//����������Ĵ�������д������
//������0x10ָ�ʮ����16
void Modbus_Func16()
{
		u16 Regadd;//��ַ16λ
		u16 Reglen;
		u16 i,crc,j;
		
		Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //Ҫ�޸����ݵ���ʼ��ַ
		Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];//��ȡ�ļĴ�������
		for(i=0;i<Reglen;i++)//���Ĵ�����д������
		{
			//��������ĵ���λ��ʼ������
			Reg[Regadd+i]=modbus.rcbuf[7+i*2]*256+modbus.rcbuf[8+i*2];//�ԼĴ���һ��д������
		}
		//д��������ϣ���������Ҫ���д���ظ�������
		
		//����Ϊ��Ӧ��������
		//����=���������ǰ6λ+��λ��У��λ
		modbus.sendbuf[0]=modbus.rcbuf[0];//���豸��ַ
		modbus.sendbuf[1]=modbus.rcbuf[1];  //������ 
		modbus.sendbuf[2]=modbus.rcbuf[2];//д��ĵ�ַ
		modbus.sendbuf[3]=modbus.rcbuf[3];
		modbus.sendbuf[4]=modbus.rcbuf[4];
		modbus.sendbuf[5]=modbus.rcbuf[5];
		crc=Modbus_CRC16(modbus.sendbuf,6);//��ȡcrcУ��λ
		modbus.sendbuf[6]=crc/256;  //crcУ��λ�������
		modbus.sendbuf[7]=crc%256;
		//���ݷ��Ͱ�������
//		Enable485(&modbus,i);
			RS485_TX_ENABLE;//���ǿ���485����
	
	for(j=0;j<i;j++)//��������
	{
	  Modbus_Send_Byte(modbus.sendbuf[j]);	
	}
	RS485_RX_ENABLE;//�����ǹر�485����
}


// Modbus�¼�������
void Modbus_Event()
{
	u16 crc,rccrc;//crc�ͽ��յ���crc
	//û���յ����ݰ�
  if(modbus.reflag == 0)  //�������δ����򷵻ؿ�
	{
	   return;
	}
	//�յ����ݰ�(�������)
	//ͨ������������֡����CRC
	//����1�������׵�ַ������2��Ҫ����ĳ��ȣ�����CRCУ��λ����ȫ�㣩
	crc = Modbus_CRC16(&modbus.rcbuf[0],modbus.recount-2); //��ȡCRCУ��λ
	// ��ȡ����֡��CRC
	rccrc = modbus.rcbuf[modbus.recount-2]*256+modbus.rcbuf[modbus.recount-1];//�����ȡ��CRCУ��λ
	//�ȼ��������������
	//rccrc=modbus.rcbuf[modbus.recount-1]|(((u16)modbus.rcbuf[modbus.recount-2])<<8);//��ȡ���յ���CRC
	if(crc == rccrc) //CRC����ɹ� ��ʼ������
	{	
	   if(modbus.rcbuf[0] == modbus.myadd)  // ����ַ�Ƿ�ʱ�Լ��ĵ�ַ
		 {
		   switch(modbus.rcbuf[1])   //����modbus������
			 {
			   case 0:             break;
				 case 1:             break;
				 case 2:             break;
				 case 3:      Modbus_Func3();      break;//���Ƕ�ȡ�Ĵ���������
				 case 4:             break;
				 case 5:             break;
         case 6:      Modbus_Func6();      break;//����д�뵥���Ĵ�������
				 case 7:             break;
				 case 8:             break;
				 case 9:             break;
				 case 16:     Modbus_Func16(); 			break;//д�����Ĵ�������
			 }
		 }
		 else if(modbus.rcbuf[0] == 0) //�㲥��ַ�����Ӧ
		 {
		    
		 }	 
	}	
	 modbus.recount = 0;//���ռ�������
   modbus.reflag = 0; //���ձ�־����
}


















/******************* MASTER MODE ************************/
















