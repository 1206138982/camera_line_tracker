#ifndef __L298N_H_
#define __L298N_H_

#include "sys.h"

#define MONITOR_ERROR   1
// #define BIKING  1
#define TIM3_PartialRemap   0
#define TIM3_FullRemap  0
#define TIM4_Remap  1

// #define M1_P PAout(4)
// #define M1_N PAout(5)
// #define M2_P PAout(6)
// #define M2_N PAout(7)

//the right monitor
#define Mr_P PGout(4)
#define Mr_N PGout(5)
//the left monitor
#define Ml_P PGout(6)
#define Ml_N PGout(7)

#define TEST_TIMER PCout(9)

void Motor_Init(void);
void Motor_PWM_Init(u16 arr,u16 psc);
void Motor_Stop(void);
void Motor_Forward(void);
void Motor_Backward(void);
void Motor_Turnleft(void);
void Motor_Turnright(void);
void monitor_PWM_Init(u16 arr,u16 psc);
void left_add(int add);
void right_add(int add);
void Motor_start(void);

#endif
