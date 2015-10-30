#define __CONTROL_C_
#include "includes.h"


int g_f_pit = 0;
int g_f_enable_mag_steer_control = 0;
int g_f_enable_speed_control = 0;	/* 启用闭环速度控制标志位 */
int g_f_enable_pwm_control = 0;	/* 启用开环速度控制标志位 */
int g_pit_cnt=0;
int update_steer_helm_basement_to_steer_helm(void);

int counter=0;


float maxep=0,maxecp=0;
float maxen=0,maxecn=0;

//全局开始
int g_f_all_start=0;

//光编距离计数
int Range=50000;
int dis_count=0;
int dis_count_last=0;
int g_f_dis_count_control = 0; 		/* 启用两米定距离前进标志位 */

//速度控制全局变量
static float d_speed_pwm=0;
static float speed_pwm=0;
float speed_pwm_0=0;
extern unsigned char g_nSpeedControlPeriod;
//角度控制全局变量
float g_fCarAngle;
float g_fGyroscopeAngleSpeed;
float g_fGyroscopeTurnSpeed;
float CarAngleInitial=0;
float CarAnglespeedInitial=0;
extern float  AngleCalculate[6];
//左右平衡控制全局变量
float g_fCarAngle_balance;
float g_fGyroscopeAngleSpeed_balance;
float g_fGyroscopeTurnSpeed_balance;
float CarAngleInitial_balance=0;
float CarAnglespeedInitial_balance=0;


//转向控制全局变量
float g_fAngleYaw;
float g_fGyroscopeAngleSpeedYaw;
float EndYawAngle;
float EndYawAnglespeed=0;
int g_turn_count=0;
int g_turn_state=0;
float g_turn_start_angle=0;

float yaw_pwm=0; 
float yaw_pwm_0=0;	//初始电机差速转向，保证偏航角不变

// float AngleControlOutMax=0.2, AngleControlOutMin=-0.2;

float  PITCH_angle_pwm;
float  ROLL_angle_pwm;


static int new_speed_pwm=0;
static int old_speed_pwm=0;
BYTE speed_period=0;



/*-----------------------------------------------------------------------*/
/* PIT中断处理函数        10ms                                                         */
/*-----------------------------------------------------------------------*/
void PitISR(void)
{
	//get_speed_now();//光编读值
	/* 开始执行速度控制算法 */
	PIT.CH[1].TFLG.B.TIF = 1;	// MPC56xxB/P/S: Clear PIT 1 flag by writing 1
}

/*-----------------------------------------------------------------------*/
/* 获得光编当前速度                                                                    */
/*-----------------------------------------------------------------------*/
void get_speed_now()
{
	/*	光编1读值	*/
		data_encoder1.is_forward = SIU.GPDI[57].B.PDI;//PD9
		data_encoder1.cnt_old = data_encoder1.cnt_new;
		data_encoder1.cnt_new = (WORD)EMIOS_0.CH[2].CCNTR.R;//PA2
		if (data_encoder1.cnt_new >= data_encoder1.cnt_old)
		{
			data_encoder1.speed_now = data_encoder1.cnt_new - data_encoder1.cnt_old;
		}
		else
		{
			data_encoder1.speed_now = 0xffff - (data_encoder1.cnt_old - data_encoder1.cnt_new);
		}
		if(g_f_all_start==1)
		{
			if(data_encoder1.is_forward==0) 
				data_encoder1.speed_real = 0 - data_encoder1.speed_now;
			else 
				data_encoder1.speed_real = data_encoder1.speed_now;
		}
		if(g_f_all_start==2)
			g_f_all_start=1;
		if(flagkey4==1 || g_f_dis_count_control==1) //按键4 手推记步  or 标志位打开自动记步
		{
			if(data_encoder1.is_forward==0) 
				dis_count+=( 0 - data_encoder1.speed_now);
			else 
				dis_count+= data_encoder1.speed_now;
		}

}
/*-----------------------------------------------------------------------*/
/* 设置俯仰电机PWM                                                                    */
/*-----------------------------------------------------------------------*/

void set_PITCH_motor_pwm(int16_t motor_pwm)	//speed_pwm正为向前，负为向后
{
	if (motor_pwm>0)	//forward
	{
		if (motor_pwm>SPEED_PWM_MAX)
		{
			motor_pwm = SPEED_PWM_MAX;
		}
		EMIOS_0.CH[21].CBDR.R = motor_pwm;
		EMIOS_0.CH[22].CBDR.R = 1;
		
	}
	else if (motor_pwm<0)	//backward
	{
		motor_pwm = 0-motor_pwm;
		if (motor_pwm>SPEED_PWM_MAX)
		{
			motor_pwm = SPEED_PWM_MAX;
		}

		EMIOS_0.CH[21].CBDR.R = 1;
		EMIOS_0.CH[22].CBDR.R = motor_pwm;	
	}
	else
	{
		EMIOS_0.CH[21].CBDR.R = 1;
		EMIOS_0.CH[22].CBDR.R = 1;	
	}
//	LCD_PrintoutInt(64, 2, motor_pwm);
}
void PITCH_motor_control(void)
{
	int16_t motor_pwm;

	motor_pwm=PITCH_angle_pwm-speed_pwm;

//	motor_pwm=speed_pwm;
	set_PITCH_motor_pwm(motor_pwm);
	
}

//void motor_control_balance(void)
//{
//	int16_t motor_pwm_balance;
//	motor_pwm_balance=angle_pwm_balance;
//	set_motor_pwm(angle_pwm_balance);
//}
#if 0
/*-----------------------------------------------------------------------*/
/* 设置平衡电机PWM                                                                    */
/*-----------------------------------------------------------------------*/
void set_ROLL_motor_pwm(int16_t motor_pwm)	//speed_pwm正为向前，负为向后
{
	if (motor_pwm>0)	//forward
	{
		if (motor_pwm>SPEED_PWM_MAX)
		{
			motor_pwm = SPEED_PWM_MAX;
		}
		EMIOS_0.CH[19].CBDR.R = motor_pwm;
		EMIOS_0.CH[20].CBDR.R = 1;
		
	}
	else if (motor_pwm<0)	//backward
	{
		motor_pwm = 0-motor_pwm;
		if (motor_pwm>SPEED_PWM_MAX)
		{
			motor_pwm = SPEED_PWM_MAX;
		}

		EMIOS_0.CH[19].CBDR.R = 1;
		EMIOS_0.CH[20].CBDR.R = motor_pwm;	
	}
	else
	{
		EMIOS_0.CH[19].CBDR.R = 1;
		EMIOS_0.CH[20].CBDR.R = 1;	
	}
}
#endif
void test_pwm(void)
{
	EMIOS_0.CH[19].CBDR.R = 1;//PE3
	EMIOS_0.CH[20].CBDR.R = 1400;//PE4
}
/*----设置螺旋桨电机A MOTOR3 黑白线电机-------*/
void set_PropellerA_motor_pwm(int16_t motor_pwm)	
{
	//使用PE3 PE4
	int16_t tempp=0,tempn=0;
	motor_pwm=800-motor_pwm;
	tempp=motor_pwm+yaw_pwm;
	tempn=motor_pwm-yaw_pwm;
	if (tempp>0&&tempn>0)	
	{
		if (tempp>1700)//最高电压7.2V
		{
			tempp = 1700;
		}
		else if(tempp<50)
		{
			tempp=50;
		}
		if (tempn>1700)//最高电压7.2V
		{
			tempn = 1700;
		}
		else if(tempn<50)
		{
			tempn=50;
		}
		EMIOS_0.CH[18].CBDR.R = tempn;//PE3
		EMIOS_0.CH[19].CBDR.R = tempp;//PE4
//		if(motor_pwm+yaw_pwm>1700)
//			EMIOS_0.CH[20].CBDR.R = 1700;//PE3
//		else
//			EMIOS_0.CH[20].CBDR.R = motor_pwm+yaw_pwm;//PE3
//
//		if(motor_pwm-yaw_pwm<1)
//			EMIOS_0.CH[19].CBDR.R =1;
//		else
//			EMIOS_0.CH[19].CBDR.R = motor_pwm-yaw_pwm;//PE4
	}
	else 	
	{
		EMIOS_0.CH[18].CBDR.R = 1;
		EMIOS_0.CH[19].CBDR.R = 1;	
	}
}

/*---设置螺旋桨电机B MOTOR2 红蓝线电机---*/
void set_PropellerB_motor_pwm(int16_t motor_pwm)	
{
	//暂时使用PE1,PE2
	int16_t tempp=0,tempn=0;
	motor_pwm=800+motor_pwm;
	tempp=motor_pwm+yaw_pwm;//p=positive
	tempn=motor_pwm-yaw_pwm;//n=negative
	if (tempp>0&&tempn>0)	
	{   
		if (tempp>1700)//最高电压7.2V
		{
			tempp = 1700;
		}
		else if(tempp<50)
		{
			tempp=50;
		}
		if (tempn>1700)//最高电压7.2V
		{
			tempn = 1700;
		}
		else if(tempn<50)
		{
			tempn=50;
		}
		EMIOS_0.CH[17].CBDR.R =tempp;//PE2
		EMIOS_0.CH[20].CBDR.R =tempn;//PE1
		
//		EMIOS_0.CH[18].CBDR.R = motor_pwm+yaw_pwm;//PE2
//		if(motor_pwm-yaw_pwm<0)
//			EMIOS_0.CH[17].CBDR.R =0;
//		else
//			EMIOS_0.CH[17].CBDR.R = motor_pwm-yaw_pwm;//PE1
	}
	else 	
	{
		EMIOS_0.CH[20].CBDR.R = 1;
		EMIOS_0.CH[17].CBDR.R = 1;	
	}
}
void PropellerA_Control(void)
{
	int16_t motor_pwm;
	motor_pwm=ROLL_angle_pwm;
	set_PropellerA_motor_pwm(motor_pwm);
}
void PropellerB_Control(void)
{
	int16_t motor_pwm;
	motor_pwm=ROLL_angle_pwm;
	set_PropellerB_motor_pwm(motor_pwm);
}



/*-----------------------------------------------------------------------*/
/* 设置转向电机PWM                                                                    */
/*-----------------------------------------------------------------------*/
void set_YAW_motor_pwm(int16_t motor_pwm)	//speed_pwm正为向前，负为向后
{
	if (motor_pwm>0)	//forward
		{
			if (motor_pwm>SPEED_PWM_MAX)
			{
				motor_pwm = SPEED_PWM_MAX;
			}
			EMIOS_0.CH[3].CBDR.R = motor_pwm;	//PA3
			EMIOS_0.CH[4].CBDR.R = 1;			//PA4
			
		}
		else if (motor_pwm<0)	//backward
		{
			motor_pwm = 0-motor_pwm;
			if (motor_pwm>SPEED_PWM_MAX)
			{
				motor_pwm = SPEED_PWM_MAX;
			}

			EMIOS_0.CH[3].CBDR.R = 1;
			EMIOS_0.CH[4].CBDR.R = motor_pwm;	
		}
		else
		{
			EMIOS_0.CH[3].CBDR.R = 1;
			EMIOS_0.CH[4].CBDR.R = 1;	
		}
}
#if 0
void ROLL_motor_control(void)
{
	int16_t motor_pwm;
	motor_pwm=ROLL_angle_pwm;
	set_ROLL_motor_pwm(motor_pwm);
}
#endif


/*-----------------------------------------------------------------------*/
/* 前后角度控制                                                             */
/*-----------------------------------------------------------------------*/
void AngleControl(void)
{  
	float delta_angle;
	float temp_angle, temp_anglespeed;
	float last_angle=0;
	g_fCarAngle= AngleCalculate[0];
	g_fGyroscopeAngleSpeed= AngleCalculate[1];
 
	temp_angle=CarAngleInitial - g_fCarAngle;
	temp_anglespeed= CarAnglespeedInitial - g_fGyroscopeAngleSpeed;
  
//   if(temp_angle<-15)
//	   data_angle_pid.p=150;	//150//100开环
//   else if(temp_angle>=-15&temp_angle<=0)
//	   data_angle_pid.p=260;	//230//200开环
//   else if(temp_angle>0&temp_angle<=15)
//	   data_angle_pid.p=260;	//230//200开环
//   else
//	   data_angle_pid.p=150;	//150//100开环
//                                                    
//  
//   if(temp_anglespeed>=50||temp_anglespeed<=-50)
//	   data_angle_pid.d=2;//0.3
//   else
//	   data_angle_pid.d=0.5;//0.1 
   delta_angle = data_angle_pid.p*(CarAngleInitial - g_fCarAngle);
   delta_angle+=data_angle_pid.d*(CarAnglespeedInitial - g_fGyroscopeAngleSpeed);

  //delta_angle = data_angle_pid.p*(CarAngleInitial - g_fCarAngle) /5000 +data_angle_pid.d*(CarAnglespeedInitial - g_fGyroscopeAngleSpeed) /15000; // 1000 与10000是否根据实际需要调整 
  //angle_pwm=delta_angle;
    PITCH_angle_pwm=delta_angle;
  
}

/*-----------------------------------------------------------------------*/
/* 前后角度控制                                                             */
/*-----------------------------------------------------------------------*/
void Fuzzypid_Control(float *tp,float *td)
{
	float ke,kec,kup,kud;
	float e,ec;
	int E,EC,UKP,UKD;
	float delta_kp;
	float delta_kd;
	static int FuzzyTable_delta_kp[13][13]={              //kp v1.2
		   { 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 0, 0,-1},
		   { 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 0, 0,-1},
		   { 5, 5, 5, 5, 4, 4, 2, 2, 1, 1, 0, 0,-1},
		   { 5, 5, 5, 5, 4, 4, 2, 2, 1, 1, 0, 0,-1},
		   { 4, 4, 4, 4, 3, 3, 2, 2, 1, 1, 1, 1, 1},
		   { 4, 4, 4, 4, 3, 3, 2, 2, 1, 1, 1, 1, 1},
		   { 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1},
		   { 1, 1, 1, 1, 1, 1, 2, 2, 1, 1, 1, 1, 1},
		   { 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4},
		   { 1, 1, 1, 1, 1, 1, 2, 2, 3, 3, 4, 4, 4},
		   {-1,-1, 0, 0, 1, 1, 2, 2, 4, 4, 5, 5, 5},
		   {-1,-1, 0, 0, 1, 1, 2, 2, 4, 4, 5, 5, 5},
		   {-1,-1, 0, 0, 2, 2, 3, 3, 4, 4, 5, 5, 6}};
//	int FuzzyTable_delta_kp[13][13]={              //kp v1.1
//			{ 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 0, 0,-1},
//		    { 6, 6, 5, 5, 4, 4, 3, 3, 2, 2, 0, 0,-1},
//		    { 5, 5, 5, 5, 4, 4, 2, 2, 1, 1, 0, 0,-1},
//		    { 5, 5, 5, 5, 4, 4, 2, 2, 1, 1, 0, 0,-1},
//		    { 4, 4, 4, 4, 3, 3, 1, 1,-1,-1,-1,-1,-2},
//		    { 4, 4, 4, 4, 3, 3, 1, 1,-1,-1,-1,-1,-2},
//		    { 3, 3, 2, 2, 1, 1, 1, 1,-1,-1,-2,-2,-3},
//		    { 3, 3, 2, 2, 1, 1, 1, 1,-1,-1,-2,-2,-3},
//		    { 2, 2, 1, 1, 0, 0,-1,-1,-2,-2,-4,-4,-4},
//		    { 2, 2, 1, 1, 0, 0,-1,-1,-2,-2,-4,-4,-4},
//		    { 1, 1,-1,-1,-1,-1,-2,-2,-4,-4,-4,-4,-5},
//		    { 1, 1,-1,-1,-1,-1,-2,-2,-4,-4,-4,-4,-5},
//		    { 1, 1,-1,-1,-2,-2,-3,-3,-4,-4,-5,-5,-5}};
//	int FuzzyTable_delta_kd[13][13]={ 
//			{ 0, 0,-1,-1,-4,-4,-5,-5,-4,-4,-2,-2,-1},
//			{ 0, 0,-1,-1,-4,-4,-5,-5,-4,-4,-2,-2,-1},
//			{ 0, 0,-2,-2,-4,-4,-4,-4,-4,-4,-2,-2,-1},
//			{ 0, 0,-2,-2,-4,-4,-4,-4,-4,-4,-2,-2,-1},
//			{ 0, 0,-2,-2,-4,-4,-4,-4,-3,-3,-2,-2,-1},
//			{ 0, 0,-2,-2,-4,-4,-4,-4,-3,-3,-2,-2,-1},
//			{-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1},
//			{-1,-1,-2,-2,-2,-2,-2,-2,-2,-2,-1,-1,-1},
//			{ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1},
//			{ 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1},
//			{ 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3},
//			{ 2, 2, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 3},
//			{ 2, 2, 2, 2, 1, 1, 3, 3, 3, 3, 3, 3, 4}};
//	int FuzzyTable_delta_kd[13][13]={                 //kd v1.1
//		     {  4, 4, 4, 4, 2, 2,-1,-1,-2,-2,-2,-2,-1},
//		     {  4, 4, 4, 4, 2, 2,-1,-1,-2,-2,-2,-2,-1},
//		     {  4, 4, 3, 3, 2, 2,-1,-1,-1,-1, 0, 0, 1},
//		     {  4, 4, 3, 3, 2, 2,-1,-1,-1,-1, 0, 0, 1},
//		     {  3, 3, 3, 3, 1, 1,-1,-1,-1,-1, 0, 0, 2},
//		     {  3, 3, 3, 3, 1, 1,-1,-1,-1,-1, 0, 0, 2},
//		     {  3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 3},
//		     {  3, 3, 2, 2, 1, 1, 1, 1, 1, 1, 2, 2, 3},
//		     {  2, 2, 0, 0,-1,-1,-1,-1, 1, 1, 3, 3, 3},
//		     {  2, 2, 0, 0,-1,-1,-1,-1, 1, 1, 3, 3, 3},
//		     {  1, 1, 0, 0,-1,-1,-1,-1, 2, 2, 3, 3, 4},
//		     {  1, 1, 0, 0,-1,-1,-1,-1, 2, 2, 3, 3, 4},
//		     { -1,-1,-2,-2,-2,-2,-1,-1, 2, 2, 4, 4, 4}};
	static int FuzzyTable_delta_kd[13][13]={             //kd v1.2
		{ 4, 4, 4, 4, 0, 0,-2,-2,-3,-3,-2,-2,-1},
		{ 4, 4, 4, 4, 0, 0,-2,-2,-3,-3,-2,-2,-1},
		{ 4, 4, 2, 2, 0, 0,-2,-2,-2,-2, 0, 0, 0},
		{ 4, 4, 2, 2, 0, 0,-2,-2,-2,-2, 0, 0, 0},
		{ 1, 1, 1, 1,-1,-1,-1,-1,-1,-1, 1, 1, 1},
		{ 1, 1, 1, 1,-1,-1,-1,-1,-1,-1, 1, 1, 1},
		{ 1, 1, 1, 1, 0, 0,-1,-1, 0, 0, 1, 1, 1},
		{ 1, 1, 1, 1, 0, 0,-1,-1, 0, 0, 1, 1, 1},
		{ 1, 1, 1, 1,-1,-1,-1,-1,-1,-1, 1, 1, 1},
		{ 1, 1, 1, 1,-1,-1,-1,-1,-1,-1, 1, 1, 1},
		{ 0, 0, 0, 0,-2,-2,-2,-2, 0, 0, 2, 2, 4},
		{ 0, 0, 0, 0,-2,-2,-2,-2, 0, 0, 2, 2, 4},
		{-1,-1,-2,-2,-3,-3,-2,-2, 0, 0, 4, 4, 4}};
	ke=6.0/7; 		//角度输入最大值 7
	kec=6.0/150;	//角速度最大值150
	kup=40.0/6; 	//delta_kp最大值为40
	kud=2.0/6; 		//delta_dp最大值为2
	e=AngleCalculate[2];
	ec=AngleCalculate[3];
	E=(int)(ke*(e-0));
	EC=(int)(kec*(ec-0));
	if(E>6)
		E=6;
	else if (E<-6)
		E=-6;
	if(EC>6)
		EC=6;
	else if (EC<-6)
		EC=-6;
	UKP=FuzzyTable_delta_kp[E+6][EC+6];
	delta_kp=kup*UKP;
	UKD=FuzzyTable_delta_kd[E+6][EC+6];
	delta_kd=kud*UKD;
	
	*tp=data_ROLL_angle_pid.p+delta_kp;
	*td=data_ROLL_angle_pid.d+delta_kd;
	
	
}
void getmax(void)
{

	if(AngleCalculate[2]<0)
	{
		if(maxen>AngleCalculate[2])
			maxen=AngleCalculate[2];
	}
	else
	{
		if(maxep<AngleCalculate[2])
			maxep=AngleCalculate[2];
	}
	if(AngleCalculate[3]<0)
	{
		if(maxecn>AngleCalculate[3])
			maxecn=AngleCalculate[3];
	}
	else
	{
		if(maxecp<AngleCalculate[3])
			maxecp=AngleCalculate[3];
	}
}



/*-----------------------------------------------------------------------*/
/* 左右平衡控制     by-LZY                                                        */
/*-----------------------------------------------------------------------*/
void BalanceControl(void)
{
	float delta_angle_balance;
	float last_angle_balance=0;
	float temp_p,temp_d;
	temp_p=data_ROLL_angle_pid.p;
	temp_d=data_ROLL_angle_pid.d;
	Fuzzypid_Control(&temp_p,&temp_d);
	g_fCarAngle_balance= AngleCalculate[2];
	g_fGyroscopeAngleSpeed_balance=AngleCalculate[3];
	 
	delta_angle_balance = temp_p*(CarAngleInitial_balance - g_fCarAngle_balance);
	delta_angle_balance+=temp_d*(CarAnglespeedInitial_balance - g_fGyroscopeAngleSpeed_balance);

	ROLL_angle_pwm=delta_angle_balance;
	ROLL_angle_pwm=ROLL_angle_pwm/1.33;
	if(ROLL_angle_pwm>2000)
	{
		ROLL_angle_pwm=2000;
	}
	else if (ROLL_angle_pwm<-2000)
	{
		ROLL_angle_pwm=-2000;
	}
//	LCD_PrintoutInt(0, 2, ROLL_angle_pwm);
}

void Propeller_YawControl(void)
{
	SWORD error=0;
	int kp,ki,kd;
	static SWORD error_last=0;
	static SWORD sum_error=0;
	static SWORD error_data[10]={0,0,0,0,0,0,0,0,0,0};
	static SWORD error_count=0;
	error_last = error;
	error = 0-GYRead[4];
	
	//P控制
	kp=(SWORD)(data_YAW_angle_pid.p*(error));       
	yaw_pwm=kp;
	//D控制
	kd=(SWORD)(data_YAW_angle_pid.d*(error-error_last));  
	yaw_pwm+=kd;
	//I控制
	sum_error-=error_data[error_count];
	sum_error+=error;
	error_data[error_count]=error;
	error_count=(error_count+1)%10;
	if(sum_error>200) sum_error=200;
	if(sum_error<-200) sum_error=-200;
	ki=(SWORD)(data_YAW_angle_pid.i*(sum_error));	
	yaw_pwm+=ki;//yaw负值为俯视顺时针
	//限幅 
	if(yaw_pwm>200)
		yaw_pwm=200;
	if(yaw_pwm<-200)
		yaw_pwm=-200;   //限制pwm变化量
	yaw_pwm+=yaw_pwm_0;
}






/*-----------------------------------------------------------------------*/
/* 前进方向控制，使用前先对CarYawInitial赋值                                                       */
/*-----------------------------------------------------------------------*/
//void YawControl(void)
//{
//	float delta_yaw;
//	float delta_yawspeed;
//	float temp_yaw, temp_yawspeed;
//	float CarYaw,GyroscopeYawSpeed;
//
//	CarYaw= GYRead[4];
//	GyroscopeYawSpeed= GYRead[5];
// 
//	temp_yaw=CarYawInitial - CarYaw;//temp_yaw>0:右偏
//	temp_yawspeed= CarYawspeedInitial - GyroscopeYawSpeed;//temp_yawspeed>0:顺时针方向的角速度
//  
//	delta_yaw = data_YAW_angle_pid.p*temp_yaw;
//	delta_yaw+=data_YAW_angle_pid.d*temp_yawspeed;
//
//	yaw_pwm=delta_yaw;
//	  
//}


/*-----------------------------------------------------------------------*/
/* 获得速度偏差                                                                      */
/* 有问题找叶川                                                                      */
/*-----------------------------------------------------------------------*/
static SWORD get_e0()
{
	SWORD tmp_speed_now;
	SWORD e0;
	if (data_encoder1.is_forward)
	{
		tmp_speed_now =(SWORD) data_encoder1.speed_now;
	}
	else
	{
		tmp_speed_now = 0 - (SWORD) data_encoder1.speed_now;
	}
	e0=data_speed_settings.speed_target-tmp_speed_now;
	return e0;
	
}

/*-----------------------------------------------------------------------*/
/* PID速度控制                                                                       */
/* 有问题找叶川                                                                      */                                                          
/*-----------------------------------------------------------------------*/
void contorl_speed_encoder_pid(void)
{
	SWORD error=0;
	int kp,ki,kd;
	static SWORD error_last=0;
	static SWORD sum_error=0;
	static SWORD error_data[10]={0,0,0,0,0,0,0,0,0,0};
	static SWORD error_count=0;
	error_last = error;
	error = data_speed_settings.speed_target - (SWORD)data_encoder1.speed_real;
	

	old_speed_pwm = new_speed_pwm;
	//P控制
	kp=(SWORD)(data_speed_pid.p*(error));       
	new_speed_pwm=kp;
	//D控制
	kd=(SWORD)(data_speed_pid.d*(error-error_last));  
	new_speed_pwm+=kd;
	//I控制
	sum_error-=error_data[error_count];
	sum_error+=error;
	error_data[error_count]=error;
	error_count=(error_count+1)%10;
	if(sum_error>200) sum_error=200;
	if(sum_error<-200) sum_error=-200;
	if(data_speed_pid.i==0) sum_error=0;
	ki=(SWORD)(data_speed_pid.i*(sum_error));	
	new_speed_pwm+=ki;
	//限幅
	new_speed_pwm+=speed_pwm_0;
	if(new_speed_pwm>1000)
		new_speed_pwm=1000;
	if(new_speed_pwm<-1000)
		new_speed_pwm=-1000;   //限制pwm变化量
}
void set_speed_pwm(void)
{
	speed_pwm = (d_speed_pwm/100)*(speed_period)+old_speed_pwm;
	d_speed_pwm = new_speed_pwm - old_speed_pwm;
}

/*-----------------------------------------------------------------------*/
/* 设置目标速度                                                                      */
/*-----------------------------------------------------------------------*/
void set_speed_target(SWORD speed_target)
{
	data_speed_settings.speed_target = speed_target;
}

/*-----------------------------------------------------------------------*/
/* 设置驱动轮电机目标占空比                                                                      */
/*-----------------------------------------------------------------------*/
void set_pwm1_target(SWORD speed_pwm)
{
	motor_pwm_settings.motor_1_pwm = speed_pwm;
}
/*-----------------------------------------------------------------------*/
/* 设置平衡轮电机目标占空比                                                                      */
/*-----------------------------------------------------------------------*/
void set_pwm2_target(SWORD speed_pwm)
{
	motor_pwm_settings.motor_2_pwm = speed_pwm;
}

/*-----------------------------------------------------------------------*/
/* 设置转向轮电机目标占空比                                                                      */
/*-----------------------------------------------------------------------*/
void set_pwm3_target(SWORD speed_pwm)
{
	motor_pwm_settings.motor_3_pwm = speed_pwm;
}

/*-----------------------------------------------------------------------*/
/* 设置速度PID控制PID值 根据目标速度设置 speed_now                                                            */
/*-----------------------------------------------------------------------*/
void set_speed_PID(void) 
{ 
	if(data_speed_settings.speed_target==0)
	{
		data_speed_pid.p=60;
		data_speed_pid.i=0;
		data_speed_pid.d=1;
	}
	if(data_speed_settings.speed_target==10)
	{
		data_speed_pid.p=60;
		data_speed_pid.i=2;
		data_speed_pid.d=1;
	}
	if(data_speed_settings.speed_target==35)
	{
		data_speed_pid.p=55;
		data_speed_pid.i=0;
		data_speed_pid.d=1;
	}
	if(data_speed_settings.speed_target==20)
	{
			data_speed_pid.p=55;
			data_speed_pid.i=1;
			data_speed_pid.d=1;
	}
	
	
	
//	if(data_speed_settings.speed_target==0)
//	{
//		data_speed_pid.p=5;
//		data_speed_pid.d=0;
//	}
//	else if(data_speed_settings.speed_target<10)
//	{
//		data_speed_pid.p=50;
//	}
//	else if(data_speed_settings.speed_target<20)
//	{
//		data_speed_pid.p=45;
//	}
	/*int speed_target=data_speed_settings.speed_target;
	int speed_now=data_speed_settings.speed_target_now;
	if(speed_target==0)//420 
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;         
	}
	else if(speed_target>=450&&speed_target<=500)//420 
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;          

		if(speed_now<speed_target)
			speed_now+=25;
		else if(speed_now>speed_target)
			speed_now-=25;
		else
			speed_now=speed_target;
	}

	else if(speed_target>500&&speed_target<=560)//420 
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;         
		if(speed_now<speed_target)
			speed_now+=30;          //30可调 值越大初始时速度变化快
		else if(speed_now>speed_target)
			speed_now-=30;
		else
			speed_now=speed_target;
	} 

	else if(speed_target>560&&speed_target<=620 )
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;          
		if(speed_now<speed_target)
			speed_now+=30;
		else if(speed_now>speed_target)
			speed_now-=30;
		else
			speed_now=speed_target;
	} 

	else
	{
		data_speed_pid.p=15;
		data_speed_pid.d=5;
		data_speed_pid.i=1;          
		if(speed_now<speed_target)
			speed_now+=30;
		else if(speed_now>speed_target)
			speed_now-=30;
		else
			speed_now=speed_target;
	} 
*/
}


/*-----------------------------------------------------------------------*/
/* 设置速度PID控制P值                                                            */
/*-----------------------------------------------------------------------*/
void set_speed_KP(float kp)
{
	data_speed_pid.p = kp;
}


/*-----------------------------------------------------------------------*/
/* 设置速度PID控制I值                                                             */
/*-----------------------------------------------------------------------*/
void set_speed_KI(float ki)
{
	data_speed_pid.i = ki;
}


/*-----------------------------------------------------------------------*/
/* 设置速度PID控制D值                                                            */
/*-----------------------------------------------------------------------*/
void set_speed_KD(float kd)
{
	data_speed_pid.d = kd;
}

/*-----------------------------------------------------------------------*/
/* 设置角度PID控制P值                                                            */
/*-----------------------------------------------------------------------*/
void set_angle_KP(float kp)
{
	data_angle_pid.p = kp;
}


/*-----------------------------------------------------------------------*/
/* 设置角度PID控制I值                                                             */
/*-----------------------------------------------------------------------*/
void set_angle_KI(float ki)
{
	data_angle_pid.i = ki;
}


/*-----------------------------------------------------------------------*/
/* 设置角度PID控制D值                                                            */
/*-----------------------------------------------------------------------*/
void set_angle_KD(float kd)
{
	data_angle_pid.d = kd;
}

/*-----------------------------------------------------------------------*/
/* 设置平衡PID控制P值                                                            */
/*-----------------------------------------------------------------------*/
void set_ROLL_KP(float kp)
{
	data_ROLL_angle_pid.p = kp;
}


/*-----------------------------------------------------------------------*/
/* 设置平衡PID控制I值                                                             */
/*-----------------------------------------------------------------------*/
void set_ROLL_KI(float ki)
{
	data_ROLL_angle_pid.i = ki;
}


/*-----------------------------------------------------------------------*/
/* 设置平衡PID控制D值                                                            */
/*-----------------------------------------------------------------------*/
void set_ROLL_KD(float kd)
{
	data_ROLL_angle_pid.d = kd;
}


void set_yaw_angle_target(float yaw_target)
{
	yaw_angle_target=yaw_target;
}
/*-----------------------------------------------------------------------*/
/* 设置Yaw PID控制P值                                                            */
/*-----------------------------------------------------------------------*/
void set_Yaw_KP(float kp)
{
	data_YAW_angle_pid.p=kp;
}
/*-----------------------------------------------------------------------*/
/* 设置Yaw PID控制D值                                                            */
/*-----------------------------------------------------------------------*/
void set_Yaw_KD(float kd)
{
	data_YAW_angle_pid.d=kd;
}
void set_Yaw_KI(float ki)
{
	data_YAW_angle_pid.i=ki;
}
/*-----------------------------------------------------------------------*/
/* 获取两个周期计数的差值，常用故写成函数                               */
/*-----------------------------------------------------------------------*/
DWORD diff_time_basis_PIT(const DWORD new_time, const DWORD old_time)
{
	DWORD diff;
	
	if (new_time >= old_time)
	{
		diff = new_time - old_time;
	}
	else
	{
		diff = new_time + (0xFFFFFFFF- old_time);
	}
	
	return diff;
}
#if 0
int abs(int data)
{
	if (data<0)
		data=0-data;
	return data;
}
#endif
