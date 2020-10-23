#ifndef __SYS_H
#define __SYS_H

#include <reg51.h>
#include <stddef.h>
#include <stdlib.h>

typedef unsigned char u8;
typedef unsigned int  u16;

#include "menu.h"
#include "key.h"
#include "lcd.h"
#include "uart.h"

//公有函数
void interrupt_init();
void timer0_init();
void timer1_init();

extern char last_operation;
extern char now_operation;

#endif