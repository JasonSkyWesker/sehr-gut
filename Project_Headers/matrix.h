/*
 * matrix.h
 *
 *  Created on: Jul 19, 2015
 *      Author: Paul
 */

#ifndef MATRIX_H_
#define MATRIX_H_



extern void m_multiply(float *a, float *b, float *c, int ra, int ca, int rb, int cb);
extern void m_scalar(float A[],float b,float B[],int m,int n);
extern void m_add(float A[],float B[],float C[],int m,int n);

#endif /* MATRIX_H_ */
