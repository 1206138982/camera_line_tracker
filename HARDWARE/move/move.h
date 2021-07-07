#ifndef __MOVE_H__
#define __MOVE_H__

#include "sys.h"
#include "AllHead.h"

void print2lcd(void);
// void move_around(void);
void turn2L(void);
void turn2R(void);
#if defined(MAP_TESTA) && MAP_TESTA
void move_for_fencha(u8 times);
#endif
void forward_ten(u16 ms_forward);
#if defined(MAP_TESTB) && MAP_TESTB	
void find_fencha_move(void);
#endif
void motation(void);
void printStopMess(int reason);

#endif