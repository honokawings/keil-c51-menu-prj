/**
 * @file: main.c
 * @author: Honokawings (1915569481@qq.com)
 * @description: 主程序，主要进行菜单的高级操作
 * @version 0.1
 * @date: 2020-10-23 16:10:43
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "sys.h"

extern MenuIndex menu_gather[8];
char last_operation;
char now_operation;
char function_index=0;
void (*current_operation_index)();

void main()
{
    int i=0;
    char buff_index;
    last_operation=0;
    now_operation=0;

    lcd_init(0);
    // config_uart(9600);
    key_init();
    interrupt_init();
    timer0_init();//相关模块初始化

    menu0();
    while (1)
    {
        if((now_operation!=last_operation)&&
        ((last_operation==0)||(last_operation==4)))//按键检测变化且刚开始变化,注意滤除连续长按回报
        {
            switch (now_operation)
            {
                case 1://短按
                {
                    function_index=menu_gather[function_index].move_enter;
                    // uart_send(&function_index);//debug
                }
                    break;
                case 3://长按
                {
                    function_index=0;
                    // uart_send(&function_index);//debug
                }
                    break;
                case 2://双击
                {
                    buff_index=function_index;//交互体验，返回后再进入会记忆之前的选项
                    function_index=menu_gather[function_index].move_esc;
                    //防止首页时按esc导致自锁
                    if(buff_index!=0)
                        menu_gather[function_index].move_enter=buff_index;
                    // uart_send(&function_index);//debug
                }
                    break;                    
                case 5://正转
                {
                    function_index=menu_gather[function_index].move_up;
                    // uart_send(&function_index);//debug
                }
                    break;
                case -5://反转
                {
                    function_index=menu_gather[function_index].move_down;
                    // uart_send(&function_index);//debug
                }
                    break;                         
                default:
                    break;
            } 
        current_operation_index=menu_gather[function_index].function;
        (*current_operation_index)();                     
        }
    }
}