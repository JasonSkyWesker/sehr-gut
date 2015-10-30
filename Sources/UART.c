#define __UART_C_
#include "includes.h"



int g_serial_port_1_f = 0;
BYTE g_serial_port_1_data;

/*-----------------------------------------------------------------------*/
/* 串口1初始化                                                           */
/* 分配给Bluetooth模块使用                                               */
/*-----------------------------------------------------------------------*/
void init_serial_port_1(void) 
{
	LINFLEX_1.LINCR1.B.INIT=1;	//进入初始化模式
	LINFLEX_1.LINCR1.R=0x00000015;
//#ifndef PERIPH_SET_1_CLK_16M
//    LINFLEX_1.LINFBRR.B.DIV_F =13;
//	LINFLEX_1.LINIBRR.B.DIV_M= 520; //57600:86&13  9600:520&13
//#else
    LINFLEX_1.LINFBRR.B.DIV_F = 1;
	LINFLEX_1.LINIBRR.B.DIV_M= 26; 	//38400:26&1	57600:17&6
//#endif
//    LINFLEX_1.LINFBRR.B.DIV_F = 3;	//小数部分先写
//	LINFLEX_1.LINIBRR.B.DIV_M= 104; 	//9600

    LINFLEX_1.UARTCR.B.UART=1;
	LINFLEX_1.UARTCR.R=0x00000033;
	
	LINFLEX_1.LINIER.B.DRIE=1;
	
	LINFLEX_1.LINCR1.B.INIT=0;	//退出初始化模式
	
	SIU.PCR[38].R = 0x0400;	/* MPC56xxB: Configure port C6 as LIN1TX */
    SIU.PCR[39].R = 0x0103;	/* MPC56xxB: Configure port C7 as LIN1RX */
    
    INTC_InstallINTCInterruptHandler(intc_serial_port_1_RX, 99, 5);
}

#if 1
void serial_port_1_TX(unsigned char data)
{
	LINFLEX_1.LINIER.B.DRIE=0;	//关中断
	LINFLEX_1.BDRL.B.DATA0=data;	//发送语句
	while(!LINFLEX_1.UARTSR.B.DTF){}	//等待数据发送完成
	LINFLEX_1.UARTSR.B.DTF=1;	//清空标志位
	LINFLEX_1.LINIER.B.DRIE=1;	//开中断
}
#endif

void serial_port_1_TX_array(const BYTE data[], BYTE n)
{
	int i = 0;
	
	for (i=0; i<n; i++)
	{
		serial_port_1_TX(data[i]);
	}
}


void intc_serial_port_1_RX()
{
	BYTE rev_ch;
	while(!LINFLEX_1.UARTSR.B.DRF){}
	rev_ch = (BYTE)LINFLEX_1.BDRM.B.DATA4;
	g_serial_port_1_f = 1;
	g_serial_port_1_data = rev_ch;
	rev_remote_frame_2(rev_ch);
	LINFLEX_1.UARTSR.B.RMB=1;		//Release Message Buffer !!!
	LINFLEX_1.UARTSR.B.DRF=1;
}



/*-----------------------------------------------------------------------*/
/* 串口3初始化                                                           */
/* 未分配                                                                */
/*-----------------------------------------------------------------------*/
void init_serial_port_3(void)
{
	LINFLEX_3.LINCR1.B.INIT=1;
	LINFLEX_3.LINCR1.R=0x00000011; 
	LINFLEX_3.LINIER.B.DRIE=1;
#ifndef PERIPH_SET_1_CLK_16M
    LINFLEX_3.LINFBRR.B.DIV_F = 83;
	LINFLEX_3.LINIBRR.B.DIV_M= 520;
#else
    LINFLEX_3.LINFBRR.B.DIV_F = 3;
	LINFLEX_3.LINIBRR.B.DIV_M= 104;
#endif
    LINFLEX_3.UARTCR.B.UART=1;
	LINFLEX_3.UARTCR.R=0x00000033;
	LINFLEX_3.LINCR1.B.INIT=0;
	
	SIU.PCR[74].R = 0x0400;	//LINFLEX_3 TX PE10
    SIU.PCR[75].R = 0x0103;	//LINFLEX_3 RX PE11 涉及到引脚复用
  	SIU.PSMI[31].R = 1;
  	INTC_InstallINTCInterruptHandler(intc_serial_port_3_RX, 122, 5); 
}


void serial_port_3_TX(BYTE data)
{
	LINFLEX_3.BDRL.B.DATA0 = data;       //发送语句
	while(!LINFLEX_3.UARTSR.B.DTF){}
	LINFLEX_3.UARTSR.B.DTF = 1;
}


void serial_port_3_TX_array(const BYTE data[], WORD n)
{
	int i = 0;
	
	for (i=0; i<n; i++)
	{
		serial_port_3_TX(data[i]);
	}
}


void intc_serial_port_3_RX(void)
{
	BYTE rev_ch;
	
	while(!LINFLEX_3.UARTSR.B.DRF){}
	rev_ch = (BYTE)LINFLEX_3.BDRM.B.DATA4;
	LINFLEX_3.UARTSR.B.DRF = 1;
}
