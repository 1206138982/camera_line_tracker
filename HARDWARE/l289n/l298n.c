#include "l298n.h"
#include "AllHead.h"
#include "grey.h"

// u16	speed_min = 380;   // the speed_min will change to 0 unexcepted because of the u16 type,when change to int,fix it
int	speed_min = 380;  //min:400 
u16 max_add = 230;   //200 can finish the wandao in pid method
u8 monitor_error_add = 50;
extern u8 RUNNING;

void Motor_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	// RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	    //使能PA端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG, ENABLE);	    //使能PA端口时钟
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;				 //端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	// GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化PA4/5/6/7
	GPIO_Init(GPIOG, &GPIO_InitStructure);					 //根据设定参数初始化PA4/5/6/7
}

void Motor_Stop(void)
{
	Mr_P = 0;
	Mr_N = 0;
	Ml_P = 0;
	Ml_N = 0;
}

void Motor_Forward(void)
{
	if(RUNNING == 0)
		return ;
	Mr_P = 1;
	Mr_N = 0;
	Ml_P = 1;
	Ml_N = 0;
}

void Motor_Backward(void)
{
	if(RUNNING == 0)
		return ;
	Mr_P = 0;
	Mr_N = 1;
	Ml_P = 0;
	Ml_N = 1;
}

void Motor_Turnleft(void)
{
	if(RUNNING == 0)
		return ;
	Mr_P = 1;
	Mr_N = 0;
	Ml_P = 0;
	Ml_N = 0;
}

void Motor_Turnright(void)
{
	if(RUNNING == 0)
		return ;
	Mr_P = 0;
	Mr_N = 0;
	Ml_P = 1;
	Ml_N = 0;
}

void Motor_Leftback(void)
{
	if(RUNNING == 0)
		return ;
	Mr_P = 0;
	Mr_N = 0;
	Ml_P = 0;
	Ml_N = 1;
}

void Motor_Rightback(void)
{
	if(RUNNING == 0)
		return ;
	Mr_P = 0;
	Mr_N = 1;
	Ml_P = 0;
	Ml_N = 0;
}

void left_add(int add)
{
	if(RUNNING == 0)
		return ;
	if(add < 0){
		printf("error in left_add() add < 0\r\n");
		return ;
	}
	if(add > max_add){
		printf("in left_add() add:%d\tbeyond the max_add:%d\r\n",add,max_add);
		add = max_add;
		printStopMess(3);
		// RUNNING = 0;
	}
	TIM_SetCompare2(TIM3,speed_min-add);
}

void right_add(int add)
{
	if(RUNNING == 0)
		return ;
	if(add < 0){
		printf("error in right_add() add < 0\r\n");
		return ;
	}
	if(add > max_add){
		printf("in right_add() add:%d\tbeyond the max_add:%d\r\n",add,max_add);
		add = max_add;
		printStopMess(4);
		// RUNNING = 0;
	}
#if defined(MONITOR_ERROR) && MONITOR_ERROR
	TIM_SetCompare1(TIM3,speed_min-add-monitor_error_add);
#else
	TIM_SetCompare1(TIM3,speed_min-add);
#endif
}

void Motor_start(void)
{
	int sp_start=350;
	Motor_Init();
	monitor_PWM_Init(899,0);
	TIM_SetCompare1(TIM3,sp_start);   //the right monitor	PB5
	TIM_SetCompare2(TIM3,sp_start); 	// the left monitor		PB0
	right_add(0);
	Motor_Forward();
	delay_ms(10);
#if defined(MONITOR_ERROR) && MONITOR_ERROR
	TIM_SetCompare1(TIM3,speed_min-monitor_error_add);   //the right monitor	PB5
#else
	TIM_SetCompare1(TIM3,speed_min);   //the right monitor	PB5
#endif
	TIM_SetCompare2(TIM3,speed_min); 	// the left monitor		PB0
}

void motor_test(void)
{
	delay_ms(500);
	// delay_ms(1500);
	// return ;

	// left_add(200);
	// right_add(0);
	// delay_ms(1500);
	// return ;

	left_add(0);
	right_add(200);
	delay_ms(1500);
}

void turnA(void)
{
	if(RUNNING == 0)
		return ;
	Motor_Forward();
	delay_ms(400);
	Motor_Turnright();
	delay_ms(800);
	Motor_Stop();
	// delay_ms(1500);
	// delay_ms(1500);
	// RUNNING = 0;
}

//PWM 部分初始化 
//arr：自动重装值
//psc：时钟预分频数
void Motor_PWM_Init(u16 arr,u16 psc)
{  
	// GPIO_PinRemapConfig(GPIO_Remap_SWJ_NoJTRST, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);	//使能定时器4时钟
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);  //使能GPIO外设和AFIO复用功能模块时钟
	
	GPIO_PinRemapConfig(GPIO_Remap_TIM4, ENABLE); //Timer4重映射 TIM4_CH3->PB8    TIM4_CH4->PB9
 
   //设置该引脚为复用输出功能,输出TIM_CH1和TIM_CH2和的PWM脉冲波形
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9; //TIM_CH3和TIM_CH4 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM4 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);  //初始化外设TIM4 OC3
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);  //初始化外设TIM4 OC4

	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR1上的预装载寄存器
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);  //使能TIM4在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM4, ENABLE);  //使能TIM4
}
