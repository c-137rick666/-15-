#ifndef GET_DIS_H
#define GET_DIS_H

#include <main.h>

#define FILTER_N 10

//类
//面向对象的封装
class SHARP//类名               
{
//属性（每一个类里的自己的属性  
private:
  float maxDistance = 150;  //cm
  float minDistance = 5;  //cm
  float offset = 0; //cm
  int frequency = 20;  //Hz
//属性（是这个类里面都有的属性
public:
  int distance = 150; //cm,
  int pin;
  int filter_buf[FILTER_N + 1];          
  float Filter_avg = 150.0;
//方法（其实就是函数
  SHARP(int Pin)//输入的引脚
  {
    this->pin = Pin;//赋值
    distance = 150;
    Filter_avg = 150.0;//滤波
    for (int i = 0; i < FILTER_N + 1; i++)
    {
      filter_buf[i] = 150;
    }
  };
};


extern SHARP DIS_F;
extern SHARP DIS_ABOVE;
extern SHARP DIS_L1;
extern SHARP DIS_L2;
extern SHARP DIS_L3;
extern SHARP DIS_R1;
extern SHARP DIS_R2;
extern SHARP DIS_R3;

#define threshlod 40.0

void distance_init();
int judege_distance1();
int judege_distance2();
void getDistance(SHARP *Sharp);
void info_output();
void Filter(SHARP *Sharp);

#endif 