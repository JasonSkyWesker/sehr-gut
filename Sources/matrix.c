/*
 * matrix.c
 *
 *  Created on: Jul 19, 2015
 *      Author: Paul
 */
#include "includes.h"

//--------------------------------------------------------
//功能：求矩阵a和b的相乘结果
//入口参数：矩阵首地址 a和b；矩阵a行数ra和列数ca；矩阵b的行数rb和列数cb
//返回值：矩阵a和b的相乘结果
//--------------------------------------------------------
void m_multiply(float a[], float b[], float c[], int ra, int ca, int rb, int cb)
{
	int i,j,k;
	int32_t sum;
	for(i=0;i<ra;i++)
	{
		for(j=0;j<cb;j++)
		{
			sum=0;
			for(k=0;k<ca;k++)
			{
				sum+=*(a+i*ca+k)*(*(b+k*cb+j));
			}
			*(c+i*ra+j)=sum;
		}
	}

}

void m_scalar(float A[],float b,float B[],int m,int n)
{
	int i;
	for(i=0;i<m*n;i++)
	{
		B[i]=b*A[i];
	}
}

void m_add(float A[],float B[],float C[],int m,int n)
{
	int i=0;
	for(;i<m*n;i++)
	{
		C[i]=A[i]+B[i];
	}
}


