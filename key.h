#ifndef __KEY_H
#define	__KEY_H

#include "sys.h"

sbit key_a_now = P0^2;
sbit key_b_now = P0^4;
sbit key_o = P0^1;

//公有函数
void key_init();
char key_scan();
char key_analyze(char scan_result_analyze);

#endif