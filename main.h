#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>
#include <Wire.h>

#include <robo_app.h>
#include <motor.h>
#include <get_dis.h>
#include <vision.h>

#include <TimerOne.h>
#include <main_fsm.h>

#include <oled.h>

#define K1 33
#define K2 35
#define K3 37
#define K4 39
#define K5 41
#define K6 43
#define K7 45
#define K8 47

#define A0 54

#define BEEP 8
#define Openmv Serial1
#define TOF Serial2
#define FRONT_DIS Serial2
extern int _code;
void testTask();
void mainTask();
void timerTask();
void start_Task();
#endif