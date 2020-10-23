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

extern MenuIndex code menu_gather[5];
char last_operation;
char now_operation;
char function_index=0;

void main()
{
    last_operation=0;
    now_operation=0;

    lcd_init(0);
    key_init();
    interrupt_init();
    timer0_init();//相关模块初始化
    //config_uart(9600);


    while (1)
    {
        if((now_operation!=last_operation)&&
        (last_operation==0))//按键检测变化且刚开始变化
        {
            switch (now_operation)
            {
                case 1://短按
                {
                    function_index=menu_gather[function_index].move_enter;
                }
                    break;
                case 3://长按
                {
                    function_index=menu_gather[function_index].move_esc;
                }
                    break;
                case 2://双击
                {
                
                }
                    break;                    
                case 5://正转
                {
                    function_index=menu_gather[function_index].move_up;
                }
                    break;
                case -5://反转
                {
                    function_index=menu_gather[function_index].move_down;
                }
                    break;                         
                default:
                    break;
            }           
        }
        menu_gather[function_index].function();
    }
}