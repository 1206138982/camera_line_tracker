#ifndef __OLED_H
#define __OLED_H	

#include "sys.h"
#include "stm32f10x.h"



//-----------------OLED端口定义---------------- 
#define OLED_RST_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_7)  //RST
#define OLED_RST_Set() GPIO_SetBits(GPIOB, GPIO_Pin_7)  //RST

#define OLED_RS_Clr() GPIO_ResetBits(GPIOB, GPIO_Pin_6)   //DC
#define OLED_RS_Set() GPIO_SetBits(GPIOB, GPIO_Pin_6)   //DC

#define OLED_SCLK_Clr()  GPIO_ResetBits(GPIOB, GPIO_Pin_9) //SCL
#define OLED_SCLK_Set()  GPIO_SetBits(GPIOB, GPIO_Pin_9)   //SCL

#define OLED_SDIN_Clr()  GPIO_ResetBits(GPIOB, GPIO_Pin_8)    //SDA
#define OLED_SDIN_Set()  GPIO_SetBits(GPIOB, GPIO_Pin_8)    //SDA




//OLED控制用函数
extern void OLED_WR_Byte(u8 dat,u8 cmd);	    
extern void OLED_Display_On(void);
extern void OLED_Display_Off(void);
extern void OLED_Refresh_Gram(void);		   				   		    
extern void OLED_Init(void);
extern void OLED_Clear(void);
extern void OLED_DrawPoint(u8 x,u8 y,u8 t);
extern void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 size,u8 mode);
extern void OLED_ShowNumber(u8 x,u8 y,u32 num,u8 len,u8 size);
extern void OLED_ShowString(u8 x,u8 y,const char *p);
extern void oledShowInit(void);
extern void myOledShowInfo(u8 x,u8 y,const char *str,u8 mode);
extern void myOledShowInfoNoRefresh(u8 x,u8 y,const char *str,u8 mode);//不带刷新


#endif  
	 
