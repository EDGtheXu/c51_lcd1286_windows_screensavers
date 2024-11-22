
#include <reg51.h>
#include <intrins.h>
#include <math.h>
#include <stdio.h>

#define uchar unsigned char
#define uint  unsigned int
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) > (b)) ? (b) : (a))
#define DATA_PORT	P1		//数据口

sbit VER = P3 ^ 0;  //player竖直运动
sbit HOR = P3 ^ 2;  //player水平移动
sbit RST = P3 ^ 3;	
sbit PSB = P3 ^ 4;	
sbit RS = P3 ^ 5;
sbit RW = P3 ^ 6;
sbit EN = P3 ^ 7;

//功能：忙检测,在对液晶显示控制器操作的每一条指令之前,需检测液晶显示器是否空闲
void lcd_wait_busy(void);

//功能：从液晶显示控制器读数据
uchar lcd_read_bit(void);

//功能：向液晶显示控制器写数据
void lcd_write_bit(uchar d);

//功能：向液晶显示控制器送指令
void lcd_write_command(uchar c);

//功能：开启LCD显示
void lcd_on(void);

//功能：液晶显示控制器初始化
void lcd_init(void);

//功能:开启绘图区域显示
void lcd_graph_on(void);

//功能:关闭绘图区域显示
void lcd_graph_off(void);

//功能:设置(x,y)对应绘图区域内部RAM指针
void lcd_set_pos(uchar x, uchar y);

//反向单个像素点
void lcd_setPixtel(int x,int y);

//画圆
void drawCircle_Mid(int x0, int y0, int r);

void lcd_display_all();
