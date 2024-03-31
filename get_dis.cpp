//距
//离
//获
//取




#include <get_dis.h>

float distance[] = {20, 30, 40, 50, 60, 70, 80, 90, 100, 110, 120, 130, 140, 150};
float voltage[14] = {2.5, 2, 1.55, 1.25, 1.1, 0.85, 0.8, 0.73, 0.7, 0.65, 0.6, 0.5, 0.45, 0.4};

SHARP DIS_F(A5);

SHARP DIS_L1(A0);
SHARP DIS_L2(A1);//
SHARP DIS_L3(A2);

SHARP DIS_R1(A9);
SHARP DIS_R2(A10);
SHARP DIS_R3(A11);

SHARP DIS_B(A4);
SHARP DIS_ABOVE(A3);

void distance_init()
{
    pinMode(A0, INPUT);

    pinMode(A1, INPUT);
    pinMode(A2, INPUT);
    pinMode(A3, INPUT);

    pinMode(A4, INPUT);
    pinMode(A5, INPUT);
    pinMode(A6, INPUT);

    pinMode(A7, INPUT);

    pinMode(A8, INPUT);
}
//夏普的什么玩意模拟量，测距的吧？
void getDistance(SHARP *Sharp)
{
    float v = analogRead(Sharp->pin);
    v = v / 1024.0 * 5;
    int index = 0;
    for (index = 0; index < 14; index++)
    {
        if (v >= voltage[index])
        {
            break;
        }
    }
    if (index == 0)
    {
        Sharp->distance = 20;
    }
    else if (index == 14)
    {
        Sharp->distance = 150;
    }
    else
    {
        Sharp->distance = map(v, voltage[index], voltage[index - 1], distance[index], distance[index - 1]);
    }
    Filter(Sharp);
}

//数据过滤
int judege_distance1()
{
    getDistance(&DIS_F);

    getDistance(&DIS_L1);
    getDistance(&DIS_L2);
    getDistance(&DIS_L3);

    getDistance(&DIS_R1);
    getDistance(&DIS_R2);
    getDistance(&DIS_R3);

    getDistance(&DIS_B);

    getDistance(&DIS_ABOVE);

    if (DIS_B.Filter_avg < threshlod)
    {
        return 1;
    }
    
    else if (DIS_L1.Filter_avg < threshlod || DIS_L2.Filter_avg < threshlod || DIS_L3.Filter_avg < threshlod)
    {
        return 2;
    }

    else if (DIS_R1.Filter_avg < threshlod || DIS_R2.Filter_avg < threshlod || DIS_R3.Filter_avg < threshlod)
    {
        return 3;
    }

    else if (DIS_F.Filter_avg < threshlod)
    {
        return 4;
    }
    
    else
    {
        return 0;
    }
}
//判断障碍物方向
int judege_distance2()
{
    
    int front = digitalRead(FRONT_REG);
    int back = digitalRead(BACK_REG);
    int left = digitalRead(LEFT_REG);
    int right = digitalRead(RIGHT_REG);

    int front_right_1 = digitalRead(FRONT_RIGHT_1);
    int front_left_1 = digitalRead(FRONT_LEFT_1);

    int front_right_2 = digitalRead(FRONT_RIGHT_2);
    int front_left_2 = digitalRead(FRONT_LEFT_2);

    if (front == 0)//ǰ
    {
        analogWrite(BEEP, 100);
        return 4;
    }
    
    else if (left == 0 || front_left_1 == 0 || front_left_2 == 0)//��
    {
        analogWrite(BEEP, 100);
        // display.setTextSize(1);
        // display.setCursor(80, 50);
        // display.print(left);
        // display.print(front_left_1);
        // display.print(front_left_2);
        return 2;
    }

    else if (right == 0 || front_right_1 == 0 || front_right_2 == 0)
    {
        analogWrite(BEEP, 100);
        display.setTextSize(1);
        display.setCursor(90, 50);
        display.print(right);
        display.print(front_right_1);
        display.print(front_right_2);
        display.display();
        return 3;
    }

    else if (back == 0)//��
    {
        analogWrite(BEEP, 100);
        return 1;
    }
    
    else
    {
        analogWrite(BEEP, 0);
        return 0;
    }
}

void info_output()
{
  Serial.print(DIS_F.Filter_avg);
  Serial.print(",");
  Serial.print(DIS_L1.Filter_avg);
  Serial.print(",");
  Serial.print(DIS_L2.Filter_avg);
  Serial.print(",");
  Serial.print(DIS_L3.Filter_avg);
  Serial.print(",");
  Serial.print(DIS_R1.Filter_avg);
  Serial.print(",");
  Serial.print(DIS_R2.Filter_avg);
  Serial.print(",");
  Serial.print(DIS_R3.Filter_avg);
  Serial.print(",");
  Serial.print(DIS_B.Filter_avg);
  Serial.print(",");
  Serial.print(DIS_ABOVE.Filter_avg);
  Serial.println();
}

//滤波处理
void Filter(SHARP *Sharp) 
{
  int i;
  int filter_sum = 0;
  Sharp->filter_buf[FILTER_N] = Sharp->distance;
  for(i = 0; i < FILTER_N; i++) {
    Sharp->filter_buf[i] = Sharp->filter_buf[i + 1]; // ???????????
    filter_sum += Sharp->filter_buf[i];
  }
  Sharp->Filter_avg = (float)filter_sum / (float)FILTER_N;
}