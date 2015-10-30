﻿#ifndef __INIT_H__
#define __INIT_H__


#define PERIPH_SET_1_CLK_16M
#define STEER_HELM_DATA (EMIOS_0.CH[9].CBDR.R)

extern int mode;


/* 设备初始化状态 */
#ifdef __INIT_C_
struct
{
	int TFCard_is_OK;
	int L3G4200D_is_OK;
	int RFIDCard_is_OK;
	int OLED_is_OK;
	int WiFi_commonication_is_OK;
	int RFIDCard_energetic_mode_enable_is_OK;
} g_devices_init_status = { 0, 0, 0, 0, 0, 0, };
#else
extern struct
{
	int TFCard_is_OK;
	int L3G4200D_is_OK;
	int RFIDCard_is_OK;
	int OLED_is_OK;
	int WiFi_commonication_is_OK;
	int RFIDCard_energetic_mode_enable_is_OK;
} g_devices_init_status;
#endif


extern void init_modes_and_clock(void);
extern void disable_watchdog(void);
extern void initEMIOS_0MotorAndSteer(void);
extern void init_led(void);
extern void init_pit_10ms(void);
extern void init_pit_1ms(void);
extern void init_encoder(void);
extern void enable_irq(void);
extern int is_big_endian();
extern void init_DIP(void);
extern void init_key(void);
extern void init_choose_mode(void);
extern void init_ADC(void);
void test_init_TF();
void test_xyz_gyro();
void read_device_no();
void read_display_helm();
void read_DIP_mode();
void init_speed_control();
void init_DSPI_1(void);
extern void enable_pit_1ms(void);
extern void disable_pit_1ms(void);










extern void init_optical_encoder(void);
extern void delay_us(DWORD us);
extern void delay_ms(DWORD ms);
extern void init_all_and_POST(void);
extern void suicide(void);


#endif
