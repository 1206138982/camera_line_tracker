#include "move.h"

#define MAX_LENGTH2LCD	20
extern double overK;
extern int b;
extern int  lineDeviationLoc;
#if defined(BIKING) && BIKING
#if defined(SIMPLE_METHOD) && SIMPLE_METHOD
#else
extern pid_struct	line_pid;
extern pid_struct	oK_pid;
extern u8 RUNNING;
extern u8 FLAG_BORDER;
int speed_change = 0;
#endif
#endif

void print2lcd(void)
{
    u8 str_info2lcd[MAX_LENGTH2LCD] = {0};
    u8 temp_str[MAX_LENGTH2LCD] = {0};
	int i,k_x,k_y;

	k_x = 30;
	k_y = 160;
    for(i=0;i<MAX_LENGTH2LCD;i++)
        str_info2lcd[i] = 32;
    sprintf(temp_str,"lineDeviaLoc:%d",lineDeviationLoc);
    strncpy(str_info2lcd,temp_str,strlen(temp_str));
    LCD_SimpleString(k_x,k_y,str_info2lcd,MAX_LENGTH2LCD);

	k_x = 30;
	k_y = 190;
    for(i=0;i<MAX_LENGTH2LCD;i++)
        str_info2lcd[i] = 32;
    sprintf(temp_str,"oK:%.3f oB:%d",overK,b);
    strncpy(str_info2lcd,temp_str,strlen(temp_str));
	LCD_SimpleString(k_x,k_y,str_info2lcd,MAX_LENGTH2LCD);

#if defined(BIKING) && BIKING
#if defined(SIMPLE_METHOD) && SIMPLE_METHOD
#else
	k_x = 30;
	k_y = 220;
    for(i=0;i<MAX_LENGTH2LCD;i++)
        str_info2lcd[i] = 32;
    sprintf(temp_str,"speed change:%d",speed_change);
    strncpy(str_info2lcd,temp_str,strlen(temp_str));
	LCD_SimpleString(k_x,k_y,str_info2lcd,MAX_LENGTH2LCD);
#endif
#endif
}

/*
    0:the line is too left or right
    1:not get the useful line in getUsefulLine()
    2:not get the slope in regression()
    3:extern the left add in left_add()
    4:extern the right add in right_add()
*/
void printStopMess(u8 reason)
{
    u8 str_info2lcd[MAX_LENGTH2LCD] = {0};
    u8 temp_str[MAX_LENGTH2LCD] = {0};
	int i,k_x,k_y;

	k_x = 30;
	k_y = 250;
    for(i=0;i<MAX_LENGTH2LCD;i++)
        str_info2lcd[i] = 32;
    sprintf(temp_str,"stop bike,reason:%d",reason);
    strncpy(str_info2lcd,temp_str,strlen(temp_str));
	LCD_SimpleString(k_x,k_y,str_info2lcd,MAX_LENGTH2LCD);
}

#if defined(BIKING) && BIKING
void motation(void)
{
#if defined(SIMPLE_METHOD) && SIMPLE_METHOD
    u8 loc2turn,turn_step_delay,str_delay;
    loc2turn = 20;
    turn_step_delay = 60;
    str_delay = 140;
    if(lineDeviationLoc > loc2turn){
        Motor_Turnright();
        delay_ms(turn_step_delay);
        Motor_Stop();
    }
    else if(lineDeviationLoc < -loc2turn){
        Motor_Turnleft();
        delay_ms(turn_step_delay);
        Motor_Stop();
    }
    else{
        Motor_Forward();
        delay_ms(str_delay);
        Motor_Stop();
    }
#else
    int speed_line = 0;
    int speed_oK = 0;
    if(overK>1 && FLAG_BORDER==2){
        lineDeviationLoc = 60;
    }
    if(overK<-1 && FLAG_BORDER==1){
        lineDeviationLoc = -60;
    }
    // if(abs(lineDeviationLoc) > 50){
    //     RUNNING = 0;
    //     printStopMess(0);
    // }
    // else
    {
        speed_line = (int)PID_realize(&line_pid,lineDeviationLoc);
    }
    if(overK<0.06 && overK>-0.06){
        overK = 0;
    }
    else
    {
        speed_oK = (int)PID_realize(&oK_pid,overK);
    }
    speed_change = speed_line+speed_oK;
    printf("speed line:%d\tspeed ok:%d\t\tspeed change:%d\r\n",speed_line,speed_oK,speed_change);
    Motor_Forward();
    if(speed_change > 0){
        //turn right;
        left_add(speed_change);
        right_add(0);
    }
    else{
        //turn left
        left_add(0);
        right_add(-speed_change);
    }
#endif
}
#endif

// #if defined(BIKING) && BIKING
// 					Motor_Turnright();
// #if defined(SIMPLE_METHOD) && SIMPLE_METHOD
// 					delay_ms(step_delay);
// #else
// 					delay_ms(turn_delayA);
// 					// turn_flag = 1;
// #endif
// 					Motor_Stop();
// #endif
// 					// RUNNING =  0;
// #if defined(BIKING) && BIKING
// 					Motor_Turnright();
// #if defined(SIMPLE_METHOD) && SIMPLE_METHOD
// 					delay_ms(step_delay);
// #else
// 					delay_ms(turn_delayB);
// 					// turn_flag = 1;
// #endif
// 					Motor_Stop();
// #endif
// 					// RUNNING =  0;
// #if defined(BIKING) && BIKING
// 					Motor_Turnright();
// #if defined(SIMPLE_METHOD) && SIMPLE_METHOD
// 					delay_ms(step_delay);
// #else
// 					delay_ms(turn_delayC);
// 					turn_flag = 1;
// #endif
// 					Motor_Stop();
// #endif
// 					// RUNNING =  0;
// #if defined(BIKING) && BIKING
// 					Motor_Turnleft();
// #if defined(SIMPLE_METHOD) && SIMPLE_METHOD
// 					delay_ms(step_delay);
// #else
// 					delay_ms(turn_delayA);
// 					// turn_flag = 1;
// #endif
// 					Motor_Stop();
// #endif
// #if defined(BIKING) && BIKING
// 					Motor_Turnleft();
// #if defined(SIMPLE_METHOD) && SIMPLE_METHOD
// 					delay_ms(step_delay);
// #else
// 					delay_ms(turn_delayB);
// 					// turn_flag = 1;
// #endif
// 					Motor_Stop();
// #endif
// 					// RUNNING =  0;
// #if defined(BIKING) && BIKING
// 					Motor_Turnleft();
// #if defined(SIMPLE_METHOD) && SIMPLE_METHOD
// 					delay_ms(step_delay);
// #else
// 					delay_ms(turn_delayC);
// 					turn_flag = 1;
// #endif
// 					Motor_Stop();
// #endif
// 					// RUNNING =  0;
// #if defined(LCD_SHOW_INFO) && LCD_SHOW_INFO
// 			for(i=0;i<MAX_LENGTH2LCD;i++)
// 				str_info2lcd[i] = 32;
// 			strncpy(str_info2lcd,"TOO LEFT",strlen("TOO LEFT"));
// 			LCD_SimpleString(loc_x,loc_y,str_info2lcd,MAX_LENGTH2LCD);
// #endif
// #if defined(BIKING) && BIKING
// 			Motor_Turnleft();
// #if defined(SIMPLE_METHOD) && SIMPLE_METHOD
// 			delay_ms(step_delay);
// #else
// 			delay_ms(turn_delay);
// #endif
// 			Motor_Stop();
// 			// RUNNING = 0;
// #endif
// #if defined(LCD_SHOW_INFO) && LCD_SHOW_INFO
// 			for(i=0;i<MAX_LENGTH2LCD;i++)
// 				str_info2lcd[i] = 32;
// 			strncpy(str_info2lcd,"TOO RIGHT",strlen("TOO RIGHT"));
// 			LCD_SimpleString(loc_x,loc_y,str_info2lcd,MAX_LENGTH2LCD);
// #endif
// #if defined(BIKING) && BIKING
// 			Motor_Turnright();
// #if defined(SIMPLE_METHOD) && SIMPLE_METHOD
// 			delay_ms(step_delay);
// #else
// 			delay_ms(turn_delay);
// #endif
// 			Motor_Stop();
// 			// RUNNING = 0;
// #endif
// #if defined(BIKING) && BIKING
// 			// RUNNING = 0;
// #endif
// #if defined(LCD_SHOW_INFO) && LCD_SHOW_INFO
// #endif
// #if defined(DEBUG_PIN) && DEBUG_PIN
// 	static u8 test_flag = 1;
// #endif
// #if defined(DEBUG_PIN) && DEBUG_PIN
// 			TEST_TIMER = test_flag;
// 			if(test_flag)
// 				test_flag = 0;
// 			else
// 				test_flag = 1;
// #endif
// #if defined(BIKING) && BIKING
// #if defined(SIMPLE_METHOD) && SIMPLE_METHOD
// #else
// 	u8 turn_delay = 30;
// 	int speed_change = 0;
// #endif
// #endif
// #if defined(BIKING) && BIKING
// #if defined(SIMPLE_METHOD) && SIMPLE_METHOD
// #else
// #endif
// #endif