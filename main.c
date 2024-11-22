#include"lcd12864_parallel.h"

void main()
{
    int cnt = 50;
	lcd_init();
	while(1) {
        if(--cnt<0){
            cnt = 50;
            lcd_display_all();
        }
	}
}
