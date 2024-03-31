#include <oled.h>
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void oled_init()
{

  display.begin(SSD1306_SWITCHCAPVCC,SCREEN_ADDRESS);
  display.setTextColor(WHITE);//设置文本颜色为白色，即显示内容的颜色。
  display.clearDisplay();//清除显示屏上的所有内容，使其全黑。

  display.setTextSize(2);//设置字体大小为 2，即放大文本。
  display.setCursor(15, 30);//设置光标位置，即将在此坐标（x=15，y=30）开始绘制文本。
  display.println("OLED TEST");//在当前光标位置输出字符串 "OLED TEST"。
  
  display.display(); // 更新显示屏，将上述设置和写入的内容实际显示出来。
}