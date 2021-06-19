#ifndef __PID_H
#define __PID_H	 
#include "sys.h"

typedef struct{
    float ActualSpeed;
    float err;
    float err_last;
    float Kp,Ki,Kd;
    float intergal;
}pid_struct;

void PID_init(pid_struct *p_pid_struct,float kp,float ki,float kd);
float PID_realize(pid_struct *p_pid_struct,float error);
		 				    
#endif
