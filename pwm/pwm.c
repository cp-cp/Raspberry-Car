#include <wiringPi.h>
#define pwm_pin 1    //定义PWM引脚 GPIO.1
int main (void)
{
    wiringPiSetup ();               // wiringPi库初始化
    pinMode (pwm_pin, PWM_OUTPUT);   // 设置GPIO.1为PWM输出    
    int val = 0;
    while(1)
    {
      val ++;
      if(val >= 1024){
        val = 0;
      }
      pwmWrite(pwm_pin,val); // 输出占空比为val/1024的pwm波
    //  digitalWrite(pwm_pin,HIGH);
      delay(10); // 延时3毫秒
    }
  return 0 ;
}