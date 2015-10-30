/*
 * matrix.c
 *
 *  Created on: Jul 19, 2015
 *      Author: Paul
 */
#include "includes.h"

//--------------------------------------------------------
//���ܣ������a��b����˽��
//��ڲ����������׵�ַ a��b������a����ra������ca������b������rb������cb
//����ֵ������a��b����˽��
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


