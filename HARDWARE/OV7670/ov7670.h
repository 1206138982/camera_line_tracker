#ifndef _OV7670_H
#define _OV7670_H
#include "sys.h"
#include "sccb.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序参考自网友guanfu_wang代码。
//ALIENTEK战舰STM32开发板V3
//OV7670 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/1/18
//版本：V1.0		    							    							  
//////////////////////////////////////////////////////////////////////////////////
 
#define OV7670_VSYNC  	PBin(11)			//同步信号检测IO
#define OV7670_WRST		PCout(15)		//写指针复位
#define OV7670_WREN		PBout(10)		//写入FIFO使能
#define OV7670_RCK_H	GPIOB->BSRR=1<<1//设置读数据时钟高电平
#define OV7670_RCK_L	GPIOB->BRR=1<<1	//设置读数据时钟低电平
#define OV7670_RRST		PCout(1)  		//读指针复位
#define OV7670_CS		PCout(3)  		//片选信号(OE)


#define OV7670_DATA   GPIOA->IDR&0x00FF  					//数据输入端口
/////////////////////////////////////////									
	    				 
u8   OV7670_Init(void);		  	   		 
void OV7670_Light_Mode(u8 mode);
void OV7670_Color_Saturation(u8 sat);
void OV7670_Brightness(u8 bright);
void OV7670_Contrast(u8 contrast);
void OV7670_Special_Effects(u8 eft);
void OV7670_Window_Set(u16 sx,u16 sy,u16 width,u16 height);


#endif





















