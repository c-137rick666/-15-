#ifndef ROBO_APP_H
#define ROBO_APP_H

#include <main.h>
#include <get_dis.h>

bool Detect_Border();
void Border_init();
void move_back();
void face_to(int dir);
void patrol();
void judge_tag(int result, int state);
void crash_forward();
void avoid_bomb();
void move_back_2();

extern int bomb_flag;
extern unsigned long bomb_startmillis;
#endif