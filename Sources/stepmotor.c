#define __STEPMOTOR_C_
#include "includes.h"

int stepspeed=1000;
BYTE g_Control;
int delay_timer=-1;
int send_timer=0;
int speed_count=0;

void Pit_1ms(void) 
{
	//static int timer=0;
	//static int ABCD=0;
	//static int count=0;
	g_Control=1;
	speed_count++;
	send_timer++;
	if(delay_timer>0) 
		delay_timer--;
	if(speed_count==5)
	{
		speed_count=0;
		get_speed_now();
	}
	PIT.CH[2].TFLG.B.TIF = 1;	// MPC56xxB/P/S: Clear PIT 1 flag by writing 1
}




