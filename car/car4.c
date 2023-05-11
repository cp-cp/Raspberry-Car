#include <wiringPi.h>
#include <softPwm.h>
#include<stdio.h>
#include<stdio.h>
#include<wiringPi.h>
#include<softPwm.h>
#include <stdio.h>  
#include <stdbool.h>
#include <sys/time.h>  


//定义引脚
int Left_motor_go = 29;       //左电机前进AIN2连接Raspberry的wiringPi编码28口
int Left_motor_back = 3;     //左电机后退AIN1连接Raspberry的wiringPi编码29口

int Right_motor_go = 4;      //右电机前进BIN2连接Raspberry的wiringPi编码24口
int Right_motor_back = 5;    //右电机后退BIN1连接Raspberry的wiringPi编码25口

int Left_motor_pwm1 = 21;      //左电机控速PWMA连接Raspberry的wiringPi编码27口
int Left_motor_pwm2 = 22;      //左电机控速PWMA连接Raspberry的wiringPi编码27口
int Right_motor_pwm1 = 23;     //右电机控速PWMB连接Raspberry的wiringPi编码23口
int Right_motor_pwm2 = 24;     //右电机控速PWMB连接Raspberry的wiringPi编码23口

const int TrackSensorLeftPin1  =  1;   //定义左边第一个循迹红外传感器引脚为wiringPi编码9口
const int TrackSensorLeftPin2  =  27;  //定义左边第二个循迹红外传感器引脚为wiringPi编码21口
const int TrackSensorRightPin1 =  0;   //定义右边第一个循迹红外传感器引脚为wiringPi编码7口
const int TrackSensorRightPin2 =  25;   //定义右边第二个循迹红外传感器引脚为wiringPi编码1口

//定义各个循迹红外引脚采集的数据的变量
int TrackSensorLeftValue1;
int TrackSensorLeftValue2;
int TrackSensorRightValue1;
int TrackSensorRightValue2;
#define Trig    2 
#define Echo    28 

void ultraInit(void)  
{  
    pinMode(Echo, INPUT);  
    pinMode(Trig, OUTPUT);  
}  
float disMeasure(void)  
{  
    struct timeval tv1;  
    struct timeval tv2;  
    long start, stop;  
    float dis;  
  
    digitalWrite(Trig, LOW);  
    delayMicroseconds(2);  
  
    digitalWrite(Trig, HIGH);  
    delayMicroseconds(10);      //发出超声波脉冲  
    digitalWrite(Trig, LOW);  
      
    while(!(digitalRead(Echo) == 1));  
    gettimeofday(&tv1, NULL);           //获取当前时间  
  
    while(!(digitalRead(Echo) == 0));  
    gettimeofday(&tv2, NULL);           //获取当前时间  
  
    start = tv1.tv_sec * 1000000 + tv1.tv_usec;   //微秒级的时间  
    stop  = tv2.tv_sec * 1000000 + tv2.tv_usec;  
  
    dis = (float)(stop - start) / 1000000 * 34000 / 2;  //求出距离  
  
    return dis;  
}  
void setOut()
{
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  printf("MOTOR END UP!\n");
}
/**
* Function       run
* @author        Danny
* @date          2017.08.16
* @brief         小车前进
* @param[in1]    left_speed:左轮速度
* @param[in2]    right_speed:右轮速度
* @param[out]    void
* @retval        void
* @par History   无
*/
void run(int left_speed, int right_speed)
{
  //左电机前进
  digitalWrite(Left_motor_go, HIGH);   //左电机前进使能
  digitalWrite(Left_motor_back, LOW);  //左电机后退禁止
  softPwmWrite(Left_motor_pwm1, left_speed );
  softPwmWrite(Left_motor_pwm2, left_speed );

  //右电机前进
  digitalWrite(Right_motor_go, HIGH);  //右电机前进使能
  digitalWrite(Right_motor_back, LOW); //右电机后退禁止
  softPwmWrite(Right_motor_pwm1, right_speed);
  softPwmWrite(Right_motor_pwm2, right_speed);
}
void brake(int time)
{
  digitalWrite(Left_motor_go, LOW);
  digitalWrite(Left_motor_back, LOW);
  digitalWrite(Right_motor_go, LOW);
  digitalWrite(Right_motor_back, LOW);

  delay(time * 100);
}
void left(int left_speed, int right_speed)
{
  //左电机停止
  digitalWrite(Left_motor_go, LOW);    //左电机前进禁止
  digitalWrite(Left_motor_back, LOW);  //左电机后退禁止
  softPwmWrite(Left_motor_pwm1, left_speed);
  softPwmWrite(Left_motor_pwm2, left_speed);

  //右电机前进
  digitalWrite(Right_motor_go, HIGH);  //右电机前进使能
  digitalWrite(Right_motor_back, LOW); //右电机后退禁止
  softPwmWrite(Right_motor_pwm1, right_speed);
  softPwmWrite(Right_motor_pwm2, right_speed);
}
void right(int left_speed, int right_speed)
{
  //左电机前进
  digitalWrite(Left_motor_go, HIGH);   //左电机前进使能
  digitalWrite(Left_motor_back, LOW);  //左电机后退禁止
  softPwmWrite(Left_motor_pwm1, left_speed);
  softPwmWrite(Left_motor_pwm2, left_speed);

  //右电机停止
  digitalWrite(Right_motor_go, LOW);   //右电机前进禁止
  digitalWrite(Right_motor_back, LOW); //右电机后退禁止
  softPwmWrite(Right_motor_pwm1, right_speed);
  softPwmWrite(Right_motor_pwm2, right_speed);
}

void spin_left(int left_speed, int right_speed)
{
  //左电机后退
  digitalWrite(Left_motor_go, LOW);     //左电机前进禁止
  digitalWrite(Left_motor_back, HIGH);  //左电机后退使能
  softPwmWrite(Left_motor_pwm1, left_speed);
  softPwmWrite(Left_motor_pwm2, left_speed);

  //右电机前进
  digitalWrite(Right_motor_go, HIGH);  //右电机前进使能
  digitalWrite(Right_motor_back, LOW); //右电机后退禁止
  softPwmWrite(Right_motor_pwm1, right_speed);
  softPwmWrite(Right_motor_pwm2, right_speed);
}

void spin_right(int left_speed, int right_speed)
{
  //左电机前进
  digitalWrite(Left_motor_go, HIGH);    //左电机前进使能
  digitalWrite(Left_motor_back, LOW);   //左电机后退禁止
  softPwmWrite(Left_motor_pwm1, left_speed);
  softPwmWrite(Left_motor_pwm2, left_speed);

  //右电机后退
  digitalWrite(Right_motor_go, LOW);    //右电机前进禁止
  digitalWrite(Right_motor_back, HIGH); //右电机后退使能
  softPwmWrite(Right_motor_pwm1, right_speed);
  softPwmWrite(Right_motor_pwm2, right_speed);
}

void back(int time)
{
  //左电机后退
  digitalWrite(Left_motor_go, LOW);     //左电机前进禁止
  digitalWrite(Left_motor_back, HIGH);  //左电机后退使能
  softPwmWrite(Left_motor_pwm1, time);
  softPwmWrite(Left_motor_pwm2, time);

  //右电机后退
  digitalWrite(Right_motor_go, LOW);    //右电机前进禁止
  digitalWrite(Right_motor_back, HIGH); //右电机后退使能
  softPwmWrite(Right_motor_pwm1, time);
  softPwmWrite(Right_motor_pwm2, time);

}

void main()
{
  //wiringPi初始化
  wiringPiSetup();
	
  //初始化电机驱动IO口为输出方式
  pinMode(Left_motor_go, OUTPUT);
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT);
  pinMode(Right_motor_back, OUTPUT);

  //创建两个软件控制的PWM脚
  softPwmCreate(Left_motor_pwm1,0,255); 
  softPwmCreate(Left_motor_pwm2,0,255); 
  softPwmCreate(Right_motor_pwm1,0,255);
  softPwmCreate(Right_motor_pwm2,0,255);
  
  //定义按键接口为输入接口
  //pinMode(key, INPUT);

  //定义四路循迹红外传感器为输入接口
  pinMode(TrackSensorLeftPin1, INPUT);
  pinMode(TrackSensorLeftPin2, INPUT);
  pinMode(TrackSensorRightPin1, INPUT);
  pinMode(TrackSensorRightPin2, INPUT);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);

  int modee;
  printf("choose your mode:"); 
  scanf("%d",&modee);
  if(modee==1)
  {
    char ch;
    int value=200;
    system("stty -icanon");//关闭缓冲区，输入字符无需按回车键直接接受
    while (ch = getchar())
    {
      if(ch=='x')
      {
        printf("over\n");
        break;
      }
      else
      {
        float dis;
        switch (ch)
        {
        case 'a':
          spin_left(value,value);
          delay(50);
          setOut();
          break;
        case 'j':
          left(value,value);
          delay(50);
          setOut();
          break;
        case 'k':
          right(value,value);
          delay(50);
          setOut();
          break;
        case 'd':
          spin_right(value,value);
          delay(50);
          setOut();
          break;
        case 'w':     
          run(value,value);
          delay(50);
          setOut();
          break;
        case 's':
          back(value);
          delay(50);
          setOut();
          break;
        case 'q':
          printf("Left1:%d Left2:%d Right1:%d Right2:%d\n",digitalRead(TrackSensorLeftPin1),digitalRead(TrackSensorLeftPin2),digitalRead(TrackSensorRightPin1),digitalRead(TrackSensorRightPin2));
          break;
        case 'r':
          printf("hi\n");
          dis = disMeasure();  
          printf("distance = %0.2f cm\n",dis);  
          delay(50);
          break;
        default:
          break;
        }
      }
    }
  }else if(modee==2)
  {
  while(1)
  {
   TrackSensorLeftValue1  = digitalRead(TrackSensorLeftPin1);
   TrackSensorLeftValue2  = digitalRead(TrackSensorLeftPin2);
   TrackSensorRightValue1 = digitalRead(TrackSensorRightPin1);
   TrackSensorRightValue2 = digitalRead(TrackSensorRightPin2);

  //  if ( (TrackSensorLeftValue1 == LOW || TrackSensorLeftValue2 == LOW) &&  TrackSensorRightValue2 == LOW)
  //  {
  //    right(120, 120);
  //    delay(80);
  //  }
  //  //四路循迹引脚电平状态
  //  // 0 X 0 0       
  //  // 0 X 0 1 
  //  // 0 X 1 0       
  //  //处理左锐角和左直角的转动
  //  else if ( TrackSensorLeftValue1 == LOW && (TrackSensorRightValue1 == LOW ||  TrackSensorRightValue2 == LOW))
  //  {
  //    left(120, 120);
  //    delay(80);
  //  }
  //  // 0 X X X
  //  //最左边检测到
  //  else 
   if ( TrackSensorLeftValue1 == LOW)
   {
     //spin_left(200, 200);
      left(150,150);
      //delay(10);
   }
   // X X X 0
   //最右边检测到
   else if ( TrackSensorRightValue2 == LOW )
   {
    right(150,150);
     //delay(10);
   }
   //四路循迹引脚电平状态
   // X 0 1 X
   //处理左小弯
   else if ( TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == HIGH)
   {
     left(150, 150);
   }
   //四路循迹引脚电平状态
   // X 1 0 X  
   //处理右小弯
   else if (TrackSensorLeftValue2 == HIGH && TrackSensorRightValue1 == LOW)
   {
    right(150,150);
   }
   //四路循迹引脚电平状态
   // X 0 0 X
   //处理直线
   else if (TrackSensorLeftValue2 == LOW && TrackSensorRightValue1 == LOW)
   {
     run(150, 150);
   }
   //当为1 1 1 1时小车保持上一个小车运行状态
  }
  } else 
  {
    spin_right(150,150);
    delay(1000);
  }
 return;
}

