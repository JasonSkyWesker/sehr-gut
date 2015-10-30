#ifndef __CONTROL_H__
#define __CONTROL_H__


/* 方向舵机PWM */
#define STEER_HELM_CENTER (3301)
#define STEER_HELM_TEST (1500)
#define STEER_HELM_LEFT (2247)
#define STEER_HELM_RIGHT (4292)
#define STEER_HELM_MIX (1038)
#define STEER_HELM_MAX (6146)

/* 信号舵机PWM */
#define SINGLE_HELM_CENTER (50)

/* 速度PWM */
#define SPEED_PWM_MIN (1)
#define SPEED_PWM_MAX (1500)
#define SPEED_PWM_TEST (20)

/* 速度光编 */
#define SPEED_ENCODER_STOP (0x0000)
#define SPEED_ENCODER_MIN (0x0002)
#define SPEED_ENCODER_MAX (0x0110)
#define SPEED_ENCODER_TEST (11)

/*光编距离计数*/
extern int Range;

extern float speed_pwm_0;

extern int g_f_pit;
extern int g_f_enable_speed_control;
extern int g_f_enable_pwm_control;	/* 启用开环速度控制标志位 */
extern int g_pit_cnt;//电机控制时间间隔记数

/*全局开始*/
extern int g_f_all_start;

/*光编距离计数*/
extern int dis_count;
extern int dis_count_last;
extern int g_f_dis_count_control;

//转向
extern int g_turn_count;
extern int g_turn_state;
extern float g_turn_start_angle;
extern float yaw_pwm;
extern float yaw_pwm_0;
extern int counter;
extern float  angle_pwm;
extern BYTE speed_period;

extern float temp_p,temp_d;
extern float maxep,maxecp;
extern float maxen,maxecn;
extern void getmax(void);
/*前进方向全局变量*/
extern float CarYawInitial;
extern float CarYawspeedInitial;

/* 时间基准 */
#ifdef __CONTROL_C_
DWORD g_time_basis_PIT;
#else
extern const DWORD g_time_basis_PIT;
#endif


/* 光编1数据 */
#ifdef __CONTROL_C_
struct
{
	WORD cnt_old;
	WORD cnt_new;
	WORD speed_now;
	int  speed_real;
	SWORD is_forward;
} data_encoder1 = { 0x0000, 0x0000, 0x0000, 0,0 };
#else
extern struct
{
	WORD cnt_old;
	WORD cnt_new;
	WORD speed_now;
	int  speed_real; 
	SWORD is_forward;
} data_encoder1;
#endif

/* 光编2数据 */
#ifdef __CONTROL_C_
struct
{
	WORD cnt_old;
	WORD cnt_new;
	WORD speed_now;
	int  speed_real;
	SWORD is_forward;
} data_encoder2 = { 0x0000, 0x0000, 0x0000, 0,0 };
#else
extern struct
{
	WORD cnt_old;
	WORD cnt_new;
	WORD speed_now;
	int  speed_real; 
	SWORD is_forward;
} data_encoder2;
#endif

/* 速度PID数据 */
#ifdef __CONTROL_C_
struct
{
	float p;
	float i;
	float d;
} data_speed_pid = {60 , 0, 1 };// 速度10：p=40，速度20：p=45 
#else
extern struct
{
	float p;
	float i;
	float d;
} data_speed_pid;
#endif
/* 角度PID数据 */
#ifdef __CONTROL_C_
struct
{
	float p;
	float i;
	float d;
} data_angle_pid = { 450, 0, 1};
#else
extern struct
{
	float p;
	float i;
	float d;
} data_angle_pid;
#endif


/*平衡PID数据*/
#ifdef __CONTROL_C_
struct
{
	float p;
	float i;
	float d;
} data_ROLL_angle_pid = { 100, 0, 6 };//三叶小风扇
#else
extern struct
{
	float p;
	float i;
	float d;
} data_ROLL_angle_pid;
#endif

/*转向PID数据*/
#ifdef __CONTROL_C_
struct
{
	float p;
	float i;
	float d;
} data_YAW_angle_pid = { 0, 0, 0 };
#else
extern struct
{
	float p;
	float i;
	float d;
} data_YAW_angle_pid;
#endif
/* 速度设置数据 */
#ifdef __CONTROL_C_
struct
{
	SWORD speed_target;
	SWORD speed_target_now;
} data_speed_settings = { 0x0000, 0x0000 };
#else
extern struct
{
	SWORD speed_target;
	SWORD speed_target_now;
} data_speed_settings;
#endif

/* 三电机PWM设置数据 */
#ifdef __CONTROL_C_
struct
{
	SWORD motor_1_pwm;
	SWORD motor_2_pwm;
	SWORD motor_3_pwm;
} motor_pwm_settings = { 1, 1, 1 };
#else
extern struct
{
	SWORD motor_1_pwm;
	SWORD motor_2_pwm;
	SWORD motor_3_pwm;
} motor_pwm_settings;
#endif

extern void PitISR(void);
extern void get_speed_now();

extern void motor_control_balance(void);

extern void set_PITCH_motor_pwm(int16_t speed_pwm);
extern void PITCH_motor_control(void);
extern void set_ROLL_motor_pwm(int16_t motor_pwm);
extern void set_YAW_motor_pwm(int16_t motor_pwm);
extern void ROLL_motor_control(void);

extern void contorl_speed_encoder_bb(void);
extern void  AngleControl(void);
extern void contorl_speed_encoder_pid(void);
extern void set_speed_pwm(void);
extern void set_speed_target(SWORD speed_target);
extern void set_speed_PID(void);
extern void set_speed_KP(float kp);
extern void set_speed_KI(float ki);
extern void set_speed_KD(float kd);


extern void BalanceControl(void);
extern void Balance_Control_HELM(void);
void Fuzzypid_Control(float *tp,float *td);

extern void set_PropellerA_motor_pwm(int16_t motor_pwm);
extern void set_PropellerB_motor_pwm(int16_t motor_pwm);
extern void PropellerA_Control(void);
extern void PropellerB_Control(void);

extern void set_pwm1_target(SWORD speed_pwm);
extern void set_pwm2_target(SWORD speed_pwm);
extern void set_pwm3_target(SWORD speed_pwm);
extern void set_angle_KP(float kp);
extern void set_angle_KI(float kp);
extern void set_angle_KD(float kp);
extern void set_ROLL_KP(float kp);
extern void set_ROLL_KI(float kp);
extern void set_ROLL_KD(float kp);
extern void set_Yaw_KP(float kp);
extern void set_Yaw_KD(float kd);
extern void set_Yaw_KI(float ki);

extern void YawControl(void);
extern void set_yaw_angle_target(float yaw_target);
extern void Propeller_YawControl(void);

extern void test_pwm(void);

extern DWORD diff_time_basis_PIT(const DWORD new_time, const DWORD old_time);
//extern int abs(int data);

#define CarSpeedConstant 10
#define SPEED_CONTROL_PERIOD 100








#endif
