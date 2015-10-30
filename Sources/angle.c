#define __ANGLE_C_
#include "includes.h"

unsigned int jos[6]={0};
unsigned int ADResult[6]={0}; //模拟量 恒为正
unsigned int AngleResult[6]={0}; //前后角度
float AngleCalculate[6]={0};
float GYRead[9]={0};   //数字陀螺仪
float yaw_angle_target=0;

/*-----------------------------------------------------------------------*/
/*-----------俯仰陀螺仪模块读值函数-------------*/
/*--------------角度角速度-----------------*/
/*-----------------------------------------------------------------------*/
void angle_read(unsigned int *AngleGra)
{
#if 1
	int i;
	unsigned int temp[4]={0};
	for(i=0;i<20;i++)
	{
		jos[0]=(unsigned int)ADC.CDR[34].B.CDATA;
		temp[0]+=jos[0];//PB10
		jos[1]=(unsigned int)ADC.CDR[35].B.CDATA;
		temp[1]+=jos[1];//PB11
		jos[2]=(unsigned int)ADC.CDR[36].B.CDATA;
		temp[2]+=jos[2];//PD12
		jos[3]=(unsigned int)ADC.CDR[37].B.CDATA;
		temp[3]+=jos[3];//PD13
	}
	ADResult[0]=5000.0/1024*(temp[0]/20);
	ADResult[1]=5000.0/1024*(temp[1]/20);
	ADResult[2]=5000.0/1024*(temp[2]/20);
	ADResult[3]=5000.0/1024*(temp[3]/20);
	
	for(i=0;i<4;i++)
		*AngleGra++=ADResult[i]; 
#endif

}

void Gy953_angle_read(void)
{
	float angle_yaw,angle_pitch,angle_roll;
	int16_t anglespeed_yaw,anglespeed_pitch,anglespeed_roll,ax,ay,az;//gx=pitch,gy=roll,gz=yaw
	GY953_READ_ACC_GYRO(&ax,&ay,&az,&anglespeed_pitch,&anglespeed_roll,&anglespeed_yaw);
	//GY953_READ_Angle(&angle_yaw,&angle_pitch,&angle_roll);
	//GYRead[0]=angle_pitch-angle_data.GY_PITCH_angle_zero;//angle_pitch  后仰 正
	GYRead[1]=anglespeed_pitch;//anglespeed_pitch 后仰 正
	//GYRead[2]=angle_roll-angle_data.GY_ROLL_angle_zero;//angle_roll 右倾 正
	GYRead[3]=anglespeed_roll;//anglespeed_roll 右倾 正
	//GYRead[4]=angle_yaw;//angle_yaw 俯视逆时针 正 																																																												
	GYRead[5]=anglespeed_yaw;//anglespeed_yaw 俯视逆时针 正
	//加速度
	GYRead[6]=ax;
	GYRead[7]=ay;
	GYRead[8]=az;
	
}

/*-----------------------------------------------------------------------*/
/*-----------俯仰+平衡陀螺仪模块数据处理-------------*/
/*-----------陀螺仪精度0.67-----------------*/
/*-----------------------------------------------------------------------*/
void angle_calculate(void)
{

	AngleCalculate[0]=((float)AngleResult[0]-(float)angle_data.PITCH_angle_zero)/5.49; //陀螺仪的角度  前倾的角度//1380   
	AngleCalculate[1]=-((float)AngleResult[1]-(float)angle_data.PITCH_anglespeed_zero)/0.67;//陀螺仪角速度
	AngleCalculate[2]=-((float)AngleResult[2]-(float)angle_data.ROLL_angle_zero)/5.0; // 左右角度
	AngleCalculate[3]=((float)AngleResult[3]-(float)angle_data.ROLL_anglespeed_zero)/0.67; //左右角速度//1380
	*(int16_t*)(&(GY953_Data[22]))=(int16_t)(AngleCalculate[0]*100);
	*(int16_t*)(&(GY953_Data[20]))=(int16_t)(AngleCalculate[2]*100);
}




void set_PITCH_angle_zero(SWORD zero)
{
	angle_data.PITCH_angle_zero = zero;
	angle_data.GY_PITCH_angle_zero = zero;
}

void set_PITCH_angle_speed_zero(SWORD zero)
{
	angle_data.PITCH_anglespeed_zero = zero;
}

void set_ROLL_angle_zero(SWORD zero)
{
	angle_data.ROLL_angle_zero = zero;
	angle_data.GY_ROLL_angle_zero = zero/10;
}

void set_ROLL_angle_speed_zero(SWORD zero)
{
	angle_data.ROLL_anglespeed_zero = zero;
}

void set_YAW_angle_zero(SWORD zero)
{
	angle_data.YAW_angle_zero = zero;
}

void set_YAW_angle_speed_zero(SWORD zero)
{
	angle_data.YAW_anglespeed_zero = zero;
}
