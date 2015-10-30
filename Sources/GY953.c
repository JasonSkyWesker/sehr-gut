#define __GY953_C_
#include "includes.h"

uint8_t GY953_Data[41];


/*------------------------------------------------------------------------------*/
/* ��������GY953��ʼ��
/*------------------------------------------------------------------------------*/
void init_GY953(void)
{
	GY953_Write(SET_A,0x7D);		//�����ų��� ������ ���ٶȼ� �����������200HZ
	GY953_Write(CONTROL_B,0x11);		//�Լ� У׼ ��λ��1�ɻָ���������
	GY953_Write(STATE_D,0x0D);		//����ģ�龫��
}

/*------------------------------------------------------------------------------*/
/* ��ȡ����У׼����
/* �����ݸ��·���1 �������ݷ���0 
/* Data[0]���ٶȼ�	Data[1]���ݼ�	Data[2]�ų��� Data[3]�����������	(��0~4��)
/*------------------------------------------------------------------------------*/
int Read_Precision(BYTE* Data)
{
	BYTE state;
	BYTE set;
	while(!GY953_Read(STATE_C,&state)){};
	while(!GY953_Read(SET_A,&set)){};
	if(state>>7)
	{
		Data[0]=state&0x30;
		Data[0]>>=4;	
		Data[1]=state&0x0C;
		Data[1]>>=2;
		Data[2]=state&0x03;
		Data[3]=set&0x07;
		return 1;
	}
	else
		return 0;
}

/*------------------------------------------------------------------------------*/
/* ��ȡȫ������
/* �����ݸ��·���1 �������ݷ���0 
/* GY953 Data[41]
/* ���ٶ�Data[2~7]���ٶ� Data[8~9]X[10~11]Y[12~13]Z��
/* Data[��20~21��]����ǣ� Data[22~23]�����ǣ�Data[24~25]����ǣ�Data[35]���ȣ�
/* Data[36~39]���ٶȼ�6�����ݼĴ��������ۼӺ� �������ǡ������ơ�ŷ����
/*------------------------------------------------------------------------------*/
int Read_GYalldata(BYTE* Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	uint8_t reg=0x01;
	int i=0;
	
	tmp_tx = 0xA8080000|(reg&0x3F)|0xC0;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	for(i=0;i<41;i++)
	{
		tmp_tx = 0x28080000|0xff;
		DSPI_1.PUSHR.R = tmp_tx;
		while(!DSPI_1.SR.B.TCF){}
		tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
		DSPI_1.SR.B.TCF = 1;
		
		Data[i]=tmp_rx;	
		reg++;//?
	}

	return 1;
}
/*------------------------------------------------------------------------------*/
/* GY953�Ĵ���һ�ζ�ȡ һ��8bit
/* �ɹ�����1 
/*------------------------------------------------------------------------------*/
int GY953_Read(uint8_t reg,uint8_t* Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	
	tmp_tx = 0xA8080000|(reg&0x3F)|0x80;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;

	tmp_tx = 0x28080000|0xff;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	*Data=tmp_rx;
	return 1;
}


/*------------------------------------------------------------------------------*/
/* GY953�Ĵ���д�� һ��8bit
/* �ɹ�����1 
/*------------------------------------------------------------------------------*/
int GY953_Write(uint8_t reg,uint8_t Data)
{
	uint32_t tmp_tx = 0x00000000;
	uint8_t tmp_rx;
	
	tmp_tx = 0xA8080000|(reg&0x3F)|0x40;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	tmp_tx = 0x28080000|Data;
	DSPI_1.PUSHR.R = tmp_tx;
	while(!DSPI_1.SR.B.TCF){}
	tmp_rx = (uint8_t)DSPI_1.POPR.B.RXDATA;
	DSPI_1.SR.B.TCF = 1;
	
	return 1;
}
int GY953_READ_ACC_GYRO(int16_t *ax,int16_t *ay,int16_t *az,int16_t *gx,int16_t *gy,int16_t *gz)
{
	uint8_t ax_H,ax_L,ay_H,ay_L,az_H,az_L,gx_H,gx_L,gy_H,gy_L,gz_H,gz_L;
	
	GY953_Read(ACC_X_H,&ax_H);
	GY953_Read(ACC_X_L,&ax_L);
	*ax=ax_H;
	*ax=(*ax<<8)|ax_L;

	
	GY953_Read(ACC_Y_H,&ay_H);
	GY953_Read(ACC_Y_L,&ay_L);
	*ay=ay_H;
	*ay=(*ay<<8)|ay_L;

	
	GY953_Read(ACC_Z_H,&az_H);
	GY953_Read(ACC_Z_L,&az_L);
	*az=az_H;
	*az=(*az<<8)|az_L;


	GY953_Read(GYRO_X_H,&gx_H);
	GY953_Read(GYRO_X_L,&gx_L);
	*gx=gx_H;
	*gx=(*gx<<8)|gx_L;

	
	GY953_Read(GYRO_Y_H,&gy_H);
	GY953_Read(GYRO_Y_L,&gy_L);
	*gy=gy_H;
	*gy=(*gy<<8)|gy_L;

	
	GY953_Read(GYRO_Z_H,&gz_H);
	GY953_Read(GYRO_Z_L,&gz_L);
	*gz=gz_H;
	*gz=(*gz<<8)|gz_L;

	GY953_Data[2]=ax_H;
	GY953_Data[3]=ax_L;
	GY953_Data[4]=ay_H;
	GY953_Data[5]=ay_L;
	GY953_Data[6]=az_H;
	GY953_Data[7]=az_L;
	
	GY953_Data[8]=gx_H;
	GY953_Data[9]=gx_L;
	GY953_Data[10]=gy_H;
	GY953_Data[11]=gy_L;
	GY953_Data[12]=gz_H;
	GY953_Data[13]=gz_L;
	
	return 1;
}
int GY953_READ_Angle(float *yaw,float *pitch,float *roll)
{
	uint8_t yaw_H,yaw_L,pitch_H,pitch_L,roll_H,roll_L;
	int16_t yawt,pitcht,rollt;//�м����
	
	GY953_Read(YAW_H,&yaw_H);
	GY953_Read(YAW_L,&yaw_L);
	yawt=yaw_H;
	yawt=(yawt<<8)|yaw_L;
	*yaw=yawt/100.0;

	
	GY953_Read(PITCH_H,&pitch_H);
	GY953_Read(PITCH_L,&pitch_L);
	pitcht=pitch_H;
	pitcht=(pitcht<<8)|pitch_L;
	*pitch=pitcht/100.0;
	
	GY953_Read(ROLL_H,&roll_H);
	GY953_Read(ROLL_L,&roll_L);
	rollt=roll_H;
	rollt=(rollt<<8)|roll_L;
	*roll=rollt/100.0;
	
	return 1;
	
}

int GY953_READ_Angle_IMU(int16_t axt,int16_t ayt,int16_t azt,int16_t gxt,int16_t gyt,int16_t gzt,float *YAW)
{
	float ax,ay,az,gx,gy,gz;
	ax=2*9.8*axt/32768.0;
	ay=2*9.8*ayt/32768.0;
	az=2*9.8*azt/32768.0;
	gx=34.91*gxt/32768.0;
	gy=34.91*gyt/32768.0;
	gz=34.91*gzt/32768.0;
	
	filterUpdate(gx,gy,gz,ax,ay,az);
	*YAW   = atan2(2 * SEq_2 * SEq_3 - 2 * SEq_1 * SEq_4, -2 * SEq_3 * SEq_3 - 2 * SEq_4 * SEq_4 + 1) * 57.3;  //ƫ���ǣ���z��ת��
	//		PITCH = asin(-2 * SEq_2 * SEq_4 + 2 * SEq_1 * SEq_3)*57.3; //�����ǣ���y��ת��	 
	//		ROLL  = atan2(2 * SEq_3 * SEq_4 - 2 * SEq_1 * SEq_2, -2 * SEq_2 * SEq_2 - 2 * SEq_3* SEq_3 + 1)*57.3; //�����ǣ���x��ת��
	*(int16_t *)&(GY953_Data[24])=(int16_t)(*YAW*100);
}

int GY953_ACC_GYRO_ADJUST(void)
{
	GY953_Write(CONTROL_B,0x15);		//�Լ� У׼ ��λ��1�ɻָ���������
	return 1;
}

int GY953_MAG_ADJUST(void)
{
	GY953_Write(CONTROL_B,0x19);		//�Լ� У׼ ��λ��1�ɻָ���������
	return 1;
}
