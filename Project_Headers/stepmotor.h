#ifndef STEPMOTOR_H_
#define STEPMOTOR_H_



/*	步进电机1	*/
#define SMotor1_A SIU.GPDO[6].B.PDO		//PA6
#define SMotor1_B SIU.GPDO[7].B.PDO		//PA7
#define SMotor1_C SIU.GPDO[8].B.PDO		//PA8
#define SMotor1_D SIU.GPDO[9].B.PDO		//PA9

/*	步进电机2	*/
#define SMotor2_A SIU.GPDO[28].B.PDO	//PB12
#define SMotor2_B SIU.GPDO[29].B.PDO	//PB13
#define SMotor2_C SIU.GPDO[30].B.PDO	//PB14
#define SMotor2_D SIU.GPDO[31].B.PDO	//PB15

extern BYTE g_Control;
extern int stepspeed;
extern int send_timer;
extern int delay_timer;
extern int ABCD;

void Pit_1ms(void);
void init_Stepmotor(void);
void Motor_stop(void);
void Motor_A(void);
void Motor_B(void);
void Motor_C(void);
void Motor_D(void);
void stepmotor_rotate(void);
void stepmotor_video(int num);
void stepmotor_balance(void);


#endif /* STEPMOTOR_H_ */
