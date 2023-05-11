//无源蜂鸣器
#include <wiringPi.h>
#include <softTone.h>
#include <stdio.h>

#define makerobo_BuzPin    7 // 有源蜂鸣器管脚定义

// 音谱定义
// 低C音符的频率
#define  Tone_CL1  131
#define  Tone_CL2  147
#define  Tone_CL3  165
#define  Tone_CL4  175
#define  Tone_CL5  196
#define  Tone_CL6  221
#define  Tone_CL7  248

// 中C音的频率
#define  Tone_CM1  262
#define  Tone_CM2  294
#define  Tone_CM3  330
#define  Tone_CM4  350
#define  Tone_CM5  393
#define  Tone_CM6  441
#define  Tone_CM7  495

// 高C音符的频率
#define  Tone_CH1  525
#define  Tone_CH2  589
#define  Tone_CH3  661
#define  Tone_CH4  700
#define  Tone_CH5  786
#define  Tone_CH6  882
#define  Tone_CH7  990

// 第一首歌音谱
int makerobo_song_1[] = {Tone_CM3,Tone_CM5,Tone_CM6,Tone_CM3,Tone_CM2,Tone_CM3,Tone_CM5,Tone_CM6,Tone_CH1,Tone_CM6,Tone_CM5,Tone_CM1,Tone_CM3,Tone_CM2,
                         Tone_CM2,Tone_CM3,Tone_CM5,Tone_CM2,Tone_CM3,Tone_CM3,Tone_CL6,Tone_CL6,Tone_CL6,Tone_CM1,Tone_CM2,Tone_CM3,Tone_CM2,Tone_CL7,
                         Tone_CL6,Tone_CM1,Tone_CL5};
// 第一首歌节拍
int makerobo_beat_1[] = {1,1,3,1,1,3,1,1,1,1,1,1,1,1,3,1,1,3,1,1,1,1,1,1,1,2,1,1,
                         1,1,1,1,1,1,3};

// 第二首歌音谱
int makerobo_song_2[] = {Tone_CM1,Tone_CM1,Tone_CM1,Tone_CL5,Tone_CM3,Tone_CM3,Tone_CM3,Tone_CM1,Tone_CM1,Tone_CM3,Tone_CM5,Tone_CM5,Tone_CM4,Tone_CM3,Tone_CM2,
                         Tone_CM2,Tone_CM3,Tone_CM4,Tone_CM4,Tone_CM3,Tone_CM2,Tone_CM3,Tone_CM1,Tone_CM1,Tone_CM3,Tone_CM2,Tone_CL5,Tone_CL7,Tone_CM2,Tone_CM1
                        };
// 第二首歌节拍
int makerobo_beat_2[] = {1,1,1,3,1,1,1,3,1,1,1,1,1,1,3,1,1,1,2,1,1,1,3,1,1,1,3,3,2,3};

int makerobo_song_3[] ={Tone_CM1,Tone_CM2,Tone_CM3,Tone_CM5,Tone_CM3,Tone_CM3,Tone_CM2,Tone_CM2,Tone_CM1,Tone_CM2,Tone_CM1,Tone_CL6,Tone_CM1,Tone_CM2,Tone_CM3,Tone_CM3};
int makerobo_beat_3[] ={2,2,2,2,2,4,1,1,2,4,2,2,2,2,4,4};

int main()
{
    int i;
    //初始化连接失败时，将消息打印到屏幕
    if(wiringPiSetup() == -1){
        printf("makerobo setup wiringPi failed !");
        return 1;
    }
    //软件模拟Tone库初始化识别，将消息打印到屏幕
    if(softToneCreate(makerobo_BuzPin) == -1){
        printf("makerobo setup softTone failed !");
        return 1;
    }

    while(1){
        printf("makerobo music is being played...\n");  // 开始播放音乐
        // 播放第一首歌音乐
        for(i=0;i<sizeof(makerobo_song_3)/4;i++){
            softToneWrite(makerobo_BuzPin, makerobo_song_3[i]); 
            //第一首歌音谱
            delay(makerobo_beat_3[i] *100);   //第一首歌节拍
        }
        // // 播放第二首歌音乐
        // for(i=0;i<sizeof(makerobo_song_2)/4;i++){
        //     softToneWrite(makerobo_BuzPin, makerobo_song_2[i]); 
        //     //第二首歌音谱
        //     delay(makerobo_beat_2[i] * 500);     //第二首歌节拍
        // }
    }
    return 0;
}


