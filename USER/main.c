#include "AllHead.h"

u32 timerCounter = 0;

/*由直线循迹发出的全局命令*/
int cmdByLine = ERR;

/*针对调试和使用冲突问题*/
void JTAG_Set(u8 mode)
{
	  u32 temp;
	  temp = mode;
	  temp <<= 25;
	  RCC->APB2ENR |= 1<<0;
	  AFIO->MAPR &= 0xF8FFFFFF;
	  AFIO->MAPR |= temp;
}

int main(void)
 {	 
	  JTAG_Set(0x01);
		delay_init();	    	 //延时函数初始化	  
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
		uart_init(115200);	 	//串口初始化为 115200	
		LED_Init();		  			//初始化与LED连接的硬件接口
		KEY_Init();					//初始化按键
	
	  TIM3_Int_Init(TIMERUNIT,7199);//进入定时器中断，增加计数值，后续的值可以根据这个定时器做定时采集发送
	
		LCD_Init();			   		//初始化LCD  
		POINT_COLOR=RED;			//设置字体为红色 
 
		LCD_ShowString(30,230,200,16,16,"OV7670 Init...");	  
	
	  GPIO_SetBits(GPIOD,GPIO_Pin_2);
	  /*注意！这里要是初始化失败的话，会一直停在这里*/
		//一直初始化，直到成功
		while(OV7670_Init())//初始化OV7670
		{
			PDout(2) = 0;
				LCD_ShowString(30,230,200,16,16,"OV7670 Error!!");
				delay_ms(200);
				LCD_Fill(30,230,239,246,WHITE);
				delay_ms(200);
			PDout(2) = 1;
		}
		PDout(2) = 0;
		
		LCD_ShowString(30,230,200,16,16,"OV7670 Init OK");
		delay_ms(500);	 	   
									  
		EXTI11_Init();						
		OV7670_Window_Set(12,176,240,320); 
		OV7670_CS=0;					
		LCD_Clear(BLACK); 
		
		
	  OLED_Init();//OLED初始化和提示信息显示
		
		OLED_Display_On();
    myOledShowInfo(0,0,"Hello world!",CLEANOLED);
		OLED_Refresh_Gram();
		
		delay_ms(500);
		
    /*test*/
//		cameraOperation();
//	  printToUart();
		
		while(1)
		{	
			  changMidGrey();//按键阀值调整
				cameraOperation();//摄像头更新显示以及后续的图像处理操作接口
				


		}
		 
}













