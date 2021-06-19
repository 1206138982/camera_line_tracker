#include "pid.h"

void PID_init(pid_struct *p_pid,float kp,float ki,float kd)
{
    printf("PID_init begin \r\n");
    p_pid->ActualSpeed = 0;
    p_pid->err = 0;
    p_pid->err_last = 0;
    p_pid->intergal = 0;
    p_pid->Kp = kp;
    p_pid->Ki = ki;
    p_pid->Kd = kd;
    printf("PID_init end\r\n");
}

float PID_realize(pid_struct *p_pid,float error)
{
    p_pid->err = error;
    p_pid->intergal += p_pid->err;
    p_pid->ActualSpeed = p_pid->Kp*p_pid->err + p_pid->Ki*p_pid->intergal + p_pid->Kd*(p_pid->err-p_pid->err_last);
    p_pid->err_last = p_pid->err;
    return p_pid->ActualSpeed;
}
