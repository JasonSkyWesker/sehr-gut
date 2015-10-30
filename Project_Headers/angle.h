#ifndef ANGLE_H_
#define ANGLE_H_


#ifdef __ANGLE_C_
struct
{
	SWORD PITCH_angle_zero;
	SWORD PITCH_anglespeed_zero;
	SWORD ROLL_angle_zero;
	SWORD ROLL_anglespeed_zero;
	SWORD YAW_angle_zero;
	SWORD YAW_anglespeed_zero;
	float GY_PITCH_angle_zero;
	float GY_ROLL_angle_zero;
}angle_data = { 1932, 1586, 1488, 1973, 0, 0,-6,0};//{ 1922, 974, 1764, 1132, 0, 0 }

#else
extern struct
{
	SWORD PITCH_angle_zero;
	SWORD PITCH_anglespeed_zero;
	SWORD ROLL_angle_zero;
	SWORD ROLL_anglespeed_zero;
	SWORD YAW_angle_zero;
	SWORD YAW_anglespeed_zero;
	float GY_PITCH_angle_zero;
	float GY_ROLL_angle_zero;
}angle_data;
#endif

extern unsigned int AngleResult[6];
extern float AngleCalculate[6];
extern unsigned int AngleResult_balance[4];
extern float AngleCalculate_balance[4];
extern float GYRead[9];   //Êý×ÖÍÓÂÝÒÇ 
extern float yaw_angle_target;
extern void Gy953_angle_read(void);

void angle_read(unsigned int *AngleGra);
void angle_calculate(void);





void set_PITCH_angle_zero(SWORD zero);
void set_PITCH_angle_speed_zero(SWORD zero);
void set_ROLL_angle_zero(SWORD zero);
void set_ROLL_angle_speed_zero(SWORD zero);
void set_YAW_angle_zero(SWORD zero);
void set_YAW_angle_speed_zero(SWORD zero);


#endif /* ANGLE_H_ */
