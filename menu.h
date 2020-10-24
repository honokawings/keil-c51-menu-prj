#ifndef __MENU_H
#define	__MENU_H

#include "sys.h"

void menu0();//主菜单
void menu1();
void menu2();
void menu3();
void menu4();
void menu5();
void menu6();
void menu7();

//定义菜单引索结构体
typedef struct menu_index
{
    u8 index;
    u8 move_up;
    u8 move_down;
    u8 move_enter;
    u8 move_esc;

    void (*function)();
} MenuIndex;


//注释部分为待拓展结构，慎删除
//C51构建树形结构不好处理内存分配问题
//
// typedef struct _menu
// {
//     //树形结构
//     struct _menu *father;
//     struct _menu *same_up;
//     struct _menu *same_down;
//     struct _menu *son;

//     //功能号 功能名
//     u8 *menu_name;
//     u8 id;

//     //功能函数
//     u8 function_id;
//     void (*function)();
// } menu_t;

// void menu_make(menu_t *head);

#endif