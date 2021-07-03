#include "AllHead.h"

double overK = 0;//反斜率
// u8 b = 0;//常数
int b = 0;//常数


/*存储最为有效的段，在左右边界中挑选出来*/
extern u8 maxUsefulBlackLine[(NEEDHEIGHT)/(SKIPLINE)];
extern u8 maxUsefulLineLen;

/*最小二乘法拟合直线*/
// int regression(u8 *maxLine,u8 *maxLen,double *oK,int *ob)
int regression(u8 *maxLine,u8 *maxLineH,u8 *maxLen,double *oK,int *ob)
{
	  /*累积变量*/
	  double x_sum = 0,y_sum = 0,xy_sum = 0,xx_sum = 0;
		double x_avr = 0,y_avr = 0,xy_avr = 0,xx_avr = 0;
		int i;
		x_sum=0;y_sum=0;xy_sum=0;xx_sum=0;
	 	
		for(i=0;i < *maxLen;i++)
		{
				x_sum+=maxLine[i];
				// y_sum+= i;
				y_sum+= (80 - maxLineH[i]);
				// xy_sum+=maxLine[i]*(i);
				xy_sum+=maxLine[i]*(80-maxLineH[i]);
				xx_sum+=maxLine[i]*maxLine[i];
		}
		x_avr=x_sum/(*maxLen);
		y_avr=y_sum/(*maxLen);
		xy_avr=xy_sum/(*maxLen);
		xx_avr=xx_sum/(*maxLen);
		
		/*公式中分子为0*/
		if((xy_avr-x_avr*y_avr)==0)
		{
			  /*****测试*****/
			  //printToUart();//在清空之前打印
			  
			  *oK = 0;
			  /*用完清除无用的最长有效段，以便下一次采集*/
			  *maxLen = 0;
	      memset(maxLine,0,(NEEDHEIGHT)/(SKIPLINE));	  
			
				return NOSLOPE;
		}
		else
		{
				*oK = (xx_avr-x_avr*x_avr)/(xy_avr-x_avr*y_avr);//反斜率
				// *oK = (xy_avr-x_avr*y_avr)/(xx_avr-x_avr*x_avr);//正斜率
				// *oK = 0 - (*oK);
//				realK=(xy_avr-x_avr*y_avr)/(xx_avr-x_avr*x_avr);//正斜率
				// *ob = maxLine[0];//获取K
				*ob = 80 - maxLineH[0]-(1/(*oK))*(maxLine[0]);//获取K
			
			  			  /*****测试*****/
			  //printToUart();//在清空之前打印
			
			   //滤除无用斜率
			  if(((*oK) >= 20) || ((*oK) <= -20))
				{
						(*oK) = 0;
				}
			
        /*用完清除无用的最长有效段，以便下一次采集*/
			  *maxLen = 0;
	      memset(maxLine,0,(NEEDHEIGHT)/(SKIPLINE));		
			  
				return GOTSLOPE;
		}
}



