#ifndef __ALLHEAD_H__
#define __ALLHEAD_H__

#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"	 
#include "string.h"
#include "ov7670.h"
#include "tpad.h"
#include "timer.h"
#include "exti.h"
#include "usmart.h"
#include "grey.h"
#include "leastsquare.h"
#include "math.h"
#include "sendCmd.h"
#include "oled.h"
#include "pid.h"
#include "l298n.h"
#include "move.h"

#define BIKING	1
#if defined(BIKING) && BIKING
#define SIMPLE_METHOD	1
#define PID_METHOD	0
#define FENCHA_TEST	1
#define START_TEST	0
#endif

#if defined(SIMPLE_METHOD) && SIMPLE_METHOD
#define SIMPLE_METHODNEW	0
#endif

#if defined(FENCHA_TEST) && FENCHA_TEST
#define MAP_TESTA	1
#define MAP_TESTB	0
#endif

#define DEBUG_PIN	0
#define CHANGE_PIN	1   	// change in ov7670.h when switch
#define LCD_ON_OFF	1

#if defined(LCD_ON_OFF) && LCD_ON_OFF
#define LCD_SHOW_INFO	1
#endif

enum All
{
	      OLED_CMD  = 0,
    OLED_DATA,
    NO ,
	  YES,
	  LEFT,
	  RIGHT,
	  MIDDLE,
	  LEFTLOST,
	  RIGHTLOST,
	  GETDIRECT,
	  BOTHLOST,
	  NOSLOPE,
	  GOTSLOPE,
	  RIGHT0_30,
	  RIGHT30_45,
	  RIGHT45_60,
	  RIGHTMORETHAN60,
	  LEFT0_30,
	  LEFT30_45,
	  LEFT45_60,
	  LEFTMORETHAN60,
	  ERR,
		TOOLEFT,
		TOORIGHT,
		GETMIDLOC,
		NOMIDLOC,
		RIGHTDEVI0_10,
		LEFTDEVI0_10,
		RIGHTDEVI10_20,
		LEFTDEVI10_20,
		RIGHTDEVI20_30,
		LEFTDEVI20_30,
		RIGHTDEVI30_40,
		LEFTDEVI30_40,
		RIGHTDEVMORETHAN40,
		LEFTDEVMORETHAN40,
		GOTLINEWIDTH,
		NOLINEWIDTH,
		CLEANOLED,
		NOCLEANOLED,
		
};

#define TIMERUNIT 99
#define MAXHORIZDISTANCE 80//最大的偏移量，也就是基本全都靠在左边
#define KDEV 2



#endif
