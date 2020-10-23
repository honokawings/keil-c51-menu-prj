/**
 * @file: uart.c
 * @author: Honokawings (1915569481@qq.com)
 * @description: 建议串口通信，裸机debug时使用
 * @version 0.1
 * @date: 2020-10-23 16:10:45
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "uart.h"

void config_uart(unsigned int baud)
{
    SCON=0x50;
    TMOD&=0x0f;
    TMOD|=0x20;
    TH1=256-(11059200/12/32)/baud;
    TL1=TH1;
    ET1=0;
    TR1=1;
}

void uart_send(char *str)
{
    SBUF= *str;
    while(!TI);
    TI=0;
}