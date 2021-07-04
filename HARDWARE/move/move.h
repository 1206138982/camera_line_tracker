#ifndef __MOVE_H__
#define __MOVE_H__

#include "sys.h"
#include "AllHead.h"

#if defined(BIKING) && BIKING
void print2lcd(void);
#endif
void move_around(void);
void turn2L(void);
void turn2R(void);
void move_for_fencha(u8 times);
void forward_ten(u16 ms_forward);
void find_fencha_move(void);

#endif