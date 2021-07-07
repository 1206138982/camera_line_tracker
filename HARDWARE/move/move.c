#include "move.h"

#define MAX_LENGTH2LCD	20
extern double overK;
extern int b;
extern int  lineDeviationLoc;
extern u8 leftBlackLoc[(NEEDHEIGHT)/(SKIPLINE)];
extern u8 rightBlackLoc[(NEEDHEIGHT)/(SKIPLINE)];
extern u8 devLocRes;

#if defined(BIKING) && BIKING
extern u8 RUNNING;
#if defined(PID_METHOD) && PID_METHOD
extern pid_struct	line_pid;
extern pid_struct	oK_pid;
extern u8 FLAG_BORDER;
int speed_change = 0;
#endif
#endif

#if defined(FENCHA_TEST) && FENCHA_TEST
extern u8 max_black_fencha;
#if defined(MAP_TESTA) && MAP_TESTA
u8 fencha_times = 0;
#endif
#if defined(MAP_TESTB) && MAP_TESTB
u8 fencha_number = 0;
#endif
#endif

void print2lcd(void)
{
#if defined(LCD_ON_OFF) && LCD_ON_OFF
    u8 str_info2lcd[MAX_LENGTH2LCD] = {0};
    u8 temp_str[MAX_LENGTH2LCD] = {0};
	int i,k_x,k_y;

	k_x = 30;
	k_y = 160;
    for(i=0;i<MAX_LENGTH2LCD;i++)
        str_info2lcd[i] = 32;
    switch (devLocRes)
    {
        case BOTHLOST:{
            strncpy(str_info2lcd,"Both lost",strlen("Both lost"));
			break;
		}
		case TOOLEFT:{
            strncpy(str_info2lcd,"TOO LEFT",strlen("TOO LEFT"));
			break;
		}
		case TOORIGHT:{
            strncpy(str_info2lcd,"TOO RIGHT",strlen("TOO RIGHT"));
			break;
		}
		case NOMIDLOC:{
            sprintf(temp_str,"nomid,Loc:%d",lineDeviationLoc);
            strncpy(str_info2lcd,temp_str,strlen(temp_str));
			break;
		}
		case NOLINEWIDTH:{
            strncpy(str_info2lcd,"NO LINE WIDTH",strlen("NO LINE WIDTH"));
			break;
		}
		case GETMIDLOC:{
            sprintf(temp_str,"lineDeviaLoc:%d",lineDeviationLoc);
            strncpy(str_info2lcd,temp_str,strlen(temp_str));
            break;
        default:
            break;
        }
    }
    LCD_SimpleString(k_x,k_y,str_info2lcd,MAX_LENGTH2LCD);

	k_x = 30;
	k_y = 190;
    for(i=0;i<MAX_LENGTH2LCD;i++)
        str_info2lcd[i] = 32;
    sprintf(temp_str,"oK:%.3f oB:%d",overK,b);
    strncpy(str_info2lcd,temp_str,strlen(temp_str));
	LCD_SimpleString(k_x,k_y,str_info2lcd,MAX_LENGTH2LCD);

#if defined(PID_METHOD) && PID_METHOD
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
    5:left fencha
    6:right fencha
    7:left fencha new
    8:right fencha new
    9:for test
*/
void printStopMess(int reason)
{
#if defined(LCD_ON_OFF) && LCD_ON_OFF
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
#endif
}

void motation(void)
{
#if defined(SIMPLE_METHOD) && SIMPLE_METHOD
#if defined(SIMPLE_METHODNEW) && SIMPLE_METHODNEW
    u8 str_delaynew = 90;
    int turn_delay = 0;
    turn_delay = (int)(30*overK) + (int)(1*lineDeviationLoc);
    printStopMess(turn_delay);
    if(turn_delay > 80){
        turn_delay = 80;
        // RUNNING = 0;
    }
    if(turn_delay < -80){
        turn_delay = -80;
        // RUNNING = 0;
    }
    if(turn_delay<32 && turn_delay>-32){
        Motor_Forward();
        delay_ms(str_delaynew);
        Motor_Stop();
    }
    else{
        if(turn_delay > 0){
            Motor_Turnright();
            delay_ms(turn_delay);
            Motor_Stop();
        }
        else{
            Motor_Turnleft();
            delay_ms(-turn_delay);
            // delay_ms(30);
            Motor_Stop();
        }
    }
#else
    u8 loc2turn,turn_step_delay,str_delay;
    float oK2turn;
    loc2turn = 18;  //default 20
    turn_step_delay = 35;   //default 60
    str_delay = 50;    //default  140
    oK2turn = 0.3;
    if(overK > oK2turn){
        Motor_Turnright();
        delay_ms(turn_step_delay);
        Motor_Stop();
    }
    else if(overK < -oK2turn){
        Motor_Turnleft();
        delay_ms(turn_step_delay);
        Motor_Stop();
    }
    else{
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
    }
#endif
#endif
#if defined(PID_METHOD) && PID_METHOD
    int speed_line = 0;
    int speed_oK = 0;
    // if(overK>1 && FLAG_BORDER==2){
    //     lineDeviationLoc = 60;
    // }
    // if(overK<-1 && FLAG_BORDER==1){
    //     lineDeviationLoc = -60;
    // }
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

#if defined(MAP_TESTA) && MAP_TESTA
void move_for_fencha(u8 times)
{
    switch(times){
        case 1:
            turn2L();
            break;
        case 2:
            turn2R();
            break;
        case 3:
            turn2L();
            break;
        case 4:
            turn2L();
            break;
        case 5:
            forward_ten(400);
            break;
        default:
            break;
    }
}
#endif

void turn2L(void)
{
    Motor_Forward();
    delay_ms(800);
    Motor_Leftback();
    delay_ms(750);
    forward_ten(200);
    Motor_Stop();
}

void turn2R(void)
{
    Motor_Forward();
    delay_ms(800);
    Motor_Rightback();
    delay_ms(750);
    forward_ten(200);
    Motor_Stop();
}

void forward_ten(u16 ms_forward)
{
    int i,n;
    n = ms_forward/1500;
    Motor_Forward();
    for(i=0;i<n;i++)
        delay_ms(1500);
    delay_ms(ms_forward%1500);
    Motor_Stop();
}

#if defined(MAP_TESTB) && MAP_TESTB	
void find_fencha_move(void)
{
    int i,flag_fencha;
    static u8 get_fencha = 0;
    flag_fencha = 0;
    for(i=0;i<(NEEDHEIGHT)/(SKIPLINE)-1;i++){
        if(abs(rightBlackLoc[i]-leftBlackLoc[i])>max_black_fencha){
            flag_fencha = 1;
            if(get_fencha == 0)
                fencha_number++;
            get_fencha = 1;
            break;
        }
    }
    if(flag_fencha == 0)
        get_fencha = 0;
    if(fencha_number == 2)
        RUNNING = 0;
}
#endif

// void move_around(void)
// {
//     int i;
//     for(i=(NEEDHEIGHT)/(SKIPLINE)-1;i>(NEEDHEIGHT)/(SKIPLINE)-5;i--){
//         if(((rightBlackLoc[i]-leftBlackLoc[i])-(rightBlackLoc[i-1]-leftBlackLoc[i-1])) > 20){
//             if(abs(rightBlackLoc[i]-rightBlackLoc[i+1]) > 15){
//                 // turn2R();
//                 printStopMess(8);
//             }
//             else if(abs(leftBlackLoc[i]-leftBlackLoc[i+1]) > 15){
//                 // turn2L();
//                 printStopMess(7);
//             }
//             RUNNING = 0;
//             break;
//         }
//     }
// }

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
