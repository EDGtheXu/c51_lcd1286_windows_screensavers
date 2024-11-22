#include "lcd12864_parallel.h"
#include <stdlib.h>

#define objcount 8
int objs[objcount][5];

int playerx;
int playery;
int playerr;

//���ܣ�æ���,�ڶ�Һ����ʾ������������ÿһ��ָ��֮ǰ,����Һ����ʾ���Ƿ����
void lcd_wait_busy(void)                  //������æ����ӳ���
{
	uchar i;
	DATA_PORT = 0xff;
	RS = 0;
	RW = 1;
	EN = 1;

	for(i = 0; i < 20; i++)             //
		if((DATA_PORT & 0x80) != 0x80)
			break;  //D7=0 �����˳�

	EN = 0;
}

//���ܣ���Һ����ʾ������������
uchar lcd_read_bit(void)               //��Һ����ʾ������������
{
	uchar d=0;
	lcd_wait_busy();                      //ST7920Һ����ʾ������æ���
	RS = 1;                          //RS=1 RW=1 E=������
	RW = 1;
	EN = 1;

	d = DATA_PORT;
	EN = 0;
	return d;
}
//���ܣ���Һ����ʾ������д����
void lcd_write_bit(uchar d)             //��Һ����ʾ������д����
{
	lcd_wait_busy();                      //ST7920Һ����ʾ������æ���
	DATA_PORT = d;
	RS = 1;                          //RS=1 RW=0 E=������
	RW = 0;
	EN = 1;

	EN = 0;
}
//���ܣ���Һ����ʾ��������ָ��
void lcd_write_command(uchar c)             //��Һ����ʾ��������ָ��
{
	lcd_wait_busy();                      //ST7920Һ����ʾ������æ���
	DATA_PORT = c;
	RS = 0;                          //RS=0 RW=0 E=������
	RW = 0;
	EN = 1;
    
	EN = 0;
}
//���ܣ�����LCD��ʾ
void lcd_on(void)                        //LCD��ʾ�����ӳ���
{
	lcd_write_command(0x30);                        //����Ϊ����ָ�
	lcd_write_command(0x0c);
}

//����:������ͼ������ʾ
void lcd_graph_on(void)                  //��ͼ������ʾ�����ӳ���
{
	lcd_write_command(0x36);							//����ָ���8λ������ͼ
	lcd_write_command(0x30);                        //�ָ�Ϊ����ָ�
}
//����:�رջ�ͼ������ʾ
void lcd_graph_off(void)                  //��ͼ������ʾ�ر��ӳ���
{
	lcd_write_command(0x34);							//����ָ���8λ���ػ�ͼ
}

//����:����(x,y)��Ӧ��ͼ�����ڲ�RAMָ��
void lcd_set_pos(uchar x, uchar y) 
{
	//lcd_write_command(0x36);                        //��չָ���8λ������ͼ

	if(y >= 32) {
		lcd_write_command((y - 32) + 0x80);	//�°���������Y��ַ
		lcd_write_command((x / 16) + 0x88);	//�°���������X��ַ

	} else {
		lcd_write_command(y + 0x80);		//�ϰ���
		lcd_write_command((x / 16) + 0x80);	//�ϰ���
	}

	//lcd_write_command(0x30);                        //�ָ�Ϊ����ָ�
}

//���򵥸�����
void lcd_setPixtel(int x,int y){
    uchar tmph=0, tmpl=0;
    uchar x_byte, x_bit;
    uchar y_byte, y_half_screen; 
    uchar row = y % 32;

    if(x < 0||x>127||y<0||y>63) return;
    x_byte = x / 16; 
    x_bit = x % 16; 
 
    y_byte = y / 32; 
    y_half_screen = y_byte * 8; 

    lcd_set_pos(x, y);
    lcd_read_bit();

    tmph = lcd_read_bit();
    tmpl = lcd_read_bit();

    lcd_set_pos(x, y);
    if (x_bit < 8) {
        
        tmph ^= (0x01 << (7 - x_bit)); 
        lcd_write_bit(tmph);

    } else {

        lcd_write_bit(tmph);
        tmpl ^= (0x01 << (15 - x_bit));
        lcd_write_bit(tmpl);

    }
}

//��Բ
void drawCircle_Mid(int x0, int y0, int r) {
	int x = 0, y = r,
		d = 1 - r,
		dx = 3, dy = 2 - r - r;

    y0 = 63 - y0;
	lcd_setPixtel(x+x0, y+y0);
	while (x < y) {
		if (d < 0) {
			d += dx;
			dx += 2;
			x++;
		}
		else {
			d += (dx + dy);
			dx += 2;dy += 2;
			x++;
			y--;
		}

		lcd_setPixtel(x + x0, y + y0);
		lcd_setPixtel(-x + x0, -y + y0);
		lcd_setPixtel(x + x0, -y + y0);
		lcd_setPixtel(-x + x0, y + y0);

		lcd_setPixtel(y + x0, x + y0);
		lcd_setPixtel(-y + x0, -x + y0);
		lcd_setPixtel(-y + x0, x + y0);
		lcd_setPixtel(y + x0, -x + y0);
	}

}

double distance(int x1,int y1,int x2,int y2){
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

//��ʼ��LCD��С��
void lcd_init(void)                     //Һ����ʾ��������ʼ���ӳ���
{
    uchar i = 0;
    
	lcd_write_command(0x33);                        //�ӿ�ģʽ���ã�8λ������ָ�
	lcd_write_command(0x30);                        //����ָ���8λ���ƿ�
	lcd_write_command(0x30);                        //�ظ��ͻ���ָ�
	lcd_write_command(0x01);                        //����������
	lcd_on();                            //����ʾ
    PSB = 1;		//����

    
    lcd_graph_off();
    playerx = 64;
    playery = 32;
    playerr = 10;
    drawCircle_Mid(playerx,playery,playerr);
    srand(5);
    for(i=0;i<objcount;i++){
        int r1;
        objs[i][0] = (i+1)*15;          //x
        objs[i][1] = rand() % 20 + 22;  //y
        objs[i][2] = rand() % 5 + 5;    //r
        while(!(r1 = rand() % 8 - 4));  
        objs[i][3] = r1;                //vx
        while(!(r1 = rand() % 8 - 4));  
        objs[i][4] = r1;                //vy

        drawCircle_Mid(objs[i][0],objs[i][1],objs[i][2]);
    }
    
}

void lcd_display_all(){
    uchar count,k;
    lcd_graph_off();
    //lcd_clear_screen();

    drawCircle_Mid(playerx,playery,10);
    if(HOR == 1) playerx+=1;
    else playerx-=1;
    if(VER == 1) playery+=1;
    else playery-=1;
    playerx = (playerx + 128) % 128;
    playery = (playery + 64) % 64;
    drawCircle_Mid(playerx,playery,playerr);

    for(count=0;count<objcount;count++){
        int nx = objs[count][0] + objs[count][3];
        int ny = objs[count][1] + objs[count][4];
        drawCircle_Mid(objs[count][0],objs[count][1],objs[count][2]);

        // ���x������ײ���ٶȷ���
        if(nx < objs[count][2]) objs[count][3] = fabs(objs[count][3]);
        else if(nx> 127-objs[count][2])  objs[count][3] = -fabs(objs[count][3]);
        // ���y������ײ���ٶȷ���

        if(ny< objs[count][2] ) objs[count][4] = fabs(-objs[count][4]);
        else if(ny> 63-objs[count][2])  objs[count][4] = -fabs(-objs[count][4]);

        for(k=count+1;k<objcount;k++){
            if(distance(nx,ny,objs[k][0],objs[k][1]) < objs[k][2] + objs[k][2]){
                objs[count][3] = fabs(objs[count][3]) * (nx>objs[k][0]?1:-1);
                objs[count][4] = fabs(objs[count][4]) * (ny>objs[k][1]?1:-1);

                objs[k][3] = fabs(objs[k][3]) * (nx>objs[k][0]?-1:1);
                objs[k][4] = fabs(objs[k][4]) * (ny>objs[k][1]?-1:1);
            }
        }
        //�ƶ�
        objs[count][0] += objs[count][3];
        objs[count][1] += objs[count][4];

        
        drawCircle_Mid(objs[count][0],objs[count][1],objs[count][2]);
    }

    lcd_graph_on();
}


