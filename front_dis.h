#include <Arduino.h>
#include <main.h>
u8 get_front_message(void);
void SDM15_Decode(uint8_t RxData);
void SMD15_setScanfHZ(u8 hz);
void SMD15_setbaudrate(u8 i);
void SMD15_setpixhawk(void);
void SMD15_setstandard(void);
void start_scan(void);
void stop_scan(void);
void SMD15_init();
u8 countsum(u8 *buf);
void getserial_task();

extern u16 front_dis;