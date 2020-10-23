/**
 * @file: lcd.c
 * @author: Honokawings (1915569481@qq.com)
 * @description: lcd驱动源代码
 * @version 0.1
 * @date: 2020-10-20 23:10:54
 * 
 * @copyright Copyright (c) 2020
 * 
 */
#include "lcd.h"

//私有声明
static void check();
static void write_cmd(u8 cmd);
static void write_data(u8 dat);
static void select_cs(u8 n);
static void set_page(u8 page);
static void set_column(u8 column);
static void set_line(u8 line);
static void set_on(u8 flag);

/**
 * @description: 检测忙
 * 
 */
void check()
{
    RS=0;
    RW=1;
    data_pin=0x00;
    E=1;
    while (busy);
    E=0;
}

/**
 * @description: 写命令
 * 
 * @param: cmd 
 */
void write_cmd(u8 cmd)
{
    //check();
    E=0;
    RS=0;
    RW=0;
    data_pin=cmd;
    E=1;
    E=0;
}

/**
 * @description: 写数据
 * 
 * @param: dat 
 */
void write_data(u8 dat)
{
    //check();
    E=0;
    RS=1;
    RW=0;
    data_pin=dat;
    E=1;
    E=0;
}

/**
 * @description: 选择屏
 * 
 * @param: n 0-全选 1-左屏 2-右屏
 */
void select_cs(u8 n)
{
    switch (n)
    {
    case 0: CS1=1; CS2=1; break; //高电平选中
    case 1: CS1=1; CS2=0; break; //cs1左屏
    case 2: CS1=0; CS2=1; break; //cs2右屏
    }
}

/**
 * @description: 设置页
 * 
 * @param: page 0-7
 */
void set_page(u8 page)
{
    page=page&0x07; //0000 0111
    page=page|0xb8; //1011 1000
    write_cmd(page);//1011 1xxx
}

/**
 * @description: 设置列
 * 
 * @param: column 0-63
 */
void set_column(u8 column)
{
    column=column&0x3f; //0011 1111
    column=column|0x40; //0100 0000
    write_cmd(column);  //01xx xxxx
}

/**
 * @description: 起始行设置
 *               对应液晶屏最上一行的显示RAM的行号
 *                  
 * @param: line 0-63
 */
void set_line(u8 line)
{
    line=line&0x3f; //0011 1111
    line=line|0xc0; //1100 0000
    write_cmd(line);//11xx xxxx
}

/**
 * @description: 屏幕开关
 * 
 * @param: flag 1-开启 0关闭
 */
void set_on(u8 flag)
{
    flag=flag|0x3e;
    write_cmd(flag);    //0011 111x
}

/**
 * @description: 清屏
 * 
 * @param: flag 0--全屏，1--左屏，2--右屏
 */
void clear(u8 flag)
{
    u8 i,j;
    select_cs(flag);//置为0时左右屏同时清空
    for ( i = 0; i < 8; i++)
    {
        set_page(i);
        set_column(0);
        for ( j = 0; j < 64; j++)//自动指向下一列
        {
            write_data(0x00);//写0清空
        }   
    }    
}

/**
 * @description: 初始化lcd
 * 
 * @param: i 起始列
 */
void lcd_init(u8 i)
{
    //check();
    set_on(1);
    clear(0);
    set_line(i);
}


/**
 * @description: 16*16汉字显示
 * 
 * @param: page 选择页
 * @param: column 选择列
 * @param: screen 选择片 1-左屏 2-右屏
 * @param: method 1-反白 0-正常
 * @param: str 显示的字符
 */
void show16(u8 page,u8 column,u8 screen,u8 method,u8 *str)
{
    u8 i,j;
    j=0;
    select_cs(screen);

    set_page(page);//上半部分（页）
    set_column(column);
    for ( i = 0; i < 16; i++)
    {
        if(method) write_data(~str[j++]);//反白显示
        else    write_data(str[j++]);
    }

    set_page(page+1);//下半部分（页）
    set_column(column);
    for ( i = 0; i < 16; i++)
    {
        if(method) write_data(~str[j++]);//反白显示
        else    write_data(str[j++]);
    }
}

/**
 * @description: 8*16字符显示
 * 
 * @param: page 选择页
 * @param: column 选择列
 * @param: screen 选择片 1-左屏 2-右屏
 * @param: method 1-反白 0-正常
 * @param: str 显示的字符
 */
void show8(u8 page,u8 column,u8 screen,u8 method,u8 *str)
{
    u8 i,j;
    j=0;
    select_cs(screen);

    set_page(page);
    set_column(column);
    for ( i = 0; i < 8; i++)//上半部分
    {
        if(method) write_data(~str[j++]);//反白显示
        else    write_data(str[j++]);
    }
    set_page(page+1);
    set_column(column);
    for ( i = 0; i < 8; i++)//下半部分
    {
        if(method) write_data(~str[j++]);//反白显示
        else    write_data(str[j++]);
    }
}

/**
 * @description: 显示一行文字
 * 
 * @param: line 1-4文字所在行数
 * @param: str  
 * @param: word_cnt 文字数（16*16）
 * @param: method 1-反白 0-正常
 */
void show_sentence(u8 line,u8 *str,u8 word_cnt,u8 method)
{
    u8 i,j,k;
    if(word_cnt<=4)//左半页显示
    {
        for (i = 0; i < word_cnt; i++)
        {
            show16((line-1)*2,16*i,1,method,str+32*i);
        }
    }
    else//双页显示
    {
        for (j = 0; j < 4; j++)
        {
            show16((line-1)*2,16*j,1,method,str+32*j);
        }
        for (k = 0; k < (word_cnt-4); k++)
        {
            show16((line-1)*2,16*k,2,method,str+32*(k+4));
        }        
    }
}

/**
 * @description: 按坐标绘制点
 * 
 * @param: draw_x 0-127 x轴
 * @param: draw_y 0-63 y轴
 */
void draw_dot(u8 draw_x,u8 draw_y)
{
    if(draw_x<=63)
    {
        select_cs(1);
        set_page(draw_y/8);
        set_column(draw_x);
        write_data(2^(draw_y%8));
    }
    else if ((draw_x>=64)&&(draw_x<128))
    {
        select_cs(2);
        set_page(draw_y/8);
        set_column(draw_x-64);
        write_data(2^(draw_y%8));
    }
}
