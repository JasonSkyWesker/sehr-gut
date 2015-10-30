/*
 * analyse.c
 *
 *  Created on: Jul 13, 2015
 *      Author: Paul
 */
#include "includes.h"
#define g (9.8)
#define stime (0.001)		//����ʱ�� ��λs

int32_t Data[6];	//�����ǣ����ٶȼ�����

int32_t xdev;	//���ƫ���ֵ
int32_t ydev;	//���ƫ���ֵ
int32_t zdev;	//���ƫ���ֵ
int32_t xA=400;		//���ƫ���ֵ
float xa=0;		//x�᷽����ٶ�
float xv=0;		//y�᷽����ٶ�
float xs=0;		//z�᷽����ٶ�

void distance(void)
{
	int32_t temp;
	const float range=65536;
	Read_GYRO_ACC(Data);	//��ȡ����������
	
	temp=(Data[3]-xdev)/xA;
	xa=-temp*xA*4*g;	//��x������ٶ��˲�
	xa=xa/range;

	xv=xv+xa*stime;			//��x�����ٶ�
	xs=xs+xv*stime;			//��x�������

	if(xs>=2)				//2�ױ�־
	//	D5=0;
	
	//��ʾ����ʾ
	LCD_Write_Num(80,1,Data[3],5);
//	LCD_Write_Num(45,2,Data[4],5);
//	LCD_Write_Num(45,3,Data[5],5);
	LCD_PrintoutFloat(45, 2, xa);
	LCD_PrintoutFloat(45, 4, xv);
	LCD_PrintoutFloat(45, 6, xs);
	
	delay_ms(1);//ÿ5ms����һ�Σ���stime��Ӧ  ���᲻��������
}

void filter_display(void)
{
	int32_t Data[6];
	int32_t temp;
	int16_t temp1;
	uint8_t Data_H;		
	uint8_t Data_L;
	int32_t xa_1;
	int i=0;
	//Read_GYRO_ACC(Data);
	while(!Reg_Read(ACCEL_XOUT_H,&Data_H)){}
	while(!Reg_Read(ACCEL_XOUT_L,&Data_L)){}
	temp1=Data_H;
	temp1=(temp1<<8)|Data_L;
	Data[3]=temp1;
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(i++);
	serial_port_1_TX(0xAA);
	
	temp=(Data[3]-xdev)/xA;
	xa_1=temp*xA;
	Data_L=xa_1;
	Data_H=xa_1>>8;
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(i++);
	serial_port_1_TX(0xBB);

	i++;
	delay_ms(5);
}

void acc_display(uint32_t* i)	//iΪѭ������,�˲���
{
	int32_t Data[6];
	int32_t temp;
	int16_t temp1;
	uint8_t Data_H;		
	uint8_t Data_L;
	int32_t xa_1;
	uint32_t ii=*i;
	
	while(!Reg_Read(ACCEL_XOUT_H,&Data_H)){}
	while(!Reg_Read(ACCEL_XOUT_L,&Data_L)){}
	temp1=Data_H;
	temp1=(temp1<<8)|Data_L;
	Data[3]=temp1;
	
	temp=(Data[3]-xdev)/xA;
	xa_1=temp*xA+xdev;
	Data_L=xa_1;
	Data_H=xa_1>>8;
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(ii);
	serial_port_1_TX(0xAA);
	
	while(!Reg_Read(ACCEL_YOUT_H,&Data_H)){}
	while(!Reg_Read(ACCEL_YOUT_L,&Data_L)){}
	temp1=Data_H;
	temp1=(temp1<<8)|Data_L;
	Data[4]=temp1;
	
	temp=(Data[4]-ydev)/xA;
	xa_1=temp*xA+ydev;
	Data_L=xa_1;
	Data_H=xa_1>>8;
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(ii);
	serial_port_1_TX(0xBB);
	
	while(!Reg_Read(ACCEL_ZOUT_H,&Data_H)){}
	while(!Reg_Read(ACCEL_ZOUT_L,&Data_L)){}
	temp1=Data_H;
	temp1=(temp1<<8)|Data_L;
	Data[5]=temp1;
	
	temp=(Data[5]-zdev)/xA;
	xa_1=temp*xA+zdev;
	Data_L=xa_1;
	Data_H=xa_1>>8;
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(ii++);
	serial_port_1_TX(0xCC);
	
	
	*i=ii;
}

void temp_analyse(uint32_t* i)
{
	uint8_t Data_H;		
	uint8_t Data_L;
	uint32_t ii=*i;
	
	while(!Reg_Read(ACCEL_XOUT_H,&Data_H)){}
	while(!Reg_Read(ACCEL_XOUT_L,&Data_L)){}
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(ii);
	serial_port_1_TX(0xAA);
	
	while(!Reg_Read(TEMP_OUT_H,&Data_H)){}
	while(!Reg_Read(TEMP_OUT_L,&Data_L)){}
	
	serial_port_1_TX(Data_H);
	serial_port_1_TX(Data_L);
	serial_port_1_TX(ii++);
	serial_port_1_TX(0xBB);
	
	*i=ii;
}

//--------------------------------------------------------
//���ܣ����������������΢�ַ��� dq/dt=0.5*W*q
//��ڲ�������Ԫ����ֵ(����),����T(Ϊ�������ڵİٷ�֮һ),���ٶȾ���W,�Լ������Ԫ��
//����ֵ��q1,���������Ԫ��
//--------------------------------------------------------
void Runge_Kutta(float q0[],float T,float W[],float q1[])
{
	float k1[4],k2[4],k3[4],k4[4],kt[4],ktt[4],y1;
	float q2[4],q3[4];
	float mat[4];
	float *a;		//��һ��ϵ��
	float b;
	int i;
	for(i=0;i<100;i++)
	{
		m_multiply(W,q0,mat,4,4,4,1);
		m_scalar(mat,0.5,k1,4,1);	//��k1
		
		m_scalar(k1,T/2,kt,4,1);
		m_add(kt,q0,ktt,4,1);
		m_multiply(W,ktt,k2,4,4,4,1);//��k2
		
		m_scalar(k2,T/2,kt,4,1);
		m_add(kt,q0,ktt,4,1);
		m_multiply(W,ktt,k3,4,4,4,1);//��k3
		
		m_scalar(k3,T,kt,4,1);
		m_add(kt,q0,ktt,4,1);
		m_multiply(W,ktt,k4,4,4,4,1);//��k4
		
		m_scalar(k2,2,ktt,4,1);
		m_add(ktt,k1,kt,4,1);
		m_scalar(k3,2,ktt,4,1);
		m_add(kt,ktt,kt,4,1);
		m_add(kt,k4,kt,4,1);
		m_scalar(kt,T/6,kt,4,1);
		m_add(kt,q0,q1,4,1);		//���q1
		
		q0=q1;
	}
	
	//��һ��
	MY_sqrt(q1[0]*q1[0]+q1[1]*q1[1]+q1[2]*q1[2]+q1[3]*q1[3],a);
	b=1/(*a);
	m_scalar(q1,b,q1,4,1);
}

void MY_sqrt(float src,float *dst)
{
	float eps=0.00001;
	float x=src/2;
	float y=x*x-src;
	while(abs(y)>eps)
	{
		x-=y/(2*x);
		y=x*x-src;
	}
	*dst=x;
}


float abs(float data)
{
	if (data<0)
		data=0-data;
	return data;
}
