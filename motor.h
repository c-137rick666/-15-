#ifndef MOTOR_H
#define MOTOR_H

#include <main.h>

#define FORWARD 'w'
#define BACKWARD 's'
#define LEFT 'a'
#define RIGHT 'd'
#define STOP 'p'

void move_go(char dir, int pwm); // �������  ǰ0��1��2��3ͣ4   aֵС��240
void motor_init();

#define MOTOR_PIN1 4
#define MOTOR_PIN2 5
#define MOTOR_PIN3 6
#define MOTOR_PIN4 7



#endif