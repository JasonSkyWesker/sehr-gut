#ifndef MPU9250_H_
#define MPU9250_H_

#define	SMPLRT_DIV		0x19	//�����ǲ����ʣ�����ֵ��0x07(125Hz)
#define	CONFIG			0x1A	//��ͨ�˲�Ƶ�ʣ�����ֵ��0x06(5Hz)
#define	GYRO_CONFIG		0x1B	//�������Լ켰������Χ������ֵ��0x18(���Լ죬2000deg/s)
#define	ACCEL_CONFIG	0x1C	//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ��0x01(���Լ죬2G��5Hz)
#define	ACCEL_CONFIG2	0x1D    //mpu9250����	�������ü��ٶȼƵĵ�ͨ�˲�Ƶ��
#define SELF_TEST_X_GYRO	0x00
#define SELF_TEST_Y_GYRO	0x01
#define SELF_TEST_Z_GYRO	0x02
#define SELF_TEST_X_ACCEL	0x03
#define SELF_TEST_Y_ACCEL	0x04
#define SELF_TEST_Z_ACCEL	0x05
//#define FIFO_EN			0x23
#define ACCEL_XOUT_H	0x3B
#define ACCEL_XOUT_L	0x3C
#define ACCEL_YOUT_H	0x3D
#define ACCEL_YOUT_L	0x3E
#define ACCEL_ZOUT_H	0x3F
#define ACCEL_ZOUT_L	0x40
#define GYRO_XOUT_H 	0x43
#define GYRO_XOUT_L		0x44
#define GYRO_YOUT_H		0x45
#define GYRO_YOUT_L		0x46
#define GYRO_ZOUT_H		0x47
#define GYRO_ZOUT_L		0x48
#define	PWR_MGMT_1		0x6B	//��Դ��������ֵ��0x00(��������)
#define PWR_MGMT_2		0x6c
#define SIGNAL_PATH_RESET	0x68
#define USER_CTRL		0x6A
//#define	WHO_AM_I		0x75	//IIC��ַ�Ĵ���(Ĭ����ֵ0x68��ֻ��)
#define DETECT_CTRL      0x6a
#define TEMP_OUT_H		0x41
#define TEMP_OUT_L		0x42


extern int Reg_Read(uint8_t reg,uint8_t* Data);
extern int Reg_Write(uint8_t reg,uint8_t Data);
extern void init_MPU9250(void);
extern int Read_GYRO_ACC(int32_t *Data);
extern void deviation_adjust_accx(int32_t *xdev,int32_t *ydev,int32_t *zdev/*,int32_t *xA*/);
extern void Read_Temp(int32_t* Temp);
#endif /* INIT_MPU9250_H_ */
