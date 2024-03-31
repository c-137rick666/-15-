#ifndef MAIN_FSM_H
#define MAIN_FSM_H
#include <main.h>
#include <get_dis.h>
//灰度
#define ANA_1 A1
#define ANA_2 A2
#define ANA_3 A3
#define ANA_4 A4
//漫反射
#define UP_REG A0

#define BORDER_1 K1 
#define BORDER_2 K2
#define BORDER_3 K3
#define BORDER_4 K4
//距离具体参数
#define FRONT_REG K5
#define BACK_REG K6
#define LEFT_REG K7
#define RIGHT_REG K8
//红外测距（模拟引脚读取数字信号）
#define FRONT_LEFT_1 A5
#define FRONT_RIGHT_1 A6

#define FRONT_LEFT_2 A7
#define FRONT_RIGHT_2 A8

void patrol_fsm();
#endif