#ifndef __INCLUDES_H__
#define __INCLUDES_H__

#ifndef _TYPEDEF_BYTE_
#define _TYPEDEF_BYTE_
typedef unsigned char BYTE;
#endif

#ifndef _TYPEDEF_SBYTE_
#define _TYPEDEF_SBYTE_
typedef signed char SBYTE;
#endif

#ifndef _TYPEDEF_WORD_
#define _TYPEDEF_WORD_
typedef unsigned short WORD;
#endif

#ifndef _TYPEDEF_SWORD_
#define _TYPEDEF_SWORD_
typedef signed short SWORD;
#endif

#ifndef _TYPEDEF_DWORD_
#define _TYPEDEF_DWORD_
//typedef unsigned int DWORD;
typedef unsigned long int DWORD;
#endif

#if 0
#ifndef _TYPEDEF_byte_
#define _TYPEDEF_byte_
typedef unsigned char byte;
#endif

#ifndef _TYPEDEF_word_
#define _TYPEDEF_word_
typedef unsigned short word;
#endif
#endif

#include "MPC5604B.h"
#include "init.h"
#include "control.h"
#include "time.h"
#include "lcd.h"
#include "UART.h"
#include "IntcInterrupts.h"
#include "stepmotor.h"
#include "angle.h"
#include "MPU9250.h"
#include "analyse.h"
#include "matrix.h"
#include "GY953.h"
#include "Bluetooth.h"
#include "debug.h"
#include <math.h>
#include "IMUUpdate.h"




///****************第二版载LED&车灯****************///
#if 1
#define StopLight_1 SIU.GPDO[16].B.PDO
#define StopLight_2 SIU.GPDO[17].B.PDO
#endif


/***************DIP*****************/
#define switch1 SIU.GPDI[48].R
#define switch2 SIU.GPDI[49].R
#define switch3 SIU.GPDI[50].R
#define switch4 SIU.GPDI[51].R


#endif
