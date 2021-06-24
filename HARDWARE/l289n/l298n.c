#include "l298n.h"
#include "AllHead.h"

// u16	speed_min = 380;
int	speed_min = 400;
u16 max_add = 150;
extern int RUNNING;
u8 monitor_error_add = 50;

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
	Mr_P = 1;
	Mr_N = 0;
	Ml_P = 1;
	Ml_N = 0;
}

void Motor_Backward(void)
{
	Mr_P = 0;
	Mr_N = 1;
	Ml_P = 0;
	Ml_N = 1;
}

void Motor_Turnleft(void)
{
	Mr_P = 1;
	Mr_N = 0;
	Ml_P = 0;
	Ml_N = 0;
}

void Motor_Turnright(void)
{
	Mr_P = 0;
	Mr_N = 0;
	Ml_P = 1;
	Ml_N = 0;
}

void left_add(int add)
{
	if(add < 0){
		printf("error in left_add() add < 0\r\n");
		return ;
	}
	if(add > max_add){
		printf("in left_add() add:%d\tbeyond the max_add:%d\r\n",add,max_add);
		add = max_add;
	}
	TIM_SetCompare2(TIM3,speed_min-add);
}

void right_add(int add)
{
	if(add < 0){
		printf("error in right_add() add < 0\r\n");
		return ;
	}
	if(add > max_add){
		printf("in right_add() add:%d\tbeyond the max_add:%d\r\n",add,max_add);
		add = max_add;
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
	delay_ms(1500);
	delay_ms(1500);
	return ;
	Motor_Stop();
	delay_ms(100);

	speed_min = 410;
	right_add(0);
	left_add(0);
	delay_ms(1000);
	Motor_Stop();
	delay_ms(100);
}

void turnA(void)
{
	Motor_Forward();
	delay_ms(400);
	Motor_Turnright();
	delay_ms(800);
	Motor_Stop();
	// delay_ms(1500);
	// delay_ms(1500);
	// RUNNING = 0;
}

//https://blog.csdn.net/qq_36958104/article/details/83661117
void monitor_PWM_Init(u16 arr,u16 psc)
{
    GPIO_InitTypeDef     GPIO_InitStrue;
    TIM_OCInitTypeDef     TIM_OCInitStrue;
    TIM_TimeBaseInitTypeDef     TIM_TimeBaseInitStrue;
    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);        //使能TIM3和相关GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);// 使能GPIOB时钟(LED在BP5引脚),使能AFIO时钟(定时器3通道2需要重映射到BP5引脚)

#if defined(TIM3_PartialRemap) && TIM3_PartialRemap
    GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_5;     // TIM_CH2
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
    GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO端口初始化设置

    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_4;     // TIM3_CH1
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
    GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO端口初始化设置
	
// 如果PB0被初始化，则lcd不能正常显示，在LCD_Init()中有用到PB0，所以TIM3_CH2无法使用
    // GPIO_InitStrue.GPIO_Pin=GPIO_Pin_0;     // TIM3_CH2
    // GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
    // GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
    // GPIO_Init(GPIOB,&GPIO_InitStrue);                //GPIO端口初始化设置
#endif

#if defined(TIM3_FullRemap) && TIM3_FullRemap    
    GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_6;     // TIM3_CH1
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
    GPIO_Init(GPIOC,&GPIO_InitStrue);                //GPIO端口初始化设置
	
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_7;     // TIM3_CH2
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
    GPIO_Init(GPIOC,&GPIO_InitStrue);                //GPIO端口初始化设置
#endif

#if defined(TIM4_Remap) && TIM4_Remap    
    GPIO_PinRemapConfig(GPIO_Remap_TIM4,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD,ENABLE);

    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_12;     // TIM4_CH1
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
    GPIO_Init(GPIOD,&GPIO_InitStrue);                //GPIO端口初始化设置
	
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_13;     // TIM4_CH2
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_AF_PP;    // 复用推挽
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
    GPIO_Init(GPIOD,&GPIO_InitStrue);                //GPIO端口初始化设置
#endif

#if defined(DEBUG_PIN) && DEBUG_PIN
    GPIO_InitStrue.GPIO_Pin=GPIO_Pin_9;     // for test
    GPIO_InitStrue.GPIO_Mode=GPIO_Mode_Out_PP;    // 复用推挽
    GPIO_InitStrue.GPIO_Speed=GPIO_Speed_50MHz;    //设置最大输出速度
    GPIO_Init(GPIOC,&GPIO_InitStrue);                //GPIO端口初始化设置
	TEST_TIMER = 0;
#endif
    
    TIM_TimeBaseInitStrue.TIM_Period=arr;    //设置自动重装载值
    TIM_TimeBaseInitStrue.TIM_Prescaler=psc;        //预分频系数
    TIM_TimeBaseInitStrue.TIM_CounterMode=TIM_CounterMode_Up;    //计数器向上溢出
    TIM_TimeBaseInitStrue.TIM_ClockDivision=TIM_CKD_DIV1;        //时钟的分频因子，起到了一点点的延时作用，一般设为TIM_CKD_DIV1
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStrue);        //TIM3初始化设置(设置PWM的周期)
    
    TIM_OCInitStrue.TIM_OCMode=TIM_OCMode_PWM2;        // PWM模式2:CNT>CCR时输出有效
    TIM_OCInitStrue.TIM_OCPolarity=TIM_OCPolarity_High;// 设置极性-有效为高电平
    TIM_OCInitStrue.TIM_OutputState=TIM_OutputState_Enable;// 输出使能
	TIM_OC1Init(TIM3,&TIM_OCInitStrue);			// TIM3_CH1    PC6
    TIM_OC2Init(TIM3,&TIM_OCInitStrue);        // TIM3_CH2    PC7
	//TIM3的通道2PWM 模式设置	PB5	 CH2	the right monitor
	// TIM_OC3Init(TIM3,&TIM_OCInitStrue);        //TIM3的通道3PWM 模式设置   PB0  CH3 the left monitor
 
    TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);        //使能预装载寄存器
    TIM_OC2PreloadConfig(TIM3,TIM_OCPreload_Enable);        //使能预装载寄存器
	// TIM_OC3PreloadConfig(TIM3,TIM_OCPreload_Enable);        //使能预装载寄存器
    
    TIM_Cmd(TIM3,ENABLE);        //使能TIM3
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
