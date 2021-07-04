#include "AllHead.h"

#if defined(BIKING) && BIKING
#include "l298n.h"
#if defined(SIMPLE_METHOD) && SIMPLE_METHOD
#else
pid_struct	line_pid;
pid_struct	oK_pid;
#endif
#endif

u8 RUNNING = 1;
u32 timerCounter = 0;  // used for TIM3_Int_Init()  TIM3_IRQHandler()

/*��ֱ��ѭ��������ȫ������*/
int cmdByLine = ERR;	//the slope value get from getCmdBySlope()

/*��Ե��Ժ�ʹ�ó�ͻ����*/
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
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ 115200	
	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();					//��ʼ������
#if defined(BIKING) && BIKING
#else
	
	TIM3_Int_Init(TIMERUNIT,7199);//���붨ʱ���жϣ����Ӽ���ֵ��������ֵ���Ը��������ʱ������ʱ�ɼ�����
#endif
	TIM6_Int_Init(10000,7199);	//10Khz����Ƶ��,1�����ж�  printf N fps message in timer.c  N frames every second  result:15fps
	LCD_Init();			   		//��ʼ��LCD  
	POINT_COLOR=RED;			//��������Ϊ��ɫ 
	LCD_ShowString(30,230,200,16,16,"OV7670 Init...");	  
	// GPIO_SetBits(GPIOD,GPIO_Pin_2);  //	be init in  OV7670_Init() used for LED tishi when OV7670_Init()
	/*ע�⣡����Ҫ�ǳ�ʼ��ʧ�ܵĻ�����һֱͣ������*/
	//һֱ��ʼ����ֱ���ɹ�
	while(OV7670_Init())//��ʼ��OV7670
	{
		// PDout(2) = 0;
		LCD_ShowString(30,230,200,16,16,"OV7670 Error!!");
		delay_ms(200);
		LCD_Fill(30,230,239,246,WHITE);
		delay_ms(200);
		// PDout(2) = 1;
	}
	// PDout(2) = 0;
	
	LCD_ShowString(30,230,200,16,16,"OV7670 Init OK");
	delay_ms(500);	 	   
									
	EXTI11_Init();		// PB15 for ov7670 VSYNC interput pin in exti.c
	OV7670_Window_Set(12,176,240,320); 
	OV7670_CS=0;					
	LCD_Clear(BLACK); 

	// OLED_Init();//OLED��ʼ������ʾ��Ϣ��ʾ	
	// OLED_Display_On();
    // myOledShowInfo(0,0,"Hello world!",CLEANOLED);
	// OLED_Refresh_Gram();

	delay_ms(500);
		
    /*test*/
	// cameraOperation();
	// printToUart();
#if defined(BIKING) && BIKING
	Motor_start();
	RUNNING = 1;
#if defined(SIMPLE_METHOD) && SIMPLE_METHOD
#else
	PID_init(&line_pid,1.2,0.05,2.5);   //PID_init(&line_pid,1.2,0.03,2.5);
	PID_init(&oK_pid,50,5,70);    //PID_init(&oK_pid,50,3,70);
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
			changMidGrey();//������ֵ����
			cameraOperation();//����ͷ������ʾ�Լ�������ͼ��������ӿ�
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
