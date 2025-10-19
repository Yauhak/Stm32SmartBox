#include "stm32f10x.h"
#include "Delay.h"

//用到的引脚与端口
#define GPIO_OUT_PINS GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9
#define GPIO_IN_PINS GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3
#define GPIO_OUT_PORT GPIOB
#define GPIO_IN_PORT GPIOA

//特殊键值
//@1 撤回（删除前一个字符），于字符接收终端响应
//@2 确认（回车），于字符接收终端响应
//@3 键盘锁，键盘驱动程序直接响应
//@4 切换键盘模式，键盘驱动程序直接响应
//@5-8 上下左右
#define UNDO 1
#define OK 2
#define CAPSLOCK 3
#define SWITCH_MODE 4
#define UP 5
#define DOWN 6
#define LEFT 7
#define RIGHT 8

const short rows[] = {6, 7, 8, 9};
const short cols[] = {3,2,1,0};

short mode = 0;
short capslock = 0;

char convert_char_bitwise(char ch) {
	if (capslock) {
		if (ch >= 'a' && ch <= 'z') {
			return ch - 32;
		}
	}
	return ch;
}

const char keys[5][16] = {
	{
		'1', '2', '3', OK,
		'4', '5', '6', UNDO,
		'7', '8', '9', CAPSLOCK,
		'(', '0', ')', SWITCH_MODE
	},

	{
		'a', 'b', 'c', OK,
		'd', 'e', 'f', UNDO,
		'g', 'h', 'i', CAPSLOCK,
		'j', 'k', 'l', SWITCH_MODE
	},

	{
		'm', 'n', 'o', OK,
		'p', 'q', 'r', UNDO,
		's', 't', 'u', CAPSLOCK,
		'v', 'w', 'x', SWITCH_MODE
	},

	{
		'y', 'z', '+', OK,
		'|', '&', '-', UNDO,
		'%', '=', '*', CAPSLOCK,
		' ', '.', '/', SWITCH_MODE
	},

	{
		UP, UP, UP, OK,
		DOWN, DOWN, DOWN, UNDO,
		LEFT, LEFT, LEFT, CAPSLOCK,
		RIGHT, RIGHT, RIGHT, SWITCH_MODE
	}
};

void initialize_gpio_about_key() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_IN_PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_IN_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_OUT_PINS;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_OUT_PORT, &GPIO_InitStructure);
}

void pull_up_rows_except_one(short row) {
	for (short i = 0; i <= 3; i++) {
		if (i == row)
			GPIO_ResetBits(GPIOB, 1 << rows[i]);
		else
			GPIO_SetBits(GPIOB, 1 << rows[i]);
	}
}

short check_cols() {
	for (short i = 0; i <= 3; i++) {
		if (!GPIO_ReadInputDataBit(GPIOA, 1 << cols[i]))
			return i;
	}
	return -1;
}

short r_and_c() {
	short i, j;
	for (i = 0; i <= 3; i++) {
		pull_up_rows_except_one(i);
		Delay_ms(25);
		j = check_cols();
		if (j != -1)
			return i * 4 + j + 1;
	}
	return 0;
}

char scan_key() {
	short key_value;
	key_value = r_and_c();
	Delay_ms(25);
	if (key_value) {
		if (mode==4 && key_value % 4){
			char keys[]={UP,DOWN,LEFT,RIGHT};
			return keys[(key_value-1)/4];
		}else if (key_value == 4){
			return 2;
		}else if (key_value == 8){
			return 1;
		}else if (key_value == 16){
			mode = (mode + 1) % 5;
			return 4;
		}else if (key_value == 12){
			capslock = !capslock;
			return 3;
		}else{
			return convert_char_bitwise(keys[mode][key_value-1]);
		}
	} else {
		return 0;
	}
}
