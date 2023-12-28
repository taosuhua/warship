#include "modbus.h"
#include "modbus_crc.h"
#include "rs485.h"

MODBUS modbus;			//Modbus结构体变量




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
//使能Modbus485发送
static void Enable485(MODBUS* modbus, u8 i){
	u8 j;
	RS485_TX_ENABLE;//这是开启485发送
	
	for(j=0;j<i;j++)//发送数据
	{
	  Modbus_Send_Byte(modbus->sendbuf[j]);	
	}
	RS485_RX_ENABLE;//这里是关闭485发送	
}

//modbus初始化函数
void Modbus_Config(){
	modbus.myadd = SLAVE_ADDR;		//定义本从机地址为0x02;
	modbus.timrun = 0;			//modbus定时器停止计时
	modbus.salve_add = MASTER_ADDR;	//主机要匹配的从机地址
}

// Modbus 3号功能码函数
// Modbus 主机读取寄存器值
void Modbus_Func3()
{
  u16 Regadd,Reglen,crc;
	u8 i,j;	
	//得到要读取寄存器的首地址
	Regadd = modbus.rcbuf[2]*256+modbus.rcbuf[3];//读取的首地址
	//得到要读取寄存器的数据长度
	Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];//读取的寄存器个数
	//发送回应数据包
	i = 0;
	modbus.sendbuf[i++] = modbus.myadd;      //ID号：发送本机设备地址
	modbus.sendbuf[i++] = 0x03;              //发送功能码
  modbus.sendbuf[i++] = ((Reglen*2)%256);   //返回字节个数
	for(j=0;j<Reglen;j++)                    //返回数据
	{
		//reg是提前定义好的16位数组（模仿寄存器）
	  modbus.sendbuf[i++] = Reg[Regadd+j]/256;//高位数据
		modbus.sendbuf[i++] = Reg[Regadd+j]%256;//低位数据
	}
	crc = Modbus_CRC16(modbus.sendbuf,i);    //计算要返回数据的CRC
	modbus.sendbuf[i++] = crc/256;//校验位高位
	modbus.sendbuf[i++] = crc%256;//校验位低位
	//数据包打包完成
	// 开始返回Modbus数据
//	Enable485(&modbus,i);
		RS485_TX_ENABLE;//这是开启485发送
	
	for(j=0;j<i;j++)//发送数据
	{
	  Modbus_Send_Byte(modbus.sendbuf[j]);	
	}
	RS485_RX_ENABLE;//这里是关闭485发送
}

// Modbus 6号功能码函数
// Modbus 主机写入寄存器值
void Modbus_Func6()  
{
  u16 Regadd;//地址16位
	u16 val;//值
	u16 i,crc,j;
	i=0;
  Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //得到要修改的地址 
	val=modbus.rcbuf[4]*256+modbus.rcbuf[5];     //修改后的值（要写入的数据）
	Reg[Regadd]=val;  //修改本设备相应的寄存器
	
	//以下为回应主机
	modbus.sendbuf[i++]=modbus.myadd;//本设备地址
  modbus.sendbuf[i++]=0x06;        //功能码 
  modbus.sendbuf[i++]=Regadd/256;//写入的地址
	modbus.sendbuf[i++]=Regadd%256;
	modbus.sendbuf[i++]=val/256;//写入的数值
	modbus.sendbuf[i++]=val%256;
	crc=Modbus_CRC16(modbus.sendbuf,i);//获取crc校验位
	modbus.sendbuf[i++]=crc/256;  //crc校验位加入包中
	modbus.sendbuf[i++]=crc%256;
	//数据发送包打包完毕
//	Enable485(&modbus,i);
		RS485_TX_ENABLE;//这是开启485发送
	
	for(j=0;j<i;j++)//发送数据
	{
	  Modbus_Send_Byte(modbus.sendbuf[j]);	
	}
	RS485_RX_ENABLE;//这里是关闭485发送
}

//这是往多个寄存器器中写入数据
//功能码0x10指令即十进制16
void Modbus_Func16()
{
		u16 Regadd;//地址16位
		u16 Reglen;
		u16 i,crc,j;
		
		Regadd=modbus.rcbuf[2]*256+modbus.rcbuf[3];  //要修改内容的起始地址
		Reglen = modbus.rcbuf[4]*256+modbus.rcbuf[5];//读取的寄存器个数
		for(i=0;i<Reglen;i++)//往寄存器中写入数据
		{
			//接收数组的第七位开始是数据
			Reg[Regadd+i]=modbus.rcbuf[7+i*2]*256+modbus.rcbuf[8+i*2];//对寄存器一次写入数据
		}
		//写入数据完毕，接下来需要进行打包回复数据了
		
		//以下为回应主机内容
		//内容=接收数组的前6位+两位的校验位
		modbus.sendbuf[0]=modbus.rcbuf[0];//本设备地址
		modbus.sendbuf[1]=modbus.rcbuf[1];  //功能码 
		modbus.sendbuf[2]=modbus.rcbuf[2];//写入的地址
		modbus.sendbuf[3]=modbus.rcbuf[3];
		modbus.sendbuf[4]=modbus.rcbuf[4];
		modbus.sendbuf[5]=modbus.rcbuf[5];
		crc=Modbus_CRC16(modbus.sendbuf,6);//获取crc校验位
		modbus.sendbuf[6]=crc/256;  //crc校验位加入包中
		modbus.sendbuf[7]=crc%256;
		//数据发送包打包完毕
//		Enable485(&modbus,i);
			RS485_TX_ENABLE;//这是开启485发送
	
	for(j=0;j<i;j++)//发送数据
	{
	  Modbus_Send_Byte(modbus.sendbuf[j]);	
	}
	RS485_RX_ENABLE;//这里是关闭485发送
}


// Modbus事件处理函数
void Modbus_Event()
{
	u16 crc,rccrc;//crc和接收到的crc
	//没有收到数据包
  if(modbus.reflag == 0)  //如果接收未完成则返回空
	{
	   return;
	}
	//收到数据包(接收完成)
	//通过读到的数据帧计算CRC
	//参数1是数组首地址，参数2是要计算的长度（除了CRC校验位其余全算）
	crc = Modbus_CRC16(&modbus.rcbuf[0],modbus.recount-2); //获取CRC校验位
	// 读取数据帧的CRC
	rccrc = modbus.rcbuf[modbus.recount-2]*256+modbus.rcbuf[modbus.recount-1];//计算读取的CRC校验位
	//等价于下面这条语句
	//rccrc=modbus.rcbuf[modbus.recount-1]|(((u16)modbus.rcbuf[modbus.recount-2])<<8);//获取接收到的CRC
	if(crc == rccrc) //CRC检验成功 开始分析包
	{	
	   if(modbus.rcbuf[0] == modbus.myadd)  // 检查地址是否时自己的地址
		 {
		   switch(modbus.rcbuf[1])   //分析modbus功能码
			 {
			   case 0:             break;
				 case 1:             break;
				 case 2:             break;
				 case 3:      Modbus_Func3();      break;//这是读取寄存器的数据
				 case 4:             break;
				 case 5:             break;
         case 6:      Modbus_Func6();      break;//这是写入单个寄存器数据
				 case 7:             break;
				 case 8:             break;
				 case 9:             break;
				 case 16:     Modbus_Func16(); 			break;//写入多个寄存器数据
			 }
		 }
		 else if(modbus.rcbuf[0] == 0) //广播地址不予回应
		 {
		    
		 }	 
	}	
	 modbus.recount = 0;//接收计数清零
   modbus.reflag = 0; //接收标志清零
}


















/******************* MASTER MODE ************************/
















