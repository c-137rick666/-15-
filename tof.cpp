#include <tof.h>

TOF_CLASS tof;//一个成员函数，它负责从给定的硬件串口

bool TOF_CLASS::tof_fsm(HardwareSerial &A)//接收 TOF 数据包并解析有效数据。
{
    HardwareSerial *SerialX = &A;
    if (!SerialX->available())
        return false; // 函数首先检查是否有可用数据，如果没有则返回 false 表示未接收到有效数据。
    // uint8_t buf[16];
    // static uint8_t len = 0;
    uint8_t getHeader = 0;
    // Serial.println("get");
    do
    {
        delay(2);
        switch (getHeader)
        {
        case 0:
            if (SerialX->read() == 0x01)
            {
                getHeader = 1;
                //Serial.println("get header 1");
            }
            break;
        case 1:
            if (SerialX->read() == 0x03)
            {
                getHeader = 2;
                //Serial.println("get header 2");
            }
            else
                getHeader = 0;
            break;
        case 2:
            if (SerialX->read() == 0x02)
            {
                getHeader = 3;
                //Serial.println("get header 3");
            }
            else
                getHeader = 0;
            break;
        case 3:
            uint8_t buf1 = SerialX->read();
            delay(2);
            uint8_t buf2 = SerialX->read();
            int temp;
            if (buf1 == 255 && buf2 == 255)
            {
                temp = TOO_FAR;
            }
            else
            {
                temp = (buf1 << 8) | buf2;
            }
            if (temp >= TOO_FAR || temp <= 0)
            {
                temp = TOO_FAR;
            }
            this->_Tofget = temp;//它按照特定的数据头格式（0x01、0x03 和 0x02）接收数据，并解析最后两个字节得到距离值，存入类的成员变量 _Tofget。如果数据异常（比如超出范围），则设定 _Tofget 为预定义的 TOO_FAR 常量。
            // if (abs(temp - this->_last) < 400 || abs(temp - this->_lastlast) < 400)
            // {
            //     this->_Tofget = temp;
            //     this->_lastlast = this->_last;
            //     this->_last = temp;
            // }
            while (SerialX->read() >= 0)
            {
            }
            return true;//成功接收并解析数据后，返回 true。
            break;
        }
    } while (SerialX->available());
    return false;
}

void Lowpass_Filter(TOF_CLASS *tof) //此函数实现低通滤波器功能，对 _Tofget 中的距离值应用一阶低通滤波算法，计算结果存入 tof->lowFir。
{
    tof->lowFir = (1-tof->alpha)*tof->_last + tof->alpha*tof->_Tofget;
}

void Filter(TOF_CLASS *tof) //实现滑动平均滤波器，将最新的 _Tofget 值添加到滤波缓冲区，并移除最旧的数据。
{
  int i;
  int filter_sum = 0;
  tof->filter_buf[TOF_FILTER_N] = tof->_Tofget;//计算缓冲区内所有数据的平均值，并将结果存入 tof->Filter_avg。
  for(i = 0; i < TOF_FILTER_N; i++) {
    tof->filter_buf[i] = tof->filter_buf[i + 1];
    filter_sum += tof->filter_buf[i];
  }
  tof->Filter_avg = filter_sum / TOF_FILTER_N;
}

void get_tof(HardwareSerial &A, TOF_CLASS *tof)//该函数接收串口对象和 TOF 类实例指针作为参数。
{
    HardwareSerial *SerialX = &A;
    int re = tof->tof_fsm(*SerialX);
    if(re)
    {
        Filter(tof);//首先调用 tof_fsm 函数来获取 TOF 距离数据，如果成功获取数据，则调用 Filter 函数进行数据滤波。
    }
    Serial.print(tof->Filter_avg);
    Serial.print(",");
    Serial.println(tof->_Tofget);//最后，将经过滤波后的平均距离值（tof->Filter_avg）和原始的 _Tofget 值通过串口输出。
}