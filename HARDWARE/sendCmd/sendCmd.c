#include "AllHead.h"

/*针对斜率，给出命令*/
extern double overK;

extern 

/*根据斜率，开始判断直线的斜率*/
//注意，overK基本不会出现0，要是出现，那么就作废
int getCmdBySlope()
{
	  //右偏0.00001-30°左右时的斜率
	  if((overK > 0) && (overK <= 2.5))
		{
	      return RIGHT0_30;
		}
		//左偏-0.00001 - -30°左右时的斜率
		if((overK < 0) && (overK >= -2.5))
		{
			  return LEFT0_30;
		}
		
		//右偏30 -45°左右的斜率
		if((overK > 2.5) && (overK <= 3.8))
		{
        return RIGHT30_45;
		}
		if((overK < -2.5) && (overK >= -3.8))
		{
			  return LEFT30_45;
		}
		
		//右偏45-60°
		if((overK > 3.8) && (overK <= 7.1))
		{
        return  RIGHT45_60;
		}
		//左偏45-60°
		if((overK < -3.8) && (overK >= -7.1))
		{
			  return LEFT45_60;
		}
		
		/*右边超超过60度*/
		if(overK > 7.1)
		{
			  return RIGHTMORETHAN60;
		}
		
		if(overK < -7.1)
		{
			  return LEFTMORETHAN60;
		}
		//其他角度均返回错误，一般为接近水平了。
		return ERR;
}





