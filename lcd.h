#ifndef __LCD_H
#define	__LCD_H

#include "sys.h"

#define  data_pin  P3

sbit busy = P3^7;
sbit RS = P1^5;
sbit RW = P1^6;
sbit E = P1^7;
sbit CS1 = P2^2;
sbit CS2 = P2^1;

//公有函数
void clear(u8 flag);    //清屏
void lcd_init(u8 i);    //初始化lcd
void show16(u8 page,u8 column,u8 screen,u8 method,u8 *str); //汉字显示
void show8(u8 page,u8 column,u8 screen,u8 method,u8 *str);  //字符显示
void draw_dot(u8 draw_x,u8 draw_y); //绘制点
void show_sentence(u8 line,u8 *str,u8 word_cnt,u8 method);//显示一行文字
void show_screen(u8 *str, u8 method);//全屏显示

#endif