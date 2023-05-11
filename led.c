#include<wiringPi.h>
#include<stdio.h>
#define LedPin 7
int main()
{
	if(wiringPiSetup() < 0)
		return 1;
	pinMode(LedPin, OUTPUT);
	for(int i=1;i<=1000;i++)
	{
		digitalWrite(LedPin,1);
		delay(2000);
		printf("working\n");
//		digitalWrite(LedPin,0);
//		delay(200);
	}
	return 0;
}
