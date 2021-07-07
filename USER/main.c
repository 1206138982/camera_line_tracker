#include "AllHead.h"

#if defined(BIKING) && BIKING
#include "l298n.h"
#if defined(PID_METHOD) && PID_METHOD
pid_struct	line_pid;
pid_struct	oK_pid;
#endif
#endif
void JTAG_Set(u8 mode);
u8 RUNNING = 1;
u32 timerCounter = 0;  // used for TIM3_Int_Init()  TIM3_IRQHandler()

int main(void)
{	 
	JTAG_Set(0x01);
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为 115200	
	// LED_Init();		  			//初始化与LED连接的硬件接口  init the PB5 and PE5
	// KEY_Init();					//初始化按键  init the PE2 PE3 PE4 PA0
	// TIM3_Int_Init(TIMERUNIT,7199);//进入定时器中断，增加计数值，后续的值可以根据这个定时器做定时采集发送  /*LED1 PE5 ON OFF when timeout*/
	TIM6_Int_Init(10000,7199);	//10Khz计数频率,1秒钟中断  printf N fps message in timer.c  N frames every second  result:15fps
#if defined(LCD_ON_OFF) && LCD_ON_OFF
	LCD_Init();			   		//初始化LCD  
	POINT_COLOR=RED;			//设置字体为红色 
	LCD_ShowString(30,230,200,16,16,"OV7670 Init...");	  
#endif
	// GPIO_SetBits(GPIOD,GPIO_Pin_2);  //	be init in  OV7670_Init() used for LED tishi when OV7670_Init()
	/*注意！这里要是初始化失败的话，会一直停在这里*/
	//一直初始化，直到成功
	while(OV7670_Init())//初始化OV7670
	{
		// PDout(2) = 0;
#if defined(LCD_ON_OFF) && LCD_ON_OFF
		LCD_ShowString(30,230,200,16,16,"OV7670 Error!!");
		delay_ms(200);
		LCD_Fill(30,230,239,246,WHITE);
		delay_ms(200);
#endif
		// PDout(2) = 1;
	}
	// PDout(2) = 0;
#if defined(LCD_ON_OFF) && LCD_ON_OFF
	LCD_ShowString(30,230,200,16,16,"OV7670 Init OK");
#endif
	delay_ms(500);	 	   
									
	EXTI11_Init();		// PB15 for ov7670 VSYNC interput pin in exti.c
	OV7670_Window_Set(12,176,240,320); 
	OV7670_CS=0;					
#if defined(LCD_ON_OFF) && LCD_ON_OFF
	LCD_Clear(BLACK); 
#endif
	// OLED_Init();//OLED初始化和提示信息显示	
	// OLED_Display_On();
    // myOledShowInfo(0,0,"Hello world!",CLEANOLED);
	// OLED_Refresh_Gram();
	delay_ms(500);
#if defined(BIKING) && BIKING
	Motor_start();
	RUNNING = 1;
#if defined(PID_METHOD) && PID_METHOD
	PID_init(&line_pid,1.6,0.07,5);
	PID_init(&oK_pid,80,5,90);
/*   可以稳定的走直线
	PID_init(&line_pid,1.6,0.07,5);
	PID_init(&oK_pid,80,5,90);
*/
#endif
#if defined(START_TEST) && START_TEST
	motor_test();
	RUNNING = 0;
#endif
#endif
	while(1)
	{
#if defined(BIKING) && BIKING
		if(RUNNING){
#endif
			// changMidGrey();//按键阀值调整  check the key input
			cameraOperation();//摄像头更新显示以及后续的图像处理操作接口
#if defined(BIKING) && BIKING
		}
		else{
			Motor_Stop();
			printf("monitor stop\r\n");
			delay_ms(1000);
		} 
#endif
	}
}

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
