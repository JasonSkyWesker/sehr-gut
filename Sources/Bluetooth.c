#define __BLUETOOTH_C_
#include "includes.h"


int g_remote_frame_state = REMOTE_FRAME_STATE_NOK;
int g_remote_frame_cnt = 0;
int g_start_PCcontrol=0;
BYTE remote_frame_data[REMOTE_FRAME_LENGTH];
BYTE remote_frame_data_send[REMOTE_FRAME_LENGTH];
//BYTE g_device_NO = WIFI_ADDRESS_CAR_2;	/* �豸�� ��WiFi��ַ */
SWORD ans=0;


/*-----------------------------------------------------------------------*/
/* ִ��Զ������                                                          */
/*-----------------------------------------------------------------------*/
void execute_remote_cmd(const BYTE *data)
{
	BYTE cmd = 0;
	BYTE num[4]={0};//��ȡGY953����
	
	cmd = (BYTE)(data[0]);
	switch (cmd)
	{
		
		/* ��ʼ��λ������ */
		case CMD_START_CONTROL:
			g_start_PCcontrol=1;
		break;
		
		/* ��λ�����ƽ��� */
		case CMD_STOP_CONTROL:
			g_start_PCcontrol=0;
		break;
		
		/* ���������Ǳ궨���� */
		case CMD_SET_PITCH_ANGLE_ZERO :
			set_PITCH_angle_zero(*((SWORD *)(&(data[2]))));
			LCD_PrintoutInt(0, 6, angle_data.PITCH_angle_zero);
		break;
		case CMD_SET_PITCH_ANGLE_SPEED_ZERO :
			set_PITCH_angle_speed_zero(*((SWORD *)(&(data[2]))));
			LCD_PrintoutInt(64, 6, angle_data.PITCH_anglespeed_zero);
		break;
		
		case CMD_SET_ANGLE_KP :
			set_angle_KP(*((SWORD *)(&(data[2]))));
			LCD_PrintoutInt(0, 6, data_angle_pid.p);
			yaw_pwm_0-=10;
			LCD_PrintoutInt(64, 6, yaw_pwm_0);
		break;
		case CMD_SET_ANGLE_KI :
			set_angle_KI(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_ANGLE_KD :
			set_angle_KD(*((SWORD *)(&(data[2]))));
			data_angle_pid.d/=10.0;
			LCD_PrintoutInt(0, 6, data_angle_pid.d*10);
			yaw_pwm_0+=10;
			LCD_PrintoutInt(64, 6, yaw_pwm_0);
		break;	
		
		/* ��������Ǳ궨���� */
		case CMD_SET_ROLL_ANGLE_ZERO :
			set_ROLL_angle_zero(*((SWORD *)(&(data[2]))));
			LCD_PrintoutInt(0, 6, angle_data.ROLL_angle_zero);
		break;
		case CMD_SET_ROLL_ANGLE_SPEED_ZERO :
			set_ROLL_angle_speed_zero(*((SWORD *)(&(data[2]))));
			LCD_PrintoutInt(64, 6, angle_data.ROLL_anglespeed_zero);
		break;
		/* ����ƽ�������� 	*/
		case CMD_SET_MOTOR2_PWM_TARGET :	//�������� 5A 5A 14 01 01 CA
			set_pwm2_target(*((SWORD *)(&(data[2]))));
			g_f_all_start=1;
			g_f_dis_count_control=1;
			g_turn_state=0;
			g_turn_count=0;
			yaw_angle_target=GYRead[4];
//			dis_count=0;
			StopLight_1=0;
		break;
		case CMD_SET_ROLL_KP :
			set_ROLL_KP(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_ROLL_KI :
			set_ROLL_KI(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_ROLL_KD :
			set_ROLL_KD(*((SWORD *)(&(data[2]))));
		break;
		
		
		/* ����������Ǳ궨���� */
		case CMD_SET_YAW_ANGLE_ZERO :
			set_yaw_angle_target(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_YAW_ANGLE_SPEED_ZERO :
			set_YAW_angle_speed_zero(*((SWORD *)(&(data[2]))));
			set_yaw_angle_target(-*((SWORD *)(&(data[2])))); //������Ŀ��ֵ
		break;
		/* ����ǵ������ 	*/
		case CMD_SET_MOTOR3_PWM_TARGET :
			set_pwm3_target(*((SWORD *)(&(data[2]))));
			LCD_PrintoutInt(0, 0, GYRead[4]*10); //��ʾ��ǰƫ���ǵ�ֵ
			LCD_PrintoutInt(64,0,AngleCalculate[2]);
			LCD_PrintoutInt(0,2,data_encoder1.speed_real);
			LCD_PrintoutInt(64,2,g_turn_count);
			LCD_PrintoutInt(0,4,data_speed_settings.speed_target);
			LCD_PrintoutInt(64,4,yaw_angle_target);
			LCD_PrintoutInt(0,6,dis_count/10000);			
			LCD_PrintoutInt(64,6,dis_count%10000);
		break;
		/* Yaw PID����*/
		case CMD_SET_YAW_KP:
			set_Yaw_KP(*((SWORD *)(&(data[2]))));
			LCD_PrintoutInt(64, 6, data_YAW_angle_pid.p);
		break;
		case CMD_SET_YAW_KD:
			set_Yaw_KD(*((SWORD *)(&(data[2]))));
			LCD_PrintoutInt(64, 6, data_YAW_angle_pid.d);
		break;
		case CMD_SET_YAW_KI:
			set_Yaw_KI(*((SWORD *)(&(data[2]))));
			LCD_PrintoutInt(64, 6, data_YAW_angle_pid.i);
		break;
			
			
		
		/* �����ֵ������ */
		case CMD_SET_MOTOR1_TARGET :
			set_speed_target(*((SWORD *)(&(data[2]))));
		break;
		case CMD_STOP_SPEED :
			set_speed_target((SWORD)0);
			
		break;
		case CMD_SET_MOTOR1_PWM_TARGET:
			set_pwm1_target(*((SWORD *)(&(data[2]))));
		break;
		
		case CMD_SET_MOTOR1_KP :
			set_speed_KP(*((SWORD *)(&(data[2]))));
			//LCD_PrintoutInt(0,6,data_speed_pid.p);
			LCD_PrintoutInt(0, 4, data_speed_pid.p);
		break;
		case CMD_SET_MOTOR1_KI :
			set_speed_KI(*((SWORD *)(&(data[2]))));
		break;
		case CMD_SET_MOTOR1_KD :
			set_speed_KD(*((SWORD *)(&(data[2]))));
			data_speed_pid.d/=10.0;
			LCD_PrintoutInt(64, 4, data_speed_pid.d*10);
		break;
		/*GY953У׼*/
		case CMD_ACC_GYRO_ADJUST:
			GY953_ACC_GYRO_ADJUST();
		break;
		case CMD_MAG_ADJUST:
			GY953_MAG_ADJUST();
			Read_Precision(num);
			while(num[2]!=0x03){};
		break;
		
		case CMD_SET_VACANT1:
			yaw_angle_target=GYRead[4];
			data_YAW_angle_pid.p=15;					//PID����
			data_YAW_angle_pid.i=1;
			data_YAW_angle_pid.d=2;
		break;
		case CMD_SET_VACANT2:
			//yaw_angle_target=GYRead[4];
			data_YAW_angle_pid.p=0;					//PID����
			data_YAW_angle_pid.i=0;
			data_YAW_angle_pid.d=0;
		break;
		
		

		

		
#if 0		
		/* ����   ����ͨ��	*/
		case CMD_NET :
		g_net_control_data.is_new_cmd = 1;
		g_net_control_data.cmd = *((WORD *)(&(data[2])));
		break;
#endif		

	}
}


/*-----------------------------------------------------------------------*/
/* ����Զ������֡                                                        */
/* �ڶ���                                                                */
/* �޸ĵ�ַλ����:Դ��ַ(1B)+Ŀ�ĵ�ַ(1B)                                */
/* 5A 5A ��������(1B) ����(1B) ������ У����(1B)              */
/* δ�ı�ԭ����֡�Ĵ󲿷ֶ��弰����                                      */
/*-----------------------------------------------------------------------*/
int rev_remote_frame_2(BYTE rev)
{
	
	if (g_remote_frame_cnt == 0)	//����֡ͷ
	{
		if (rev == 0x5A)
		{
			remote_frame_data[g_remote_frame_cnt++] = 0x5A;
		}
	}
	else if (g_remote_frame_cnt == 1)	//����֡ͷ
	{
		if (rev == 0x5A)
		{
			remote_frame_data[g_remote_frame_cnt++] = 0x5A;
		}
		else
		{
			g_remote_frame_cnt = 0;
		}
	}
	else if (g_remote_frame_cnt == 2)	//������������
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
	}
	else if (g_remote_frame_cnt == 3)	//���ճ���
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;

		if (rev+5>REMOTE_FRAME_LENGTH)	//�ж��Ƿ�ᵼ�»��������
		{
			g_remote_frame_cnt = 0;
		}
	}
	else if (g_remote_frame_cnt>3 && g_remote_frame_cnt<=remote_frame_data[3]+3)	//����������
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
	}
	else if (g_remote_frame_cnt==remote_frame_data[3]+4)	//����У���ֽ�	
	{
		BYTE sum;
		remote_frame_data[g_remote_frame_cnt++] = rev;
		sum = check_sum((const BYTE *)(remote_frame_data), (BYTE)(remote_frame_data[3]+4));
		if (sum != remote_frame_data[remote_frame_data[3]+4])
		{
			g_remote_frame_cnt = 0;	//CheckSum Fail
		}
		else
		{
			g_remote_frame_cnt = 0;
			g_remote_frame_state = REMOTE_FRAME_STATE_OK;	//CheckSum Success
		}
	}
	
	return g_remote_frame_state;
#if 0
	uint8_t num[4]={0};
	if (g_remote_frame_cnt == 0)	//����֡ͷ
	{
		if (rev == 0xA5)
		{
			D6=~D6;
			remote_frame_data[g_remote_frame_cnt++] = 0xA5;
		}
	}
	else if (g_remote_frame_cnt == 1)	//������������
	{
		remote_frame_data[g_remote_frame_cnt++] = rev;
	}
	else if (g_remote_frame_cnt == 2)	//������������
	{
		BYTE sum;
		remote_frame_data[g_remote_frame_cnt++] = rev;
		sum = check_sum((const BYTE *)(remote_frame_data), 2);
		if (sum != remote_frame_data[2])
		{
			g_remote_frame_cnt = 0;	//CheckSum Fail
		}
		else
		{
			D7=~D7;
			g_remote_frame_cnt = 0;
			g_remote_frame_state = REMOTE_FRAME_STATE_OK;	//CheckSum Success
		}
	}
	LCD_Write_Num(80,1,remote_frame_data[0],5);
	LCD_Write_Num(80,2,remote_frame_data[1],5);
	LCD_Write_Num(80,3,remote_frame_data[2],5);
	switch(remote_frame_data[1])
	{
	case 0x57:GY953_Write(0x02,0x15);break;
	case 0x58:GY953_Write(0x02,0x19);break;
	case 0x75:Read_Precision(num);generate_remote_frame_2(PREC_TYPE,4,num);break;
	default:break;
	}
	
	return g_remote_frame_state;
#endif
}


/*-----------------------------------------------------------------------*/
/* ���ɲ�������������λ��                                                    */
/* 5A 5A ��������(1B) ����(1B) ������   У���(1B)	*/
/* ���� :   type ������������                                                 */
/*        length data����                                                */
/*        data������������                                   */
/*		     У��λ��������ȫ��λ�ĺ�У��   */
/*-----------------------------------------------------------------------*/
void generate_remote_frame_2(BYTE type, BYTE length, const BYTE data[])
{
	WORD i = 0, j = 0;
	remote_frame_data_send[i++] = 0x5A;
	remote_frame_data_send[i++] = 0x5A;
	remote_frame_data_send[i++] = type;
	remote_frame_data_send[i++] = length;
	for (j = 0; j < length; j++)
	{
		remote_frame_data_send[i++] = data[j];
	}
	remote_frame_data_send[i] = check_sum(remote_frame_data_send, length+4);
	
	serial_port_1_TX_array(remote_frame_data_send, length+5);
}


/*-----------------------------------------------------------------------*/
/* ����GY953������PC��λ��                                                          */
/* ����generate_remote_frame_2()                                          */
/* sensor����������
/* datatype��������
/*-----------------------------------------------------------------------*/
void send_data2PC(BYTE sensor, BYTE type, BYTE data[])
{
	if(sensor==ENC03)
	{
		if(type==GYR_TYPE)
			generate_remote_frame_2( type, 2, (const BYTE *)(&data[1]));
		else if(type==ANGLE_TYPE)
			generate_remote_frame_2( type, 2, (const BYTE *)(&data[0]));
	}
	else if(sensor==MPU9250)
	{
		if(type==ACC_TYPE)
			generate_remote_frame_2( type, 6, (const BYTE *)(&data[6]));
		else if(type==GYR_TYPE)
			generate_remote_frame_2( type, 6, (const BYTE *)(&data[0]));
	}
	else if(sensor==GY953)
		{
		if(type==ACC_TYPE)
			generate_remote_frame_2( type, 6, (const BYTE *)(&data[2]));
		else if(type==GYR_TYPE)
			generate_remote_frame_2( type, 6, (const BYTE *)(&data[8]));
		else if(type==ANGLE_TYPE)
			generate_remote_frame_2( type, 6, (const BYTE *)(&data[20]));
		else if(type==MAG_TYPE)
			generate_remote_frame_2( type, 6, (const BYTE *)(&data[14]));
		else if(type==FOUR_TYPE)
			generate_remote_frame_2( type, 8, (const BYTE *)(&data[26]));
		}
	
}


/*-----------------------------------------------------------------------*/
/* ���У��                                                                            */
/*-----------------------------------------------------------------------*/
BYTE check_xor(const BYTE *data, BYTE length)
{
	int i;
	BYTE res = 0;
	
	for (i=0; i<length; i++)
	{
		res ^= data[i];
	}
	
	return res;
}

/*-----------------------------------------------------------------------*/
/* �ۼӺ�У��                                                                            */
/*-----------------------------------------------------------------------*/
BYTE check_sum(const BYTE *data, BYTE length)
{
	int i;
	WORD res = 0;
	
	for (i=0; i<length; i++)
	{
		res += data[i];
		res  = res&0x00FF;
	}
	
	return (BYTE)res;
}



