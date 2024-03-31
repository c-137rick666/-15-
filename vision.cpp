#include <vision.h>

int _code;

int get_code1(HardwareSerial &A)
{
    unsigned long startmillis = millis(); // store the current time
    unsigned long currentMillis;
    HardwareSerial *SerialX = &A;
    String comdata = "";
    while (true)
    {
        if (SerialX->available() > 0)
        {
            char getmsg = char(SerialX->read());
            comdata += getmsg;
            delay(2);
            // Serial.println(comdata);
            if (getmsg == '#')
            {
                while (SerialX->read() >= 0)
                {
                }
                // Serial.println("done");
                char cmd = comdata[0];
                /// Serial.println(cmd);
                if (cmd == '1')
                {
                    return 1;
                }
                else if (cmd == '2')
                {
                    return 2;
                }
            }
        }
        currentMillis = millis();
        if (currentMillis - startmillis >= 1000)
        {
            while (SerialX->read() >= 0)
            {
            }
            return 0;
        }
    }
}
void get_code2(HardwareSerial &A)
{
    String comdata = "";
    HardwareSerial *SerialX = &A;
    while (SerialX->available() > 0)
    {
        char getmsg = char(SerialX->read());
        comdata += getmsg;
        delay(2);
        // Serial.println(comdata);
        if (getmsg == '#')
        {
            while (SerialX->read() >= 0)
            {
            } // Çå³ý»º³åÇø
            // Serial.println("done");
            char cmd = comdata[0];
            /// Serial.println(cmd);
            if (cmd == '1')
            {
                _code = 1;
            }
            else if (cmd == '2')
            {
                _code = 2;
            }
            else
            {
                _code = 0;
            }
        }
    }
}
int get_result()
{
    int temp = _code;
     _code = 0;
    return temp;
}