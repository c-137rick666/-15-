/*
 * OLED��ʾ
 */
#include <main.h>

void setup()
{
  Serial.begin(9600);
  Openmv.begin(9600);
  motor_init();
  Border_init();
  distance_init();
  oled_init();
  Timer1.initialize(5000);           // ��ʼ�� Timer1 ����ʱ��ÿ��� 0.02sִ���жϺ���һ��
  Timer1.attachInterrupt(timerTask); // �趨 callback Ϊ Timer ���жϺ���
}

void loop()
{
  // testTask();
  start_Task();
  patrol_fsm();
}

void timerTask()
{
  get_code2(Openmv);
  // get_front_message();
  // Serial.println(front_dis);
}

void testTask()

{
  while (true)
  {
    Detect_Border();
  }
}

void start_Task()
{
  while (true)
  {
    if (!digitalRead(LEFT_REG) && !digitalRead(RIGHT_REG))
    {
      move_go(BACKWARD, 220);
      delay(1000);
      move_go(LEFT, 150);
      delay(300);
      move_go(STOP, 0);
      break;
    }
  }
}