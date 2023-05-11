#include <stdio.h>
#include <wiringPi.h>
 
int main(){
	wiringPiSetup();  //初始化硬件环境
	//pinMode(引脚编号，INPUT/OUTPUT);
 	pinMode(7, OUTPUT);   
	//pinMode(9, OUTPUT);
	int value = 0;  //一周期内高电平的时间
	int i;
	while(1){  // 设置一个周期为1ms，占空比为60%
		for(i=0; i<200; i++){	
			//8,9引脚低电平时发光
			digitalWrite(7, LOW);
		//	digitalWrite(9, LOW);
			delayMicroseconds(value);  // 微秒级延迟，0.4ms
			//高电平时熄灭
			digitalWrite(7, HIGH);
		//	digitalWrite(9, HIGH);
			delayMicroseconds(1000-value);
		}
		value += 50;
		if(value>1000){
		    value = 0;
		}
	}
}