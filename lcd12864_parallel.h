
#include <reg51.h>
#include <intrins.h>
#include <math.h>
#include <stdio.h>

#define uchar unsigned char
#define uint  unsigned int
#define max(a,b)  (((a) > (b)) ? (a) : (b))
#define min(a,b)  (((a) > (b)) ? (b) : (a))
#define DATA_PORT	P1		//���ݿ�

sbit VER = P3 ^ 0;  //player��ֱ�˶�
sbit HOR = P3 ^ 2;  //playerˮƽ�ƶ�
sbit RST = P3 ^ 3;	
sbit PSB = P3 ^ 4;	
sbit RS = P3 ^ 5;
sbit RW = P3 ^ 6;
sbit EN = P3 ^ 7;

//���ܣ�æ���,�ڶ�Һ����ʾ������������ÿһ��ָ��֮ǰ,����Һ����ʾ���Ƿ����
void lcd_wait_busy(void);

//���ܣ���Һ����ʾ������������
uchar lcd_read_bit(void);

//���ܣ���Һ����ʾ������д����
void lcd_write_bit(uchar d);

//���ܣ���Һ����ʾ��������ָ��
void lcd_write_command(uchar c);

//���ܣ�����LCD��ʾ
void lcd_on(void);

//���ܣ�Һ����ʾ��������ʼ��
void lcd_init(void);

//����:������ͼ������ʾ
void lcd_graph_on(void);

//����:�رջ�ͼ������ʾ
void lcd_graph_off(void);

//����:����(x,y)��Ӧ��ͼ�����ڲ�RAMָ��
void lcd_set_pos(uchar x, uchar y);

//���򵥸����ص�
void lcd_setPixtel(int x,int y);

//��Բ
void drawCircle_Mid(int x0, int y0, int r);

void lcd_display_all();
