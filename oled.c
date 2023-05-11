// An highlighted block
#include <wiringPi.h>
#include <stdio.h>  
#include <wiringPiSPI.h>

#define LCD_DC  3     // 模式选择 1：写数据 0：写命令
#define LCD_DIN 12     // 串行数据线
#define LCD_CLK 14     // 串行时钟线
#define LCD_RST 0    // 复位信号  低电平有效
#define LCD_CE  10     // 芯片使能  低电平有效
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

#define OLED_MODE 0
#define SIZE 16
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	 

#define u16 unsigned int 
#define u8  unsigned char
const unsigned char F6x8[][6];
const unsigned char F8X16[];
unsigned char BMP2[];
unsigned char BMP1[];
unsigned char picture6[];
unsigned char picture7[];
unsigned char picture8[];
unsigned char picture9[];
char Hzk[][32];
#define X_Col_addr  0x80  // 定位列 地址 0-83
#define Y_Row_addr  0x40  // 定位行 地址 0-5				
// 初始化GPIO 端口
void gpioInit(){
    pinMode(LCD_DC,OUTPUT);
    pinMode(LCD_RST,OUTPUT);
    pinMode(LCD_CE,OUTPUT);
}

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 
	lcd_writeByte(0xb0+y,OLED_CMD);
	lcd_writeByte((((x+2)&0xf0)>>4)|0x10,OLED_CMD);
	lcd_writeByte(((x+2)&0x0f),OLED_CMD); 
} 
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!	  
void lcd_clear(void)  
{  
	u8 i,n;		    
	for(i=0;i<8;i++)  
	{  
		lcd_writeByte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
		lcd_writeByte (0x02,OLED_CMD);      //设置显示位置—列低地址
		lcd_writeByte (0x10,OLED_CMD);      //设置显示位置—列高地址   
		for(n=0;n<128;n++)lcd_writeByte(0,OLED_DATA); 
	} //更新显示
}

//显示汉字
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	u8 t,adder=0;
	OLED_Set_Pos(x,y);	
    for(t=0;t<16;t++)
		{
				lcd_writeByte(Hzk[2*no][t],OLED_DATA);
				adder+=1;
     }	
		OLED_Set_Pos(x,y+1);	
    for(t=0;t<16;t++)
			{	
				lcd_writeByte(Hzk[2*no+1][t],OLED_DATA);
				adder+=1;
      }					
}

/***********功能描述：显示显示BMP图片128×64起始点坐标(x,y),x的范围0～127，y为页的范围0～7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[])
{ 	
 unsigned int j=0;
 unsigned char x,y;
  
  if(y1%8==0) y=y1/8;      
  else y=y1/8+1;
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
    for(x=x0;x<x1;x++)
	    {      
	    	lcd_writeByte(BMP[j++],OLED_DATA);	    	
	    }
	}
} 


//在指定位置显示一个字符,包括部分字符
//x:0~127
//y:0~63
//mode:0,反白显示;1,正常显示				 
//size:选择字体 16/12 
void OLEDaL:aL:aL:aL:aL:aL:_ShowChar(u8 x,u8 y,u8 chr)
{      	
	unsigned char c=0,i=0;	
		c=chr-' ';//得到偏移后的值			
		if(x>Max_Column-1){x=0;y=y+2;}
		if(SIZE ==16)
			{
			OLED_Set_Pos(x,y);	
			for(i=0;i<8;i++)
			lcd_writeByte(F8X16[c*16+i],OLED_DATA);
			OLED_Set_Pos(x,y+1);
			for(i=0;i<8;i++)
			lcd_writeByte(F8X16[c*16+i+8],OLED_DATA);
			}
			else {	
				OLED_Set_Pos(x,y+1);
				for(i=0;i<6;i++)
				lcd_writeByte(F6x8[c][i],OLED_DATA);
				
			}
}

//m^n函数
unsigned int oled_pow(u8 m,u8 n)
{
	unsigned int result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//显示2个数字
//x,y :起点坐标	 
//len :数字的位数
//size:字体大小
//mode:模式	0,填充模式;1,叠加模式
//num:数值(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,unsigned int num,u8 len,u8 size)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size/2)*t,y,' ');
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size/2)*t,y,temp+'0'); 
	}
} 

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,u8 *chr)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j]);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}



// 初始化液晶
void lcd_init(){
    gpioInit();
    int isOK = wiringPiSPISetup(0, 1000000);
    if (isOK == -1) {
        printf("SPI设置失败\n");
    }
    else {
        printf("SPI设置OK == %d\n",isOK);
    }
    digitalWrite(LCD_RST,LOW); // 液晶复位
    delay(200);
    digitalWrite(LCD_RST,HIGH);
    delay(20);
    	lcd_writeByte(0xAE,OLED_CMD);//--turn off oled panel
	lcd_writeByte(0x02,OLED_CMD);//---set low column address
	lcd_writeByte(0x10,OLED_CMD);//---set high column address
	lcd_writeByte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
	lcd_writeByte(0x81,OLED_CMD);//--set contrast control register
	lcd_writeByte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
	lcd_writeByte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
	lcd_writeByte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
	lcd_writeByte(0xA6,OLED_CMD);//--set normal display
	lcd_writeByte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	lcd_writeByte(0x3f,OLED_CMD);//--1/64 duty
	lcd_writeByte(0xD3,OLED_CMD);//-set display offset	Shift Mapping RAM Counter (0x00~0x3F)
	lcd_writeByte(0x00,OLED_CMD);//-not offset
	lcd_writeByte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
	lcd_writeByte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
	lcd_writeByte(0xD9,OLED_CMD);//--set pre-charge period
	lcd_writeByte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
	lcd_writeByte(0xDA,OLED_CMD);//--set com pins hardware configuration
	lcd_writeByte(0x12,OLED_CMD);
	lcd_writeByte(0xDB,OLED_CMD);//--set vcomh
	lcd_writeByte(0x40,OLED_CMD);//Set VCOM Deselect Level
	lcd_writeByte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
	lcd_writeByte(0x02,OLED_CMD);//
	lcd_writeByte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
	lcd_writeByte(0x14,OLED_CMD);//--set(0x10) disable
	lcd_writeByte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
	lcd_writeByte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
	lcd_writeByte(0xAF,OLED_CMD);//--turn on oled panel
	
	lcd_writeByte(0xAF,OLED_CMD); /*display ON*/ 
    lcd_clear(); // 清屏
    OLED_Set_Pos(0,0); 
}

int main (void)
{
	unsigned char t;
    wiringPiSetup();
    lcd_init();
    lcd_clear(); // 清屏
    t=' ';
    while(1)
    {
		lcd_clear();
	//	LED_ON;
		OLED_ShowCHinese(0,0,0);//中
		OLED_ShowCHinese(18,0,1);//景
		OLED_ShowCHinese(36,0,2);//园
		OLED_ShowCHinese(54,0,3);//电
		OLED_ShowCHinese(72,0,4);//子
		OLED_ShowCHinese(90,0,5);//科
		OLED_ShowCHinese(108,0,6);//技
		OLED_ShowString(0,3,"1.3' OLED TEST");
		//OLED_ShowString(8,2,"ZHONGJINGYUAN");  
	 //	OLED_ShowString(20,4,"2014/05/01");  
		OLED_ShowString(0,6,"ASCII:");  
		OLED_ShowString(63,6,"CODE:");  
		OLED_ShowChar(48,6,t);//显示ASCII字符	   
		t++;
		if(t>'~')t=' ';
		OLED_ShowNum(103,6,t,3,16);//显示ASCII字符的码值 	
		delay(2000);
		lcd_clear();
		delay(100);
	//		LED_OFF;
		OLED_DrawBMP(0,0,128,8,BMP1);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
		delay(1000);
		OLED_DrawBMP(0,0,128,8,picture6);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
		delay(200);
		OLED_DrawBMP(0,0,128,8,picture7);
		delay(200);
		OLED_DrawBMP(0,0,128,8,picture8);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
		delay(200);
		OLED_DrawBMP(0,0,128,8,picture9);
		delay(200);
		OLED_DrawBMP(0,0,128,8,picture6);  //图片显示(图片显示慎用，生成的字表较大，会占用较多空间，FLASH空间8K以下慎用)
		delay(200);
		OLED_DrawBMP(0,0,128,8,BMP2);
		delay(1000);
	}
}
//取模部分代码太长了，需要的去资源下载或者点赞收藏私信博主获取。
