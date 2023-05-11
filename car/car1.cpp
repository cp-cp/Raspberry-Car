#include<iostream>
#include<cstdio>
#include<wiringPi.h>

class Car
{
    public:
        int enab_pin[4]={5,6,13,19};
        int inx_pin[4]={21,22,23,24};
        int RightAhead_pin = inx_pin[0];
        int RightBack_pin = inx_pin[1];
        int LeftAhead_pin = inx_pin[2];
        int LeftBack_pin = inx_pin[3];
        void setup()
        {
            wiringPiSetup();
            for(int i=0;i<4;i++) {
                pinMode(enab_pin[i], OUTPUT);
                digitalWrite(enab_pin[i],1);
            }
            for(int i=0;i<4;i++) {
                pinMode(inx_pin[i], OUTPUT);
                digitalWrite(inx_pin[i],0);

            }

        }
        void fornt()
        {
            setup();
            digitalWrite(RightAhead_pin,1);
            digitalWrite(LeftAhead_pin,0);
        }
        void blink()
        {
            setup();
            digitalWrite(LED,1);  // pin , situation
            delay(500)
            digitalWrite(LED,0);
            delay(500);
            printf("This is a blink");
        };


    private:
};
Car firstCar;
int main()
{
    firstCar.front();
    return 0;
}