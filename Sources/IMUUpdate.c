/*
 * IMUUpdate.c
 *
 *  Created on: Aug 22, 2015
 *      Author: Paul
 */


#include "includes.h"
// Math library required for ��sqrt��
// System constants
#define Accel_2_Scale_Factor    16384.0
#define Accel_4_Scale_Factor    8192.0
#define Accel_8_Scale_Factor    4096.0
#define Accel_16_Scale_Factor   2048.0
#define deltat 0.005			// sampling period in seconds (shown as 5 ms)
#define Kp 2.0     //proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.005	//integral gain governs rate of convergence of gyroscope biases
#define Accel_Zout_Offset		600
#define gyroMeasError 3.14159265358979 * (5.0 / 180.0) 		// gyroscope measurement error in rad/s (shown as 5 deg/s)
#define beta sqrt(3.0 / 4.0) * gyroMeasError 			// compute beta 
// Global system variables
float a_x, a_y, a_z;			// accelerometer measurements
float w_x, w_y, w_z;		// gyroscope measurements in rad/s
float SEq_1 = 1.0, SEq_2 = 0.0, SEq_3 = 0.0, SEq_4 = 0.0; 		// estimated orientation quaternion elements with initial conditions 
float init_ax, init_ay, init_az, init_gx, init_gy, init_gz, init_mx, init_my, init_mz;
float exInt = 0, eyInt = 0, ezInt = 0; 
float q0 = 1, q1 = 0, q2 = 0, q3 = 0;

void filterUpdate(float w_x, float w_y, float w_z, float a_x, float a_y, float a_z) 
{ 
	// Local system variables
	float norm, halfT;		// vector norm
	float SEqDot_omega_1, SEqDot_omega_2, SEqDot_omega_3, SEqDot_omega_4; 	// quaternion derrivative from gyroscopes elements
	float f_1, f_2, f_3;		// objective function elements
	float J_11or24, J_12or23, J_13or22, J_14or21, J_32, J_33; 			// objective function Jacobian elements	
	float SEqHatDot_1, SEqHatDot_2, SEqHatDot_3, SEqHatDot_4; 		// estimated direction of the gyroscope error 
	
	// Axulirary variables to avoid reapeated calcualtions 
	float halfSEq_1 = 0.5 * SEq_1; 
	float halfSEq_2 = 0.5 * SEq_2; 
	float halfSEq_3 = 0.5 * SEq_3; 
	float halfSEq_4 = 0.5 * SEq_4; 
	float twoSEq_1 = 2.0 * SEq_1; 
	float twoSEq_2 = 2.0 * SEq_2; 
	float twoSEq_3 = 2.0 * SEq_3; 

	// Normalise the accelerometer measurement
	norm = invSqrt(a_x * a_x + a_y * a_y + a_z * a_z); 
	a_x *= norm;
	a_y *= norm;
	a_z *= norm; 
	
	// Compute the objective function and Jacobian
	f_1 = twoSEq_2 * SEq_4 - twoSEq_1 * SEq_3 - a_x;
	f_2 = twoSEq_1 * SEq_2 + twoSEq_3 * SEq_4 - a_y;
	f_3 = 1.0 - twoSEq_2 * SEq_2 - twoSEq_3 * SEq_3 - a_z; 
	J_11or24 = twoSEq_3;		// J_11 negated in matrix multiplication 
	J_12or23 = 2.0 * SEq_4;	// J_12 negated in matrix multiplication 
	J_13or22 = twoSEq_1;
	J_14or21 = twoSEq_2;
	J_32 = 2.0 * J_14or21;	// negated in matrix multiplication
	J_33 = 2.0 * J_11or24; // negated in matrix multiplication
	
	// Compute the gradient (matrix multiplication)
	SEqHatDot_1 = J_14or21 * f_2 - J_11or24 * f_1;
	SEqHatDot_2 = J_12or23 * f_1 + J_13or22 * f_2 - J_32 * f_3; 
	SEqHatDot_3 = J_12or23 * f_2 - J_33 * f_3 - J_13or22 * f_1; 
	SEqHatDot_4 = J_14or21 * f_1 + J_11or24 * f_2; 

	// Normalise the gradient
	norm = invSqrt(SEqHatDot_1 * SEqHatDot_1 + SEqHatDot_2 * SEqHatDot_2 + SEqHatDot_3 * SEqHatDot_3 + SEqHatDot_4 * SEqHatDot_4); 
	SEqHatDot_1 *= norm;
	SEqHatDot_2 *= norm;
	SEqHatDot_3 *= norm;
	SEqHatDot_4 *= norm; 
	
	// Compute the quaternion derrivative measured by gyroscopes 
	SEqDot_omega_1 = -halfSEq_2 * w_x - halfSEq_3 * w_y - halfSEq_4 * w_z; 
	SEqDot_omega_2 = halfSEq_1 * w_x + halfSEq_3 * w_z - halfSEq_4 * w_y; 
	SEqDot_omega_3 = halfSEq_1 * w_y - halfSEq_2 * w_z + halfSEq_4 * w_x; 
	SEqDot_omega_4 = halfSEq_1 * w_z + halfSEq_2 * w_y - halfSEq_3 * w_x; 
	
	// Compute then integrate the estimated quaternion derrivative 
	SEq_1 += (SEqDot_omega_1 - (beta * SEqHatDot_1)) * deltat; 
	SEq_2 += (SEqDot_omega_2 - (beta * SEqHatDot_2)) * deltat; 
	SEq_3 += (SEqDot_omega_3 - (beta * SEqHatDot_3)) * deltat; 
	SEq_4 += (SEqDot_omega_4 - (beta * SEqHatDot_4)) * deltat; 
	
	// Normalise quaternion
	norm = invSqrt(SEq_1 * SEq_1 + SEq_2 * SEq_2 + SEq_3 * SEq_3 + SEq_4 * SEq_4); 
	SEq_1 *= norm;
	SEq_2 *= norm;
	SEq_3 *= norm;
	SEq_4 *= norm; 
} 

#if 0
/*******************************************************************************
* Function Name  : init_quaternion
* Description    : �����ʼ����Ԫ��q0 q1 q2 q3.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void init_quaternion(void)
{ 
	signed short int accel[3], mag[3];
	float init_Yaw, init_Pitch, init_Roll;
	int i;

	Read_GYalldata(GY953_Data);
	accel[0]=(*((SWORD *)(&(GY953_Data[2]))));
	accel[1]=(*((SWORD *)(&(GY953_Data[4]))));
	accel[2]=(*((SWORD *)(&(GY953_Data[6]))))   + Accel_Zout_Offset;
		
	init_ax=(float)(accel[0] / Accel_2_Scale_Factor);	   //��λת�����������ٶȵĵ�λ��m/s2
	init_ay=(float)(accel[1] / Accel_2_Scale_Factor);
	init_az=(float)(accel[2] / Accel_2_Scale_Factor);
	
	for(i=0;i<5;i++)   //��һ�ζ�ȡ��compsass�����Ǵ�ģ����Ҫ������α�֤�Ժ�������ȷ��оƬbug
	{
	Read_GYalldata(GY953_Data);
	init_mx =(float)(*((SWORD *)(&(GY953_Data[14]))))-8;						
	init_my =(float)1.046632*(*((SWORD *)(&(GY953_Data[16]))))-1.569948;
	init_mz =(float)(-(*((SWORD *)(&(GY953_Data[18])))));
	}
	//������y��Ϊǰ������    
	init_Roll = -atan2(init_ax, init_az);    //����ĵ�λ�ǻ��ȣ�����Ҫ�۲���Ӧ����57.3ת��Ϊ�Ƕ�
	init_Pitch=  asin(init_ay);              //init_Pitch = asin(ay / 1);      
	init_Yaw  =  atan2(init_mx*cos(init_Roll) + init_my*sin(init_Roll)*sin(init_Pitch) + init_mz*sin(init_Roll)*cos(init_Pitch),
					   init_my*cos(init_Pitch) - init_mz*sin(init_Pitch));//������atan2(my, mx)�����е�init_Roll��init_Pitch�ǻ���
	if(init_Yaw < -3.141593){init_Yaw = init_Yaw + 2*3.141593;}
	if(init_Yaw > 3.141593){init_Yaw = init_Yaw - 2*3.141593;}				            
	//����ʼ��ŷ����ת���ɳ�ʼ����Ԫ����ע��sin(a)��λ�õĲ�ͬ������ȷ����xyz��ת����Pitch����Roll����Yaw������ZXY˳����ת,Qzyx=Qz*Qy*Qx�����е�init_YawRollPtich�ǽǶ�        
	SEq_1 = cos(0.5*init_Roll)*cos(0.5*init_Pitch)*cos(0.5*init_Yaw) - sin(0.5*init_Roll)*sin(0.5*init_Pitch)*sin(0.5*init_Yaw);  //w
	SEq_2 = cos(0.5*init_Roll)*sin(0.5*init_Pitch)*cos(0.5*init_Yaw) - sin(0.5*init_Roll)*cos(0.5*init_Pitch)*sin(0.5*init_Yaw);  //x   ��x����ת��pitch
	SEq_3 = sin(0.5*init_Roll)*cos(0.5*init_Pitch)*cos(0.5*init_Yaw) + cos(0.5*init_Roll)*sin(0.5*init_Pitch)*sin(0.5*init_Yaw);  //y   ��y����ת��roll
	SEq_4 = cos(0.5*init_Roll)*cos(0.5*init_Pitch)*sin(0.5*init_Yaw) + sin(0.5*init_Roll)*sin(0.5*init_Pitch)*cos(0.5*init_Yaw);  //z   ��z����ת��Yaw
}
#endif

#if 0
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az, float init_mx, float init_my, float init_mz) 
{
        float norm, halfT;
        float vx, vy, vz;
        float ex, ey, ez;
		float Pitch, Roll, Yaw_New, Mag_Yaw;
		float q0, q1, q2, q3;
/*��һ������ֵ�����ٶȼƺʹ����Ƶĵ�λ��ʲô������ν����Ϊ�����ڴ˱����˹�һ������*/        
        //normalise the measurements
        norm = invSqrt(ax*ax + ay*ay + az*az);       
        ax = ax * norm;
        ay = ay * norm;
        az = az * norm;
 
        vx = 2*(q1*q3 - q0*q2);
        vy = 2*(q0*q1 + q2*q3);
        vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
      
      
//���ڰѼ��ٶȵĲ���ʸ���Ͳο�ʸ����������Ѵų��Ĳ���ʸ���Ͳο�ʸ��Ҳ����������������������ݡ�
        // error is sum of cross product between reference direction of fields and direction measured by sensors
        ex = (ay*vz - az*vy);
        ey = (az*vx - ax*vz);
        ez = (ax*vy - ay*vx);
       
        // integral error scaled integral gain
        exInt = exInt + ex*Ki;
        eyInt = eyInt + ey*Ki;
        ezInt = ezInt + ez*Ki;
        
        // adjusted gyroscope measurements
        gx = gx + Kp*ex + exInt;
        gy = gy + Kp*ey + eyInt;
        gz = gz + Kp*ez + ezInt;

        // integrate quaternion rate and normalise����Ԫ�������㷨��һ������-������
        q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
        q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
        q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
        q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  
        
        // normalise quaternion
        norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
        q0 = q0 * norm;       //w
        q1 = q1 * norm;       //x
        q2 = q2 * norm;       //y
        q3 = q3 * norm;       //z
/*����Ԫ�������Pitch  Roll  Yaw
Roll=arctan2(2wx+2yz, 1-2xx-2yy);
Pitch=arcsin(2wy-2zx);
Yaw=arctan2(2wz+2xy, 1-2yy-2zz);
1=q0*q0+q1*q1+q2*q2+q3*q3;
����57.3��Ϊ�˽�����ת��Ϊ�Ƕȣ�������y��Ϊǰ������*/
	Yaw   = atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2 * q2 - 2 * q3 * q3 + 1) * 57.3;  //ƫ���ǣ���z��ת��
	Pitch = asin(-2 * q1 * q3 + 2 * q0 * q2); //�����ǣ���y��ת��	 
    Roll  = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1); //�����ǣ���x��ת��

//0.9��0.1������ϵ��������5.73=0.1*57.3������57.3��Ϊ�˽�����ת��Ϊ�Ƕȣ��ù�ʽ��˼�ǽ������Ƶĳ���׼ȷ�Ⱥ�
//�����ǵĸ������Ƚ��л����˲������������ǵ����ݽ��и�ͨ�˲����Դ����Ƶ����ݽ��е�ͨ�˲��������
	Yaw   = -(0.9 * (-Yaw + init_gz*2*halfT) + 5.73 * atan2(init_mx*cos(Roll) + init_my*sin(Roll)*sin(Pitch) + init_mz*sin(Roll)*cos(Pitch), init_my*cos(Pitch) - init_mz*sin(Pitch)));
	Pitch = Pitch * 57.3;
	Roll = Roll * 57.3;
}
#endif

/*******************************************************************************
���ټ��� 1/Sqrt(x)��Դ������3��һ�δ��룬�����0x5f3759df���������Ĵ����4�� 	
*******************************************************************************/
float invSqrt(float x) 
{
	float halfx = 0.5f * x;
	float y = x;
	long i = *(long*)&y;
	i = 0x5f3759df - (i>>1);
	y = *(float*)&i;
	y = y * (1.5f - (halfx * y * y));
	return y;
}

#if 0
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
		float norm, halfT;
		float vx, vy, vz;
		float ex, ey, ez;  
		float Pitch, Roll, Yaw_New, Mag_Yaw;
/*��һ������ֵ�����ٶȼƺʹ����Ƶĵ�λ��ʲô������ν����Ϊ�����ڴ˱����˹�һ������*/        
		//normalise the measurements
		norm = invSqrt(ax*ax + ay*ay + az*az);       
		ax = ax * norm;
		ay = ay * norm;
		az = az * norm;
 
		vx = 2*(q1*q3 - q0*q2);
		vy = 2*(q0*q1 + q2*q3);
		vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;
	  
	  
//���ڰѼ��ٶȵĲ���ʸ���Ͳο�ʸ����������Ѵų��Ĳ���ʸ���Ͳο�ʸ��Ҳ����������������������ݡ�
		// error is sum of cross product between reference direction of fields and direction measured by sensors
		ex = (ay*vz - az*vy);
		ey = (az*vx - ax*vz);
		ez = (ax*vy - ay*vx);
	   
		// integral error scaled integral gain
		exInt = exInt + ex*Ki;
		eyInt = eyInt + ey*Ki;
		ezInt = ezInt + ez*Ki;
		
		// adjusted gyroscope measurements
		gx = gx + Kp*ex + exInt;
		gy = gy + Kp*ey + eyInt;
		gz = gz + Kp*ez + ezInt;
		

		// integrate quaternion rate and normalise����Ԫ�������㷨��һ������-������
		q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
		q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
		q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
		q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;  
		
		// normalise quaternion
		norm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
		q0 = q0 * norm;       //w
		q1 = q1 * norm;       //x
		q2 = q2 * norm;       //y
		q3 = q3 * norm;       //z
}
#endif
