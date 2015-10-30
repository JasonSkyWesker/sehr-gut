/*
 * IMUUpdate.h
 *
 *  Created on: Aug 22, 2015
 *      Author: Paul
 */

#ifndef IMUUPDATE_H_
#define IMUUPDATE_H_

extern float SEq_1, SEq_2, SEq_3, SEq_4; 		// estimated orientation quaternion elements with initial conditions

extern void filterUpdate(float w_x, float w_y, float w_z, float a_x, float a_y, float a_z)  ;
extern void init_quaternion(void);
extern float invSqrt(float x); 
extern void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

#endif /* IMUUPDATE_H_ */
