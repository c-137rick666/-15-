#include <robo_app.h>

int bomb_flag = 0;

void Border_init()//初始化BEEP引脚为输出模式。
{
    pinMode(BEEP, OUTPUT);
}
bool Detect_Border() // 检测四个边界传感器（BORDER_1到BORDER_4）是否检测到障碍物。若检测到障碍，则设置蜂鸣声（通过PWM实现），并返回true；否则关闭蜂鸣声并返回false。
{
    if (digitalRead(BORDER_1) == 1 || digitalRead(BORDER_2) == 1 || digitalRead(BORDER_3) == 1 || digitalRead(BORDER_4) == 1) 
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

void 
move_back()//后退  远离障碍物。
{
    Serial.println("move_back");
    int state1 = digitalRead(BORDER_1);
    int state2 = digitalRead(BORDER_2);
    int state3 = digitalRead(BORDER_3);
    int state4 = digitalRead(BORDER_4);
    if (state1 == 1 || state2 == 1)
    {
        move_go(BACKWARD, 20);
        delay(50);
        move_go(BACKWARD, 100);
        delay(400);
        if (state1 == 1 && state2 == 0)//转一点
        {
            move_go(LEFT, 150);
            delay(300);
            return;
        }
        else if (state1 == 0 && state2 == 1)//转一点
        {
            move_go(LEFT, 150);
            delay(300);
            return;
        }
        else if (state1 == 1 && state2 == 1)//转很多
        {
            move_go(LEFT, 150);
            delay(600);
            return;
        }
    }
    if (state3 == 1 || state4 == 1)
    {
        move_go(BACKWARD, 20);
        delay(50);
        move_go(BACKWARD, 100);
        delay(400);
        if (state3 == 1 && state4 == 0)
        {
            move_go(RIGHT, 150);
            delay(300);
            return;
        }
        else if (state3 == 0 && state4 == 1)
        {
            move_go(RIGHT, 150);
            delay(300);
            return;
        }
        else if (state3 == 1 && state4 == 1)
        {
            move_go(RIGHT, 150);
            delay(600);
            return;
        }
    }
}

void move_back_2()
{
    //Serial.println("move_back_2");
    if (digitalRead(BORDER_1) == 1 || digitalRead(BORDER_2) == 1)
    {
        move_go(BACKWARD, 0);
        delay(50);
    }
    if (digitalRead(BORDER_3) == 1 || digitalRead(BORDER_4) == 1)
    {
        move_go(BACKWARD, 0);
        delay(50);
    }
}

unsigned long bomb_startmillis;

void patrol()//机器人的主巡逻循环。它检查bomb_flag状态并相应地执行不同操作。如果尚未检测到炸弹（bomb_flag == 0），则调用judege_distance2()和get_result()分析传感器数据，然后根据结果使用judge_tag()采取行动。
{
    Serial.println("patroling");
    if (bomb_flag == 0)
    {
        int state = judege_distance2();
        int result = get_result();
        judge_tag(result, state);
    }
    else if (bomb_flag == 1)
    {
        bomb_startmillis = millis(); // store the current time
        bomb_flag = 2;
    }
    else if (bomb_flag == 2)
    {
        judge_tag(0, 0);
        unsigned long current_millis = millis();
        if (current_millis - bomb_startmillis > 2000)
        {
            bomb_flag = 0;
        }
    }
}

void judge_tag(int result, int state)//此函数分析检测结果并决定采取何种行动——当找到目标或能量时向前行进（crash_forward），当发现炸弹时躲避炸弹（avoid_bomb），或者继续导航。
{
    if (result == 1)//�Ӿ�ֱ�Ӽ��������
    {
        Serial.println("find energy");
        crash_forward();
    }
    else if (result == 2)//�Ӿ�ֱ�Ӽ��ը��
    {
        Serial.println("find bomb");
        avoid_bomb();
    }
    else if (result == 0 && state == 4)//�Ӿ�û��⵽��ǰ����⵽����
    {
        Serial.println("find target");
        crash_forward();
    }
    else if (state == 1 || state == 2 || state == 3)
    {
        face_to(state);
        // int result = get_code1(Openmv);
        int temp_result = get_result();
        if (temp_result == 1 || temp_result == 0) // Ŀ��
        {
            Serial.println("find target");
            crash_forward();
        }
        else if (temp_result == 2) // ը��
        {
            Serial.println("find bomb");
            avoid_bomb();
        }
    }
    else if (state == 0 && result == 0)
    {
        move_go(FORWARD, 50);
    }
}
void crash_forward()//让机器人持续前进，直到遇到障碍物或检测到炸弹为止。
{
    while (true) // ײ��ͷ
    {
        move_go(FORWARD, 50);
        if (Detect_Border()) // ���߽��˳�
        {
            break;
        }
        if (get_result() == 2) // ����ը���˳�
        {
            break;
        }
    }
}
void avoid_bomb()//当检测到炸弹时，此函数使机器人向左转，并将bomb_flag设置为1。
{
    // ת��
    move_go(LEFT, 80);
    delay(500);
    bomb_flag = 1;
}
void face_to(int dir)//使机器人朝指定方向（0、1、2或3）转动。它结合转向和利用K5传感器检测障碍物来停止旋转，
{
    unsigned long startmillis = millis(); // store the current time
    unsigned long currentMillis;
    switch (dir) // 0ǰ1��2��3��
    {
    case 0:
        move_go(STOP, 50);
        break;
    case 1:
        while (true)
        {
            if (Detect_Border()) // ���߽�
            {
                move_back();
            }
            else
            {
                Serial.println("TURN BACK");
                int front = digitalRead(K5);
                // getDistance(&DIS_F);
                // getDistance(&DIS_ABOVE);
                // get_tof(TOF,&tof);
                // ת180��
                move_go(LEFT, 30);
                // if (DIS_F.Filter_avg < threshlod || DIS_ABOVE.Filter_avg < threshlod )
                if(front == 0)
                {
                    move_go(STOP, 50);
                    break;
                }
                currentMillis = millis();
                if (currentMillis - startmillis >= 2000)
                {
                    move_go(STOP, 50);
                    break;
                }
            }
        }
        break;
    case 2:
        while (true)
        {
            if (Detect_Border()) // ���߽�
            {
                move_back();
            }
            else
            {
                Serial.println("TURN LEFT");
                int front = digitalRead(K5);
                // getDistance(&DIS_F);
                // getDistance(&DIS_ABOVE);
                // get_tof(TOF,&tof);
                // ת90��
                move_go(LEFT, 30);
                currentMillis = millis();
                // if (DIS_F.Filter_avg < threshlod || DIS_ABOVE.Filter_avg < threshlod )//|| tof.Filter_avg < TOF_THRESHOLD)
                if(front == 0)
                {
                    move_go(STOP, 50);
                    break;
                }
                if (currentMillis - startmillis >= 2000)
                {
                    move_go(STOP, 50);
                    break;
                }
            }
        }
        break;
    case 3:
        while (true)
        {
            if (Detect_Border()) // ���߽�
            {
                move_back();
            }
            else
            {
                Serial.println("TURN RIGHT");
                int front = digitalRead(K5);
                // getDistance(&DIS_F);
                // getDistance(&DIS_ABOVE);
                // get_tof(TOF,&tof);
                // ת90��
                move_go(RIGHT, 30);
                currentMillis = millis();
                //if (DIS_F.Filter_avg < threshlod || DIS_ABOVE.Filter_avg < threshlod )//|| tof.Filter_avg < TOF_THRESHOLD)
                if(front == 0)
                {
                    move_go(STOP, 50);
                    break;
                }
                if (currentMillis - startmillis >= 2000)
                {
                    move_go(STOP, 50);
                    break;
                }
            }
        }
        break;
    }
}