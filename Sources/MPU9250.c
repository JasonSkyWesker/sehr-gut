#define __MPU9250_C_
#include "includes.h"

#if 0	//老版
#define setting 0x08040000
#endif
#if 1
#define setting 0x08000000
#endif

void init_MPU9250(void)
{
	Reg_Write(PWR_MGMT_1,0x80);		//reset
	Reg_Write(PWR_MGMT_1,0x01);		//select clock
	Reg_Write(PWR_MGMT_2,0x00);		//enable Acc&Gyro
	Reg_Write(CONFIG,0x06);			//disable FSYNC;
	Reg_Write(GYRO_CONFIG,0x00);	//±250dps	DLPF bandwidth 3600, delay 0.17 Fs 8kHz
	Reg_Write(ACCEL_CONFIG,0x00);	//±2g
	Reg_Write(ACCEL_CONFIG2,0x00);	//Acc DLPF
	Reg_Write(SMPLRT_DIV,0x07);
	//Reg_Write(FIFO_EN,0x78);		//enable gyro&Acc FIFO
	Reg_Write(USER_CTRL,0x10);
	
}

int Reg_Read(uint8_t reg,uint8_t* Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	
	tmp_tx = 0x80000000|setting|reg|0x80;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;

	tmp_tx = setting|0xff;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	*Data=tmp_rx;
	return 1;
}

int Reg_Write(uint8_t reg,uint8_t Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	
	tmp_tx = 0x80000000|setting|(reg&0x7f);
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	tmp_tx = setting|Data;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	return 1;
}

int Read_GYRO_ACC(int32_t *Data)	//读取角速度与加速度,依次是x,y,z角速度与x,y,z加速度
{
	uint8_t Data_H=0x00;
	uint8_t Data_L=0x00;
	int16_t temp=0x0000;
	
	while(!Reg_Read(GYRO_XOUT_H,&Data_H)){}
	while(!Reg_Read(GYRO_XOUT_L,&Data_L)){}
	temp=Data_H;
	temp=(temp<<8)|Data_L;
	Data[0]=temp;
	
	temp=0x00000000;
	while(!Reg_Read(GYRO_YOUT_H,&Data_H)){}
	while(!Reg_Read(GYRO_YOUT_L,&Data_L)){}
	temp=Data_H;
	temp=(temp<<8)|Data_L;
	Data[1]=temp;
	
	temp=0x00000000;
	while(!Reg_Read(GYRO_ZOUT_H,&Data_H)){}
	while(!Reg_Read(GYRO_ZOUT_L,&Data_L)){}
	temp=Data_H;
	temp=(temp<<8)|Data_L;
	Data[2]=temp;
	
	temp=0x00000000;
	while(!Reg_Read(ACCEL_XOUT_H,&Data_H)){}
	while(!Reg_Read(ACCEL_XOUT_L,&Data_L)){}
	temp=Data_H;
	temp=(temp<<8)|Data_L;
	Data[3]=temp;
	
	temp=0x00000000;
	while(!Reg_Read(ACCEL_YOUT_H,&Data_H)){}
	while(!Reg_Read(ACCEL_YOUT_L,&Data_L)){}
	temp=Data_H;
	temp=(temp<<8)|Data_L;
	Data[4]=temp;
	
	temp=0x00000000;
	while(!Reg_Read(ACCEL_ZOUT_H,&Data_H)){}
	while(!Reg_Read(ACCEL_ZOUT_L,&Data_L)){}
	temp=Data_H;
	temp=(temp<<8)|Data_L;
	Data[5]=temp;
	
	return 1;
}

void deviation_adjust_accx(int32_t *xdev,int32_t *ydev,int32_t *zdev /*, int32_t *xA*/)		//x方向加速度零位偏差，现暂定xA为500
{
	int i;
	int32_t Data[6];
	int32_t Xdev=0;
	int32_t Ydev=0;
	int32_t Zdev=0;
//	int32_t max;
//	int32_t min;
	for(i=0;i<800;i++)			//取样800次，与后面求平均值要对应
	{
		Read_GYRO_ACC(Data);
		Xdev=Xdev+Data[3];
		Ydev=Ydev+Data[4];
		Zdev=Zdev+Data[5];
//		if(i==0)
//		{
//			max=Data[3];
//			min=Data[3];
//		}
//		else
//		{
//			if(Data[3]>max)
//				max=Data[3];
//			if(Data[3]<min)
//				min=Data[3];
//		}
		delay_ms(5);
		
	}
	*xdev=Xdev/800;	
	*ydev=Ydev/800;
	*zdev=Zdev/800;
//	*xA=(max-*xdev)>(*xdev-min)?(max-*xdev):(*xdev-min);
}

void Read_Temp(int32_t* Temp)
{
	uint8_t Temp_H=0x00;
	uint8_t Temp_L=0x00;
	int16_t temp=0x0000;
	
	while(!Reg_Read(TEMP_OUT_H,&Temp_H)){}
	while(!Reg_Read(TEMP_OUT_L,&Temp_L)){}
	
	temp=Temp_H;
	temp=(temp<<8)|Temp_L;
	*Temp=temp;
}



