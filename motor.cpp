#include <motor.h>

void motor_init()
{
    pinMode(MOTOR_PIN1,OUTPUT);
    pinMode(MOTOR_PIN2,OUTPUT);
    pinMode(MOTOR_PIN3,OUTPUT);
    pinMode(MOTOR_PIN4,OUTPUT);
}

void move_go(char dir, int pwm) // 电机驱动  前0后1左2右3停4   a值小于240
{
    if (pwm > 240)
    {
        Serial.println("pwm can not over 240");
        return;
    }
    switch (dir)
    {
    case BACKWARD:
        digitalWrite(MOTOR_PIN1, 0);
        analogWrite(MOTOR_PIN2, pwm);
        digitalWrite(MOTOR_PIN3, 0);
        analogWrite(MOTOR_PIN4, pwm);
        break;

    case FORWARD:
        analogWrite(MOTOR_PIN1, pwm);
        digitalWrite(MOTOR_PIN2, 0);
        analogWrite(MOTOR_PIN3, pwm);
        digitalWrite(MOTOR_PIN4, 0);
        break;

    case RIGHT:
        digitalWrite(MOTOR_PIN1, 0);
        analogWrite(MOTOR_PIN2, pwm);
        analogWrite(MOTOR_PIN3, pwm);
        digitalWrite(MOTOR_PIN4, 0);
        break;

    case LEFT:
        analogWrite(MOTOR_PIN1, pwm);
        digitalWrite(MOTOR_PIN2, 0);
        digitalWrite(MOTOR_PIN3, 0);
        analogWrite(MOTOR_PIN4, pwm);
        break;

    case STOP:
        digitalWrite(MOTOR_PIN1, 0);
        digitalWrite(MOTOR_PIN2, 0);
        digitalWrite(MOTOR_PIN3, 0);
        digitalWrite(MOTOR_PIN4, 0);
        break;
    }
}