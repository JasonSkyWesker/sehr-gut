#include "includes.h"

BYTE count=0;
BYTE  SpeedCountFlag=0;
BYTE g_f_speed_adapt=0;
void key_work(void);

void main(void)	  
{
	float temp1=0,temp2=0,c1=0;
	int16_t rYAW;//由寄存器读取的YAW
	
	int16_t axt,ayt,azt,gxt,gyt,gzt;//直接读出的数据
	float ax,ay,az,gx,gy,gz;
	float YAW,ROLL,PITCH;
	
	init_all_and_POST();
	if(mode==0||mode==2) Range=50000;
	if(mode==1||mode==3) Range=55000;
	if(mode==0||mode==1) g_f_speed_adapt=0;
	if(mode==2||mode==3) g_f_speed_adapt=1;
	LCD_PrintoutInt(0,0,Range/10);
	LCD_PrintoutInt(64,0,g_f_speed_adapt);
	set_speed_target(0);
	StopLight_1=1;
	
	//set_YAW_motor_pwm();新增电机口
	for(;;)
	{
		set_key();//按键设置
		if(g_f_all_start==1)
		{
			if(send_timer>=3000)
			{
				//StopLight_1=~StopLight_1;
				disable_pit_1ms();
				send_data2PC(GY953,ANGLE_TYPE,GY953_Data);
				send_timer=0;
				enable_pit_1ms();
				g_f_all_start=2;
			}
			else if(send_timer==2000)
			{
				//StopLight_1=~StopLight_1;
				disable_pit_1ms();
				send_data2PC(GY953,ACC_TYPE,GY953_Data);
				enable_pit_1ms();
				g_f_all_start=2;
			}
			else if(send_timer==1000)
			{
				//StopLight_1=~StopLight_1;
				disable_pit_1ms();
				send_data2PC(GY953,GYR_TYPE,GY953_Data);
				enable_pit_1ms();
				g_f_all_start=2;
			}
		}

		
#if 1
		if (REMOTE_FRAME_STATE_OK == g_remote_frame_state)
		{
			g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
			
			execute_remote_cmd(remote_frame_data+2);
		}	
		if(g_Control)
		{
			count++;
			speed_period++;
			/*读取各陀螺仪值*/
			angle_read(AngleResult); 	//读取模拟陀螺仪的AD返回值
			angle_calculate();			 //转换模拟陀螺仪返回值
			if(count==2)
					Gy953_angle_read();			 //GY953 数字陀螺仪数据读取 陀螺仪数据更新周期5ms
			if(count==3)
			{
				GY953_READ_Angle_IMU(GYRead[6],GYRead[7],GYRead[8],GYRead[1],GYRead[3],GYRead[5],&GYRead[4]);
				//根据航向目标值 修改航向坐标零点 目标方向为0
				GYRead[4]=-GYRead[4];
				GYRead[4]=(GYRead[4]-yaw_angle_target);
				while(GYRead[4]>180) GYRead[4]-=360;
				while(GYRead[4]<-180) GYRead[4]+=360;
			}
			key_work();					//按键屏幕显示
			/*车身姿态判断*/
			if((AngleCalculate[0]>20||AngleCalculate[0]<-20)||(AngleCalculate[2]>20||AngleCalculate[2]<-20))
			{
				set_PITCH_motor_pwm(0);
				set_PropellerA_motor_pwm(2000);
				set_PropellerB_motor_pwm(-2000);
				g_Control=0;
				if(count==5)
				{
					count=0;
				}
				continue;
			}
			set_speed_pwm();			//前后速度pwm递增
			AngleControl();				//前后角度控制pwm计算
			PITCH_motor_control();		//前后平衡、速度控制 每毫秒控制  *开屏幕时不可开启驱动板

			if(g_f_dis_count_control==1)				//开启两米记步前进
			{
				if(delay_timer<=0&&g_f_speed_adapt==1)
				{
					StopLight_2=0;
					delay_timer=1000;
					if(dis_count-dis_count_last<=1800)
					{
						speed_pwm_0+=30;
						StopLight_2=1;
					}
					if((dis_count-dis_count_last>=7000))
					{
						speed_pwm_0-=30;
						StopLight_2=1;
					}
					dis_count_last=dis_count;
				}
				if(dis_count>=Range)
				{
					dis_count=0;
					g_f_dis_count_control=0;
					speed_pwm_0=0;
					delay_timer=0;
					if(g_turn_count<4)
					{
						set_speed_target(0);
						g_turn_state=1;	
//						g_turn_start_angle=yaw_angle_target;
					}
				/*	if(g_turn_count==3)
					{
						set_speed_target(0);
						StopLight_2=1;
					}
					*/
				}
				else	if(dis_count<Range/5)
					
				{
					data_YAW_angle_pid.p=30;
					data_YAW_angle_pid.i=1;
					data_YAW_angle_pid.d=3;
					set_speed_target(20);
				}
//				else	if(dis_count>Range*9/10)
//				{
//					data_YAW_angle_pid.p=30;
//					data_YAW_angle_pid.i=1;
//					data_YAW_angle_pid.d=3;
//					set_speed_target(15);
//				}
				else	if(dis_count>Range*4/5)
				{
					data_YAW_angle_pid.p=30;
					data_YAW_angle_pid.i=1;
					data_YAW_angle_pid.d=3;
					set_speed_target(20);
				}
				else
				{
					data_YAW_angle_pid.p=20;
					data_YAW_angle_pid.i=1;
					data_YAW_angle_pid.d=3;
					set_speed_target(35);
				}
				
			}
			
			if(g_turn_state!=0)
			{
				if(delay_timer==0)
				{
					delay_timer=500;		//重新计时
					yaw_pwm_0+=30;		//转向增益50 
					StopLight_2=0;
				}
				if(GYRead[4]<=5&&GYRead[4]>=-5)
				{
					//StopLight_1=~StopLight_1;
					if(g_turn_state<10)								//分9次转向 每次10度
					{
						StopLight_2=1;
						delay_timer=1000;							//开始计时2s
						yaw_pwm_0=0;								//转向增益清零
						yaw_angle_target+=10;						//目标值递增
						GYRead[4]=(GYRead[4]-yaw_angle_target);		//坐标转换
						while(GYRead[4]>180) GYRead[4]-=360;
						while(GYRead[4]<-180) GYRead[4]+=360;
						data_YAW_angle_pid.p=15;					//PID调整
						data_YAW_angle_pid.i=1;
						data_YAW_angle_pid.d=2;
						g_turn_state++;								//进入下一转向阶段
					}
					else if(g_turn_state==10&&GYRead[4]<=3&&GYRead[4]>=-3)
					{
						StopLight_2=0;
						yaw_pwm_0=0;				//转向增益清零
						delay_timer=-1;				//计时停止
						g_turn_state=0;				//转向停止
						g_turn_count++;				//转向次数+1
						if(g_turn_count<4)
							g_f_dis_count_control=1;	//开启2m记步
						if(g_turn_count==4)
						{
							set_speed_target(0);
							StopLight_1=1;
						}
					}
				}
				
			}
			if(flagkey4==1||flagkey1==1)				//按键4和按键1不进行左右平衡
			{
				set_PropellerA_motor_pwm(2000);
				set_PropellerB_motor_pwm(-2000);				
			}
			
			if(count==3&&flagkey3==1||flagkey2==1) 	//5毫秒控制一次  按键2（有显示）、按键3（无显示）有效
			{
				/*	左右控制	*/
				BalanceControl();				//左右平衡控制
				Propeller_YawControl();			//左右航向控制
				PropellerA_Control();			//黑白电机控制
				PropellerB_Control();			//红蓝电机控制
			}
			
			if(count==4)//5毫秒计算一次速度控制
			{
				SpeedCountFlag++;
				if(SpeedCountFlag>=20) 				//达到一百毫秒
				{
					if(flagkey3)
						set_speed_PID();				//根据速度设置PID 标定时关闭
					contorl_speed_encoder_pid(); 	//速度PWM计算
					speed_period=0;					//速度增量值清零
					SpeedCountFlag=0;				//100毫秒计数清零
				}
			}

			
			if(count==5)
			{
				count=0;
			}
			g_Control=0;
		}

	#endif
		}

}


void key_work(void)
{
	if(flagkey1==1)
	{
		LCD_PrintoutInt(0, 0, AngleResult[0]);
		LCD_PrintoutInt(64, 0, AngleResult[1]);
		LCD_PrintoutInt(0, 4, AngleCalculate[0]);
		LCD_PrintoutInt(64, 4, AngleCalculate[1]);
	}
	if(flagkey1==2)
	{
		LCD_PrintoutInt(0, 0, GYRead[4]*10);
		LCD_PrintoutInt(64, 0, GYRead[5]*10);
	}
	if(flagkey2==1)
	{
		/*	左右控制	*/
		LCD_PrintoutInt(0, 0, AngleResult[2]);
		LCD_PrintoutInt(64, 0, AngleResult[3]);
		LCD_PrintoutInt(0, 4, AngleCalculate[2]);
		LCD_PrintoutInt(64, 4, AngleCalculate[3]);
	}
	if(flagkey3==1)
	{
//				getmax();
//				LCD_PrintoutInt(0, 0, maxep);
//				LCD_PrintoutInt(64, 0, maxen);
//				LCD_PrintoutInt(0,4,maxecp);
//				LCD_PrintoutInt(64,4,maxecn);
	}
	if(flagkey4==1)
	{
		LCD_PrintoutInt(0,0,dis_count/10000);			
		LCD_PrintoutInt(64,0,dis_count%10000);
		
//				EMIOS_0.CH[20].CBDR.R = yaw_pwm;
//				LCD_PrintoutInt(64, 6, yaw_pwm);			
//				Gy953_angle_read(AngleCalculate);
//				LCD_PrintoutInt(0,0,(AngleCalculate[0]*10.0));
//				LCD_PrintoutInt(64,0,AngleCalculate[1]);
//				LCD_PrintoutInt(0,2,(AngleCalculate[2]*10.0));
//				LCD_PrintoutInt(64,2,AngleCalculate[3]);
//				LCD_PrintoutInt(0,4,AngleCalculate[4]);
//				LCD_PrintoutInt(64,4,AngleCalculate[5]);
//				LCD_PrintoutInt(0,0,data_speed_pid.p);
//				LCD_PrintoutInt(0,2,data_speed_pid.i);
//				LCD_PrintoutInt(0,4,data_speed_pid.d);
//				LCD_PrintoutInt(64,0,data_angle_pid.p);
//				LCD_PrintoutInt(64,2,data_angle_pid.i);
//				LCD_PrintoutInt(64,4,data_angle_pid.d*10);
	}
	
}
