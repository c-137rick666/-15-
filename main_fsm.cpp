#include <main_fsm.h>

void patrol_fsm()//根据不同的状态（state），机器人执行相应的动作，同时读取并处理传感器数据。
{ 
    int state = 0;             //表示机器人的当前状态。
    int judge_reg;             // 中间变量，用于存储判断结果或传感器返回的数据。
    int result_cv;             // 
    unsigned long startmillis; // store the current time
    unsigned long currentMillis;
    int break_flag = 0;        // 作为标志位，在特定情况下跳出循环。  

    #define inner_speed  120   //定义的常量有四种速度值，包括内圈速度（inner_speed）、外圈速度（outer_speed）、转向速度（turn_speed）和攻击速度（attack_speed）。
    #define outer_speed  80
    #define turn_speed  120
    #define attack_speed  210

    while (true)
    { 
        display.clearDisplay();

        display.setTextSize(5);
        display.setCursor(2,4);

        display.print(state);

        //读取红外测距
        int ana_1 = analogRead(ANA_1);
        int ana_2 = analogRead(ANA_2);
        int ana_3 = analogRead(ANA_3);
        int ana_4 = analogRead(ANA_4);

        //串口
        display.setTextSize(1);

        display.setCursor(90, 2);
        display.println(ana_1);
        display.setCursor(90, 14);
        display.println(ana_2);
        display.setCursor(90, 26);
        display.println(ana_3);
        display.setCursor(90, 38);
        display.println(ana_4);

        display.display();
        switch (state)
        {
        case 0: //状态0时，根据传感器平均读数决定行驶速度，并检测边界、目标物（能量、炸弹、目标等），根据检测结果切换到对应状态 
            if (bomb_flag == 0)//没有炸弹
            {
                //台上位置判断
                if (( ana_2 + ana_3 + ana_4) / 3 <= 700)//内圈
                {
                    move_go(FORWARD, inner_speed);
                }
                //外圈
                else
                {
                    move_go(FORWARD, outer_speed);
                }
                //台下判断
                if (( ana_2 + ana_3 + ana_4) / 3 >= 900)//台下
                {
                    state = 5;
                    break;
                }
                //边缘检测
                if (Detect_Border())//重复边缘检测
                {
                    state = 1;
                    break;
                }
                //判断边缘和能量块
                else
                {
                    judge_reg = judege_distance2();//判断方向
                    result_cv = get_result();//视觉判断物体
                    if (result_cv == 1)//中立能量块
                    {
                        Serial.println("find energy");
                        state = 2;
                        break;
                    }
                    else if (result_cv == 2)//己方能量块
                    {
                        Serial.println("find bomb");
                        state = 3;
                        break;
                    }
                    else if (result_cv == 0 && judge_reg == 4)//对方能量块或车
                    {
                        Serial.println("find target");
                        state = 2;
                        break;
                    }
                    else if (judge_reg == 1 || judge_reg == 2 || judge_reg == 3)
                    {
                        state = 4;
                        break;
                    }
                    else if (judge_reg == 0 && result_cv == 0)//回归正常行驶
                    {
                        state = 0;
                        break;
                    }
                }
            }
            else if (bomb_flag == 2)//有炸弹
            {
                move_go(FORWARD, 100);
                //检测有无障碍物
                if (Detect_Border())
                {
                    state = 1;
                    break;
                }
                unsigned long current_millis = millis();
                //距离遭遇爆炸物时间过长，再次正常行驶
                if (current_millis - bomb_startmillis > 2000)
                {
                    bomb_flag = 0;
                    state = 0;
                    break;
                }
            }
            break;
        case 1:  //状态1时，根据边界传感器决定行驶速度，并检测边界、目标物（能量、炸弹、目标等）
            move_back();
            state = 0;
            break;
        case 2:  //状态2时，根据传感器平均读数决定行驶速度，并检测边界、目标物（能量、炸弹、目标等），根据检测结果切换到对应状态
            display.setTextSize(5);
            display.setCursor(32, 4);
            if (Detect_Border())//初步检测有障碍物（无操作
            {
                move_back();//先躲避障碍物→再次扫描
                state = 2;
                break;   
            }
            if (get_result() == 2)//视觉检测遇到炸弹
            {
                state = 3;
                break;
            }
            //遇到可攻击目标
            if (digitalRead(FRONT_REG) == 0)
            {
                display.print("F");
                move_go(FORWARD, attack_speed);
            }
            else if (digitalRead(FRONT_RIGHT_1) == 0 || digitalRead(FRONT_RIGHT_2) == 0)
            {
                
                display.print("R");
                move_go(RIGHT, turn_speed);
            }
            else if (digitalRead(FRONT_LEFT_1) == 0 || digitalRead(FRONT_LEFT_2) == 0)
            {
                display.print("L");
                move_go(LEFT, turn_speed);
            }
            else
            {
                state = 0;
                break;
            }
            display.display();
            break;
        case 3:// 遇到炸弹 标记并稍作延迟后继续搜索。            
            bomb_startmillis = millis(); // store the current time
            bomb_flag = 2;
            move_go(LEFT, turn_speed);
            delay(500);
            if (get_result() != 2)
            {
                state = 0;
                break;
            }
            break;
        case 4: // 转向 若达到设定时间或条件满足则回到状态0。
            startmillis = millis(); // store the current time
            break_flag = 0;
            while (true)
            {
                display.setTextSize(5);
                switch (judge_reg) //根据判断结果进行转向
                {
                    case 0://向前
                        state = 0;
                        break_flag = 1;
                        display.setCursor(32, 4);
                        display.print("F");
                        break;
                    case 1://左转掉头看看后面
                        Serial.println("TURN BACK");
                        display.setCursor(32, 4);
                        display.print("LB");
                        move_go(LEFT, turn_speed);
                        if (digitalRead(FRONT_REG) == 0)
                        {
                            state = 0;
                            break_flag = 1;
                            break;
                        }
                        currentMillis = millis();
                        if (currentMillis - startmillis >= 2000)
                        {
                            state = 0;
                            break_flag = 1;
                            break;
                        }
                        break;
                    case 2://左面检测到就转向左
                        Serial.println("TURN LEFT");
                        display.setCursor(32, 4);
                        display.print("LL");
                        move_go(LEFT, turn_speed);
                        currentMillis = millis();
                        if (digitalRead(FRONT_REG) == 0)
                        {
                            state = 0;
                            break_flag = 1;
                            break;
                        }
                        if (currentMillis - startmillis >= 2000)
                        {
                            state = 0;
                            break_flag = 1;
                            break;
                        }
                        break;
                    case 3://右面检测到就转向右再检查
                        Serial.println("TURN RIGHT");
                        display.setCursor(32, 4);
                        display.print("RR");
                        move_go(RIGHT, turn_speed);
                        currentMillis = millis();
                        if (digitalRead(FRONT_REG) == 0)
                        {
                            state = 0;
                            break_flag = 1;
                            break;
                        }
                        if (currentMillis - startmillis >= 2000)
                        {
                            state = 0;
                            break_flag = 1;
                            break;
                        }
                        break;
                }
                if (break_flag == 1)
                {
                    display.setCursor(32, 4);
                    display.print("F");
                    break;
                }
                display.display();
            }
            break;
        case 5: //掉下台子了  尝试复原
            analogWrite(BEEP, 240);
            move_go(RIGHT, 70);
            delay(300);
            analogWrite(BEEP, 180);
            move_go(FORWARD, 70);
            delay(300);
            if (digitalRead(UP_REG) == 0 && digitalRead(FRONT_REG) == 0 && digitalRead(BACK_REG) == 0)
            {
                move_go(BACKWARD, 200);
                delay(1300);
                move_go(LEFT, 150);
                delay(300);
                analogWrite(BEEP, 0);
                state = 0;
                break;
            }
            break;
        }
    }
}

