#ifndef __GREY_H__
#define __GREY_H__

#include "sys.h"

#define MODIFYVAL  3 //每次调整的大小
#define MAXGREY  0xE0//最大灰度值
#define MINGREY  0x0F  //最小灰度值

/*隔行扫描完成的图像大小  120宽：80高*/
#define NEEDHEIGHT  80     //高位80
#define NEEDWITH    120  //宽为120

#define MIDHORIHEIGHT (((NEEDHEIGHT)/(SKIPLINE)/2) -1)//中间水平高度位置，
#define MIDVERIWIDETH  (NEEDWITH/2)//中间垂直位置的LOC，用于便于计算偏移

/*正负跳变的差值*/
#define DOWNJUMP   254 //从白色到黑色的跳变
#define UPJUMP     -254//从黑色到白色的跳变

/*进行左右边界检测时，选择在缩小图中检测采样的间隔行数*/
#define SKIPLINE 4

#define DEVIATION 3 //边界相邻像素之间差值的差值的误差允许范围

#define MIDHORLOC (((NEEDWITH) / 2) - 1)//中间水平位置中点  59

extern void cameraRefresh(void);
extern void changMidGrey(void);
extern void printToUart(void);
extern void getLineEdge(u8 *leftBlackLoc,u8 *rightBlackLoc,u16 startLine,u16 endLine,u16 skipLine);
extern void cameraOperation(void);
extern int getUsefulLine(void);
extern void getOneSideUsefulLine(u8 *needBlackLoc,u8 countZero,u8 *maxUBlackLine,u8 *maxUBlackLineLen,u8 *maxUBlackHeight);
extern int getLineLocCompare2MidLine(int *realVerticalLoc);
extern void memsetBothBlackLoc(void);
extern int getCmdByDeviLoc(void);
extern int getLineWidth(u8 *lMaxULine,u8 *rMaxULine,u8 lMaxULen,u8 rMaxULen,u8 *lMaxLineHei,u8 *rMaxLineHei,u8 *lineWidth);

#endif
