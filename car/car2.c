#include<stdio.h>
#include<wiringPi.h>
#include<softPwm.h>
#include <stdio.h>  
#include <stdbool.h>
#include <sys/time.h>  


int power_pin[4]={21,22,23,24}; //使能开启

int Left_motor_go = 29;      //左电机前进方向连接Raspberry的wiringPi编码29口
int Left_motor_back = 3;     //左电机后退方向连接Raspberry的wiringPi编码3口

int Right_motor_go = 4;      //右电机前进方向连接Raspberry的wiringPi编码4口
int Right_motor_back = 5;    //右电机后退方向连接Raspberry的wiringPi编码5口

int TCRT_IN_1 = 0;
int TCRT_IN_2 = 25;
int TCRT_IN_3 = 1;
int TCRT_IN_4 = 27;
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
void setUp()
{
  wiringPiSetup();
  //L298N
  for(int i=0;i<4;i++)
  {
    pinMode(power_pin[i],OUTPUT);
    printf("pin%d out\n",power_pin[i]);
    digitalWrite(power_pin[i],HIGH);
  }
  pinMode(Left_motor_go, OUTPUT);
  pinMode(Left_motor_back, OUTPUT);
  pinMode(Right_motor_go, OUTPUT);
  pinMode(Right_motor_back, OUTPUT);
  //TCRT
  pinMode(TCRT_IN_1, INPUT);
  pinMode(TCRT_IN_2, INPUT);
  pinMode(TCRT_IN_3, INPUT);
  pinMode(TCRT_IN_4, INPUT);
  // pinMode(TCRT_IN_5, INPUT);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  //超声波
  ultraInit(); 
}
void front()
{
  digitalWrite(Left_motor_go,HIGH);
  digitalWrite(Right_motor_go,HIGH);
  printf("The car go straight on!\n");
}

void back()
{
  digitalWrite(Left_motor_back,HIGH);
  digitalWrite(Right_motor_back,HIGH);
  printf("The car back to tail!\n");
}

void left()
{
  digitalWrite(Left_motor_back,HIGH);
  digitalWrite(Right_motor_go,HIGH);
  printf("The car turns right!\n");
}

void right()
{
  digitalWrite(Right_motor_back,HIGH);
  digitalWrite(Left_motor_go,HIGH);
  printf("The car turns right!\n");
}               
void setOut()
{
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  printf("MOTOR END UP!\n");
}
void powerOff()
{
  for(int i=0;i<4;i++)
  {
    digitalWrite(power_pin[i],LOW);
  }
  printf("MOTOR POWER OFF!\n");
}
int main() 
{
  setUp();
  int modee;
  printf("choose your mode:"); 
  scanf("%d",&modee);
  if(modee==1)
  {
    char ch;
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
          left();
          delay(50);
          setOut();
          break;
        case 'd':
          right();
          delay(50);
          setOut();
          break;
        case 'w':     
          printf("hi %d %d\n",digitalRead(TCRT_IN_1),digitalRead(TCRT_IN_2));
          dis = disMeasure();  
          if(dis<50.00)
          {
            printf("warning！！！！！！！！distance = %0.2f cm\n",dis);  
            delay(1000);
          }
          else
          {
           // if(digitalRead(TCRT_IN_2))left();
           // else if(digitalRead(TCRT_IN_1))right();
        //    else 
	          front();
            delay(50);
          }
          setOut();
          break;
        case 's':
          
          if(digitalRead(TCRT_IN_1))left();
          else if(digitalRead(TCRT_IN_2))right();
          else back();
          delay(50);
          setOut();
          break;
        case 'r':
          
          printf("hi\n");
          dis = disMeasure();  
          printf("distance = %0.2f cm\n",dis);  
          delay(50);
          break;
        case 't':
          printf("In_1:%d In_2:%d In_3:%d In_4:%d \n",digitalRead(TCRT_IN_1),digitalRead(TCRT_IN_2),digitalRead(TCRT_IN_3),digitalRead(TCRT_IN_4));
          break;
        default:
          break;
        }
      }
    }
  }
  else
  {
    int value = 466;  //一周期内高电平的时间
    int i;
    while(1){  // 设置一个周期为1ms，占空比为60%
        //delay(500);
        printf("hi %d %d\n",digitalRead(TCRT_IN_2),digitalRead(TCRT_IN_3));
        if(!digitalRead(TCRT_IN_2))right(),value=300;
        else if(!digitalRead(TCRT_IN_3))left(),value=300;
        // else if()
        // else if(digitalRead(TCRT_IN_1))left();
        // else if(digitalRead(TCRT_IN_4))right();
        else 
            front(),value=466;
        delayMicroseconds(value); 
        setOut();
        delayMicroseconds(1000-value);
        value=466;
    }
    // if(digitalRead(TCRT_IN_2))left();
    // else if(digitalRead(TCRT_IN_1))right();
    // else front();
    // delay(50);
  }
  setOut();
}
