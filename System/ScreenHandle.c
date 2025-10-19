#include "stm32f10x.h"
#include "OLED.h"
#include "MatrixKeyboard.h"

#define SCREEN_BUFF_SIZE 256
#define SCREEN_WIDTH 16
#define SCREEN_HEIGHT 4

#define UNDO 1
#define OK 2
#define CAPSLOCK 3
#define SWITCH_MODE 4
#define UP 5
#define DOWN 6
#define LEFT 7
#define RIGHT 8

char screen_buff[256] = {0};
short total_sc_buff_index = 0;
short is_ok = 0;

short len_buff(){
	short i=0;
	char *ptr=screen_buff;
	while(*ptr){
		i++;
		ptr++;
	}return i;
}

void update_screen_buff(short gap) {
    if (total_sc_buff_index < SCREEN_BUFF_SIZE - 1) return;
    for (short i = 0; i < SCREEN_BUFF_SIZE - gap; i++) {
        screen_buff[i] = screen_buff[i + gap];
    }
    for (short i = 0; i < gap; i++) {
        screen_buff[SCREEN_BUFF_SIZE - 1 - i] = 0;
    }
    total_sc_buff_index -= gap; 
    if (total_sc_buff_index < 0) total_sc_buff_index = 0;
}

void update_echo(const char *c) {
    const char *ptr = c;
    while (*ptr) {
        update_screen_buff(1);
        screen_buff[total_sc_buff_index] = *ptr;
        if (total_sc_buff_index < SCREEN_BUFF_SIZE - 1) {
            total_sc_buff_index++;
        }
        ptr++;
    }
    //screen_buff[total_sc_buff_index] = '\0';
}

void handle_keypress() {
	char x = scan_key();
	if (x > 0) {
		if (x && x >= 32) {
			char ary[]={x,0};
			update_echo(ary);
		} else if (x == UNDO) {
			if (total_sc_buff_index > 0) {
        total_sc_buff_index--;
        screen_buff[total_sc_buff_index] = 0;
    }
		} else if (x == OK) {
			is_ok = 1;
		} else if (x == UP) {
			if (total_sc_buff_index > 16)total_sc_buff_index -= 16;
		} else if (x == DOWN) {
			if (total_sc_buff_index + 16 <= len_buff())
				total_sc_buff_index += 16;
			else if ((15-(total_sc_buff_index-1)%16)+((len_buff()-1)%16+1)<=16)
				total_sc_buff_index+=16-(total_sc_buff_index-1)%16;
		} else if (x == LEFT) {
			if (total_sc_buff_index)total_sc_buff_index--;
		} else if (x == RIGHT) {
			if (total_sc_buff_index < len_buff())total_sc_buff_index++;
		}
		short total_x = (total_sc_buff_index-1)%16+1;
		short total_y = (total_sc_buff_index - total_x) / 16 + 1;
		OLED_Clear();
		OLED_ShowString_Limit(1, 1, screen_buff + ((total_y - 4) > 0 ? (total_y - 4) * 16 : 0), 16 * 4);
		OLED_ShowChar_Reverse(total_y>4?4:total_y,total_x,screen_buff[total_sc_buff_index-1]);
	}
}
