/**
 * @file: sys.c
 * @author: Honokawings (1915569481@qq.com)
 * @description: 中断处理
 * @version 0.1
 * @date: 2020-10-21 18:10:68
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "sys.h"
#include "menu.h"

/**
 * @description: 中断程序初始化
 * 
 */
void interrupt_init()
{
    EA=1;   //总中断使能
}

void timer0_init()
{
    TMOD&=0xf0;
    TMOD|=0x01;
    TH0=0xFC;
    TL0=0X67; //1ms计时
    ET0=1;
    TF0=0;
    TR0=1;//启动T0
}

/**
 * @description: 定时器T0中断
 * 
 */
void interrupt_timer0()  interrupt 1
{
    TH0=0xFC;
    TL0=0X67; //1ms计时

    //定时器0中断函数
    last_operation=now_operation;
    now_operation=key_analyze(key_scan());
    
}

//定时器1初始化及中断
// void timer1_init()
// {
//     TMOD&=0x0f;
//     TMOD|=0x10;
//     TH1=0xFC;
//     TL1=0X67; //1ms计时
//     TF1=0;
//     TR1=1;//启动T1
// }

// /**
//  * @description: 定时器T1中断
//  * 
//  */
// void interrupt_timer1() interrupt 3
// {
//     TH1=0xFC;
//     TL1=0X67; //1ms计时

//     //定时器1中断函数

// }