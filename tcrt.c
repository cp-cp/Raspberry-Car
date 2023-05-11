#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#define KEY      21   //wiringPi模式第7号脚
int main()
{
    if(wiringPiSetup()==-1)
    {
      perror("树莓派启动失败");
      exit(-1);
    }
    pinMode(KEY,INPUT);//设置引脚为输入
    while(1)
    {
        if(digitalRead(KEY)==0)
        {
        //遮挡低电平，未遮挡高电平
             puts("被遮挡...");
        }
	}
   return 0;
 }

