#define __INIT_C_
#include "includes.h"


int mode=0;

/*-----------------------------------------------------------------------*/
/* 设置单片机的模式和时钟                                                */
/*-----------------------------------------------------------------------*/
void init_modes_and_clock(void) 
{
    ME.MER.R = 0x0000001D;	/* Enable DRUN, RUN0, SAFE, RESET modes */
	/* 设置sysclk */
    //CGM.FMPLL_CR.R = 0x02400100;	/* 8 MHz xtal: Set PLL0 to 64 MHz */
    CGM.FMPLL_CR.R = 0x01280000;	/* 8 MHz xtal: Set PLL0 to 80 MHz */
    //CGM.FMPLL_CR.R = 0x013C0000;	/* 8 MHz xtal: Set PLL0 to 120 MHz */ 
    ME.RUN[0].R = 0x001F0064;	/* RUN0 cfg: 16MHzIRCON,OSC0ON,PLL0ON,syclk=PLL     sysclk选择锁相环时钟 */
    ME.RUNPC[0].R = 0x00000010;	/* Peri. Cfg. 1 settings: only run in RUN0 mode      选择RUN0模式 */
  
	/* PCTL[?] 选择需要时钟模块(默认即可，不用设置) */
	//ME.PCTL[32].R = 0x00;	/* MPC56xxB/P/S ADC 0: select ME.RUNPC[0] */
    ME.PCTL[32].B.DBG_F = 0;
	/* Mode Transition to enter RUN0 mode */
    ME.MCTL.R = 0x40005AF0;	/* Enter RUN0 Mode & Key */
    ME.MCTL.R = 0x4000A50F;	/* Enter RUN0 Mode & Inverted Key */
    
    while (ME.GS.B.S_MTRANS) {}	/* Wait for mode transition to complete 等待模式转换完成 */
    while(ME.GS.B.S_CURRENTMODE != 4) {} /* Verify RUN0 is the current mode 等待选择RUN0模式 */
  
	/* 开peri0、1、2 */
	/* 外设时钟总线 可用于分频 */
	CGM.SC_DC[0].R = 0x84;	/* LIN */
	CGM.SC_DC[1].R = 0x80;	/* FLEXCAN,DSPI */
    CGM.SC_DC[2].R = 0x80;	/* eMIOS,CTU,ADC */
}


/*-----------------------------------------------------------------------*/
/* 禁用看门狗                                                            */
/* 据徐博说应该首先运行                                                  */
/*-----------------------------------------------------------------------*/
void disable_watchdog(void)
{
	SWT.SR.R = 0x0000c520;	/* rite keys to clear soft lock bit */
	SWT.SR.R = 0x0000d928;
	SWT.CR.R = 0x8000010A;	/* Clear watchdog enable (WEN) */
}


/*-----------------------------------------------------------------------*/
/* 初始化all灯                                                         */
/*-----------------------------------------------------------------------*/
void init_led(void)
{
	//停车指示灯
	SIU.PCR[16].R = 0x0203;/* PB0	*///StopLight_1
	SIU.PCR[17].R = 0x0203;/* PB1	*///StopLight_2

	StopLight_1 = 0;
	StopLight_2 = 0;
}


/*-----------------------------------------------------------------------*/
/* 初始化拨码开关                                                         */
/*-----------------------------------------------------------------------*/
void init_DIP(void)
{
	SIU.PCR[48].R=0x0100;	/*PD0 as input*/
	SIU.PCR[49].R=0x0100;	/*PD1 as input*/
	SIU.PCR[50].R=0x0100;	/*PD2 as input*/
	SIU.PCR[51].R=0x0100;	/*PD3 as input*/
}

/*-----------------------------------------------------------------------*/
/* 初始化按键                                                        */
/*-----------------------------------------------------------------------*/
void init_key(void)
{
	SIU.PCR[52].R=0x0100;	/*PD4 as input*/
	SIU.PCR[53].R=0x0100;	/*PD5 as input*/
	SIU.PCR[54].R=0x0100;	/*PD6 as input*/
	SIU.PCR[55].R=0x0100;	/*PD7 as input*/
}
/*-----------------------------------------------------------------------*/
/* 初始化PIT中断                                                         */
/* 10ms                                                                  */
/*-----------------------------------------------------------------------*/
void init_pit_10ms(void)
{
	/* NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 */
	PIT.PITMCR.R = 0x00000001;	/* Enable PIT and configure timers to stop in debug modem */
	PIT.CH[1].LDVAL.R = 800000;	/* 800000==10ms */
	PIT.CH[1].TCTRL.R = 0x00000003;	/* Enable PIT1 interrupt and make PIT active to count */
	INTC_InstallINTCInterruptHandler(PitISR,60,1);	/* PIT 1 interrupt vector with priority 1 */
}

/*-----------------------------------------------------------------------*/
/* 初始化PIT中断                                                         */
/* 1ms                                                                  */
/*-----------------------------------------------------------------------*/
void init_pit_1ms(void)
{
	/* NOTE:  DIVIDER FROM SYSCLK TO PIT ASSUMES DEFAULT DIVIDE BY 1 */
	PIT.PITMCR.R = 0x00000001;	/* Enable PIT and configure timers to stop in debug modem */
	PIT.CH[2].LDVAL.R = 80000;	/* 80000==1ms */
	PIT.CH[2].TCTRL.R = 0x00000003;	/* Enable PIT1 interrupt and make PIT active to count */
	INTC_InstallINTCInterruptHandler(Pit_1ms,61,3);	/* PIT 2 interrupt vector with priority 3 */
}
void disable_pit_1ms(void)
{
	PIT.CH[2].TCTRL.R = 0x00000001;	/* Disable PIT1 interrupt and make PIT active to count */
}

void enable_pit_1ms(void)
{
	PIT.CH[2].TFLG.B.TIF = 1;	/* clear flag */
	PIT.CH[2].TCTRL.R = 0x00000003;	/* Enable PIT1 interrupt and make PIT active to count */
}
/*-----------------------------------------------------------------------*/
/* 初始化eMIOS0                                                          */
/* 初始化三电机                                                      */
/*-----------------------------------------------------------------------*/
void initEMIOS_0MotorAndSteer(void)
{
	/* eMIOS0初始化80MHz分为10MHz */
	EMIOS_0.MCR.B.GPRE= 7;	/* GPRE+1=分频系数；/* Divide 80 MHz sysclk by 7+1 = 8 for 10MHz eMIOS clk */
	EMIOS_0.MCR.B.GPREN = 1;	/* Enable eMIOS clock */
	EMIOS_0.MCR.B.GTBE = 1;   /* Enable global time base */
	EMIOS_0.MCR.B.FRZ = 1;    /* Enable stopping channels when in debug mode */
	
    /* Modulus Up Counter 5kHZ */
    EMIOS_0.CH[16].CCR.B.UCPRE=0;	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[16].CCR.B.UCPEN = 1;	/* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[16].CCR.B.FREN = 1;	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[16].CADR.R = 2000;	/* 设置周期200us 5KHZ */
	EMIOS_0.CH[16].CCR.B.MODE = 0x50;	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[16].CCR.B.BSL = 0x3;	/* Use internal counter */

    /* ROLL1  OPWMB PE1 输出0-2000 */
	EMIOS_0.CH[17].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[17].CCR.B.MODE = 0x60;	/* Mode is OPWM Buffered */
    EMIOS_0.CH[17].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[17].CADR.R = 0;	/* Leading edge when channel counter bus= */
	EMIOS_0.CH[17].CBDR.R = 0;	/* Trailing edge when channel counter bus= */
	SIU.PCR[65].R = 0x0600;	/*[11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	/* ROLL2  OPWMB PE2 输出0-2000 */
	EMIOS_0.CH[18].CCR.B.BSL = 0x1;
	EMIOS_0.CH[18].CCR.B.MODE = 0x60;
    EMIOS_0.CH[18].CCR.B.EDPOL = 1;
	EMIOS_0.CH[18].CADR.R = 0;
	EMIOS_0.CH[18].CBDR.R = 0;
	SIU.PCR[66].R = 0x0600;
    /* ROLL3  OPWMB PE3 输出0-2000 */
	EMIOS_0.CH[19].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[19].CCR.B.MODE = 0x60;	/* Mode is OPWM Buffered */
    EMIOS_0.CH[19].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[19].CADR.R = 0;	/* Leading edge when channel counter bus= */
	EMIOS_0.CH[19].CBDR.R = 0;	/* Trailing edge when channel counter bus= */
	SIU.PCR[67].R = 0x0600;	/*[11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	/* ROLL4  OPWMB PE4 输出0-2000 */
	EMIOS_0.CH[20].CCR.B.BSL = 0x1;
	EMIOS_0.CH[20].CCR.B.MODE = 0x60;
    EMIOS_0.CH[20].CCR.B.EDPOL = 1;
	EMIOS_0.CH[20].CADR.R = 0;
	EMIOS_0.CH[20].CBDR.R = 0;
	SIU.PCR[68].R = 0x0600;

    /* PITCH  OPWMB PE5 输出0-2000 */
	EMIOS_0.CH[21].CCR.B.BSL = 0x1;	/* Use counter bus D (default) */
	EMIOS_0.CH[21].CCR.B.MODE = 0x60;	/* Mode is OPWM Buffered */
    EMIOS_0.CH[21].CCR.B.EDPOL = 1;	/* Polarity-leading edge sets output/trailing clears*/
	EMIOS_0.CH[21].CADR.R = 0;	/* Leading edge when channel counter bus= */
	EMIOS_0.CH[21].CBDR.R = 0;	/* Trailing edge when channel counter bus= */
	SIU.PCR[69].R = 0x0600;	/*[11:10]选择AFx 此处AF1 /* MPC56xxS: Assign EMIOS_0 ch 21 to pad */
	/* PITCH  OPWMB PE6 输出0-2000 */
	EMIOS_0.CH[22].CCR.B.BSL = 0x1;
	EMIOS_0.CH[22].CCR.B.MODE = 0x60;
    EMIOS_0.CH[22].CCR.B.EDPOL = 1;
	EMIOS_0.CH[22].CADR.R = 0;
	EMIOS_0.CH[22].CBDR.R = 0;
	SIU.PCR[70].R = 0x0600;
	
    /* Modulus Up Counter 5kHZ */
    EMIOS_0.CH[0].CCR.B.UCPRE=0;	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[0].CCR.B.UCPEN = 1;	/* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[0].CCR.B.FREN = 1;	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[0].CADR.R = 2000;	/* 设置周期200us 5KHZ */
	EMIOS_0.CH[0].CCR.B.MODE = 0x50;	/* Modulus Counter Buffered (MCB) */
	EMIOS_0.CH[0].CCR.B.BSL = 0x3;	/* Use internal counter */
	
	/* YAW  OPWMB PA3 输出0-2000 */
	EMIOS_0.CH[3].CCR.B.BSL = 0x1;
	EMIOS_0.CH[3].CCR.B.MODE = 0x60;
    EMIOS_0.CH[3].CCR.B.EDPOL = 1;
	EMIOS_0.CH[3].CADR.R = 0;
	EMIOS_0.CH[3].CBDR.R = 0;
	SIU.PCR[3].R = 0x0600;
	/* YAW  OPWMB PA4 输出0-2000 */
	EMIOS_0.CH[4].CCR.B.BSL = 0x1;
	EMIOS_0.CH[4].CCR.B.MODE = 0x60;
    EMIOS_0.CH[4].CCR.B.EDPOL = 1;
	EMIOS_0.CH[4].CADR.R = 0;
	EMIOS_0.CH[4].CBDR.R = 0;
	SIU.PCR[4].R = 0x0600;
	
	/* M NULL  OPWMB PA5 输出0-2000 */
	EMIOS_0.CH[5].CCR.B.BSL = 0x1;
	EMIOS_0.CH[5].CCR.B.MODE = 0x60;
    EMIOS_0.CH[5].CCR.B.EDPOL = 1;
	EMIOS_0.CH[5].CADR.R = 0;
	EMIOS_0.CH[5].CBDR.R = 0;
	SIU.PCR[5].R = 0x0600;
	/* M NULL  OPWMB PA6 输出0-2000 */
	EMIOS_0.CH[6].CCR.B.BSL = 0x1;
	EMIOS_0.CH[6].CCR.B.MODE = 0x60;
    EMIOS_0.CH[6].CCR.B.EDPOL = 1;
	EMIOS_0.CH[6].CADR.R = 0;
	EMIOS_0.CH[6].CBDR.R = 0;
	SIU.PCR[6].R = 0x0600;

}

/*-----------------------------------------------------------------------*/
/* 拨码开关模式选择                                                */
/*                                                              */
/*-----------------------------------------------------------------------*/
void init_choose_mode(void)
{
	mode=switch1*8+switch2*4+switch3*2+switch4;
}


/*----------------------------------------------------------------------*/
/*读取拨码开关选择模式                                            	 */
/*状态OLED显示				*/
/*----------------------------------------------------------------------*/
void read_DIP_mode()
{
	LCD_P8x16Str(0, 6, (BYTE*)"MODE=");
	LCD_PrintoutInt(40, 6, mode);
}
void init_speed_control()
{
	g_f_enable_speed_control = 1;
	LCD_P8x16Str(0, 4, (BYTE*)"S.T=0");
	set_speed_target(0);
}
/*-----------------------------------------------------------------------*/
/* 使能外部中断                                                          */
/* 总开关                                                                */
/*-----------------------------------------------------------------------*/
void enable_irq(void)
{
  INTC.CPR.B.PRI = 0;	/* Single Core: Lower INTC's current priority */
  asm(" wrteei 1");	/* Enable external interrupts */
}

void disable_irq(void)
{
	
}


/*-----------------------------------------------------------------------*/
/* 判断大端                                                              */
/* MPC5604大端 高位在低                                                  */
/*-----------------------------------------------------------------------*/
int is_big_endian()
{
	WORD a = 0x1234;
	BYTE b = *(BYTE *)&a;	/*通过将int强制类型转换成char单字节，通过判断起始存储位置。即等于 取b等于a的低地址部分 */
    
    if(b == 0x12)
	{
    	return 1;
	}
    
	return 0;
}


/*-----------------------------------------------------------------------*/
/* 初始化光电编码器          已测试                                            */
/*-----------------------------------------------------------------------*/
void init_optical_encoder(void)	//PD12模数计数器入口，上升沿
{
	/* OE1计数部分 PA2 */
	EMIOS_0.CH[2].CCR.B.MODE = 0x51;	/* Mode is MCB */
	EMIOS_0.CH[2].CCR.B.BSL = 0x3;	/* Use internal counter */
	EMIOS_0.CH[2].CCR.B.UCPRE=0;	/* Set channel prescaler to divide by 1 */
	EMIOS_0.CH[2].CCR.B.UCPEN = 1;	/* Enable prescaler; uses default divide by 1 */
	EMIOS_0.CH[2].CCR.B.FREN = 0;	/* Freeze channel counting when in debug mode */
	EMIOS_0.CH[2].CCR.B.EDPOL=1;	/* Edge Select rising edge */
	EMIOS_0.CH[2].CADR.R=0xffff;
	/* (WORD)EMIOS_0.CH[15].CCNTR.R 数据寄存器 */
	SIU.PCR[2].R = 0x0500;	/* Initialize pad for eMIOS channel Initialize pad for input */

	/* OE1方向部分PD9 */
	SIU.PCR[57].R = 0x0100;
}

/* 初始化陀螺仪 */
/*
void test_xyz_gyro()
{
	int i=0;
	LCD_P8x16Str(0,2, (BYTE*)"L3G..");
	switch (g_device_NO)
	{
		case WIFI_ADDRESS_CAR_1 :
		case WIFI_ADDRESS_CAR_3 :
		while (1)
		{
			BYTE rev = 0x00;
			
			ReadReg(WHO_AM_I, &rev);
			if (I_AM_L3G4200D == rev)
			{
				g_devices_init_status.L3G4200D_is_OK = 1;
				SetODR(  ODR_100Hz_BW_12_5 );
				SetInt1Filters( LPF2 );
				SetAxis(X_ENABLE | Y_ENABLE | Z_ENABLE);
				SetMode(NORMAL);
				break;
			}
		}
		break;
	
		case WIFI_ADDRESS_CAR_2 :
		case WIFI_ADDRESS_CAR_4 :
		for (i=0; i<5; i++)
		{
			BYTE rev = 0x00;
			
			ReadReg(WHO_AM_I, &rev);
			if (I_AM_L3G4200D == rev)
			{
				g_devices_init_status.L3G4200D_is_OK = 1;
				SetODR(ODR_100Hz_BW_12_5);
				SetAxis(X_ENABLE | Y_ENABLE | Z_ENABLE);
				SetMode(NORMAL);
				break;
			}
		}
		break;
	
	}
	if (g_devices_init_status.L3G4200D_is_OK)
	{
		LCD_P8x16Str(0,2, (BYTE*)"L3G..OK");
	}
	else
	{
		LCD_P8x16Str(0,2, (BYTE*)"L3G..NOK");
	}
}
*/

/*-----------------------------------------------------------------------*/
/* 初始化AD                                                              */
/*-----------------------------------------------------------------------*/
void init_ADC(void)
{
	ADC.MCR.R = 0x20000100;     //Conversion times for 80MHz ADClock  连续模式
	ADC.NCMR[1].R = 0x000000FF; //Select ANS0 ANS1 ANS2 ANS3 inputs for conversion 
								//channel 32~39 sampling enable
	ADC.CTR[1].R = 0x00008606;  //Conversion times for 32MHz ADClock？？ 
	ADC.MCR.B.NSTART=1;       //Trigger normal conversions for ADC0
	SIU.PCR[24].R = 0x2100;     //MPC56xxB: Initialize PB[8] as ANS0 CDR32 	电位器
	SIU.PCR[25].R = 0x2100;     //MPC56xxB: Initialize PB[9] as ANS1 CDR33	预留AD口
	SIU.PCR[26].R = 0x2100;     //MPC56xxB: Initialize PB[10] as ANS2 34	俯仰陀螺仪双参数输入：角度
	SIU.PCR[27].R = 0x2100;     //MPC56xxB: Initialize PB[11] as ANS3 35	俯仰陀螺仪双参数输入：角速度
	SIU.PCR[60].R = 0x2100;     //MPC56xxB: Initialize PD[12] as ANS4 36	横滚陀螺仪双参数输入：角度
	SIU.PCR[61].R = 0x2100;     //MPC56xxB: Initialize PD[13] as ANS5 37	横滚陀螺仪双参数输入：角速度
	SIU.PCR[62].R = 0x2100;     //MPC56xxB: Initialize PD[14] as ANS6 38	航向陀螺仪双参数输入：角度
	SIU.PCR[63].R = 0x2100;     //MPC56xxB: Initialize PD[15] as ANS7 39	航向陀螺仪双参数输入：角速度
}


/*-----------------------------------------------------------------------*/
/* 延时 xus                                                              */
/* 依赖总线80M                                                           */
/*-----------------------------------------------------------------------*/
void delay_us(DWORD us)
{
	volatile int i, j;
	
	for (i = 0; i < us; i++)
	{
		for (j = 0; j < 9; j++) {}
	}
}


/*-----------------------------------------------------------------------*/
/* 延时 xms                                                              */
/* 依赖delay_us()                                                        */
/*-----------------------------------------------------------------------*/
void delay_ms(DWORD ms)
{
	int i;
	
	for (i = 0; i < ms; i++)
	{
		delay_us(1000);
	}
}


/*-----------------------------------------------------------------------*/
/* 初始化并自检                                                          */
/*-----------------------------------------------------------------------*/
void init_all_and_POST(void)
{
	int i = 0;

	disable_watchdog();
	init_modes_and_clock();
	initEMIOS_0MotorAndSteer();
	
	/* PIT：光编读值&速度控制 */
//	init_pit_10ms();
	
	/* PIT：步进电机控制&角度控制标志位 */
	init_pit_1ms();	
	
	
	//init_Stepmotor();		/* 初始化步进电机 */

	init_led();
	init_DIP();				/* 拨码开关 */
	init_key();				/* 按键 */
	init_serial_port_1();	/* BlueTooth */

	init_ADC();				/* 陀螺仪读值*/
	init_optical_encoder();	/* 光编 */

	//init_I2C();
	init_choose_mode();		/* 拨码开关模式选择 */
	
	
	/* 初始化SPI总线 */
	init_DSPI_1();
	
	/* 开启外部总中断 */
	enable_irq();
	
	/* 初始化显示屏 */
	initLCD();

	LCD_DISPLAY();
	LCD_Fill(0xFF);	/* 亮屏 */
	delay_ms(50);
	LCD_Fill(0x00);	/* 黑屏 */
	delay_ms(50);

	/* 初始化陀螺仪 */
//	init_MPU9250();
	
	/* 初始化电子罗盘 */
	init_GY953();

	/* 读取拨码开关模式号 */
	read_DIP_mode();

	delay_ms(1000);
	/* 换屏 */
	LCD_Fill(0x00);

	/* 速度闭环开启及测试 速度=0 */	
	//init_speed_control();
	
	//delay_ms(2000);
	
	/* 换屏 */
	//LCD_Fill(0x00);

}
//

/*-----------------------------------------------------------------------*/
/* 初始SPI总线	                                                        */
/*-----------------------------------------------------------------------*/
void init_DSPI_1(void)
{
	DSPI_1.MCR.R = 0x803f0001;     /* Configure DSPI_1 as master */
	DSPI_1.CTAR[0].R = 0x3E0A7724;	//陀螺仪 用于发送8bits MSB先发,调整极性为1，相位为1，调整波特率为1m/s
	DSPI_1.CTAR[1].R = 0x38087726;  //TF 极性为0，相位为0，baud rate=625k/s
	DSPI_1.CTAR[2].R = 0x3E097725;  //GY953	电子罗盘	MSB先发	一次发送8bit 极性为1，相位为1，baud rate=833kHZ
	DSPI_1.CTAR[3].R = 0x380A7720;	//OLED 极性为0，相位为0，baud rate=8m/s
	DSPI_1.MCR.B.HALT = 0x0;	     /* Exit HALT mode: go from STOPPED to RUNNING state*/


	SIU.PCR[34].R = 0x0604;	//PC2 SCK_1
	SIU.PCR[36].R = 0x0104;	//PC4 SIN_1
	SIU.PCR[37].R = 0x0604;	//PC5 SOUT_1
	SIU.PCR[35].R = 0x0503;	//PC3 CS0_1		9250
	SIU.PCR[74].R = 0x0A04;	//PE10 CS3_1	GY953
	SIU.PCR[75].R = 0x0A04;	//PE11 CS4_1	OLED


//	DSPI_1.RSER.B.TCFRE = 0;	//关闭传输完成中断
}

//

/*-----------------------------------------------------------------------*/
/* 自杀                                                                                  */
/* 在系统初始化出错时一直卡住                                                 */
/* 阻止系统在外设不正确的情况下启动                                        */
/*-----------------------------------------------------------------------*/
void suicide(void)
{
	while (1) { }
}






