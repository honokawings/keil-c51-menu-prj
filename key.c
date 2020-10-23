/**
 * @file: key.c
 * @author: Honokawings (1915569481@qq.com)
 * @description: 旋转编码器驱动源代码
 * @version 0.1
 * @date: 2020-10-21 18:10:97
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "key.h"

//j局内宏变量
#define KEY_SCAN_MS         1                   //扫描周期
#define KEY_DESSHACKING     (20/KEY_SCAN_MS)    //消抖周期
#define KEY_LONGTIME        (600/KEY_SCAN_MS)   //长按判定
#define KEY_DCLICKTIME      (150/KEY_SCAN_MS)   //双击判定
#define KEY_LONG_REPEAT     (200/KEY_SCAN_MS)   //连续长按判定

//局内变量
static char key_a_last=0;//a相位上一次状态
static char key_b_last=0;//b相位上一次状态

static u16 key_o_cnt=0;//按键计数
static u16 key_o_dclick_cnt=0; //双击计数

static char key_o_sclick_flag=0;//短按标志
static char key_o_lclick_flag=0;//长按标志
static char key_o_dclick_flag=0;//双击标志

/**
 * @description: 编码器按键初始化
 * 
 */
void key_init()
{
    //输入写1
    key_a_now=1;
    key_b_now=1;
    key_o=1;

    //避免误判
    key_a_last=key_a_now;
    key_b_last=key_b_now;

    //清相关计数及标志
    key_o_cnt=0;
    key_o_dclick_cnt=0;
    key_o_dclick_flag=0;
    key_o_lclick_flag=0;
    key_o_sclick_flag=0;
}

/**
 * @description: 按键扫描函数
 * 
 * @return: char 0-无动作 
 *               1-仅正转 -1-仅反转
 *               2-仅按键
 *               3-按键正转 -3-按键反转
 */
char key_scan()
{
    char scan_result=0;

    if (key_a_now!=key_a_last)//旋转判定
    {
        if (key_a_now==0)//下降沿
        {
            if(key_b_now==1)
                scan_result=1;//正转
            else
                scan_result=-1;//反转
        }
        key_a_last=key_a_now;
        key_b_last=key_b_now;
    }

    if(key_o==0)
    {
        if(scan_result==0)
            scan_result=2;
        else
        {
            if(scan_result==1)
                scan_result=3;
            if(scan_result==-1)
                scan_result=-3;
        }
    } 
    return scan_result;
}

/**
 * @description: 按键情况分析程序
 * 
 * @param: scan_result_analyze 按键扫描结果
 * @return: char 0-静态
 *               1-单击 2-双击 3-长按 4-连续长按
 *               5-正转 -5-反转
 *               6-按键正转 -6-按键反转
 */
char key_analyze(char scan_result_analyze)
{
    char analyze_result=0;
    static unsigned int TMP_Value = 0;//连续长按间隔

    //旋转处理函数
    if(scan_result_analyze==1)
        analyze_result=5;
    if(scan_result_analyze==-1)
        analyze_result=-5;

    //按下旋转处理函数
    if(scan_result_analyze==3)
        analyze_result=6;
    if(scan_result_analyze==-3)
        analyze_result=-6;

    //按键状态处理程序
    if(scan_result_analyze==2)//扫描到按下按键
    {
        if(key_o_cnt<10000)//打开计数定时器
            key_o_cnt++; 

        if(key_o_cnt>=KEY_DESSHACKING)//短按判定
        {
            key_o_sclick_flag=1;
                       
            if(key_o_cnt==KEY_LONGTIME)//长按判定
            {
                key_o_sclick_flag=0;
                key_o_lclick_flag=1;
            }
        }

        //若已经判定在双击间隔中，再按下按键，则判定进入双击(状态1)
        if((key_o_dclick_cnt>0)&&(key_o_dclick_cnt<=KEY_DCLICKTIME))
            key_o_dclick_flag=1;
    }
    else if(scan_result_analyze==0)//松开按键
    {
        if(key_o_cnt<KEY_DESSHACKING)   //未到消抖时长即松开按键，清除所有标志
        {                               
            key_o_cnt=0;
            key_o_dclick_cnt=0;
            key_o_dclick_flag=0;
            key_o_lclick_flag=0;
            key_o_sclick_flag=0;
        }
        else
        {
            if(key_o_sclick_flag==1)//短按状态保持中
            {
                //双击判定计时
                if((key_o_dclick_flag==0)&&(key_o_dclick_cnt>=0))
                    key_o_dclick_cnt++;

                //双击判定在双击限定时间（KEY_DCLICKTIME）内二次释放(满足状态1)则双击成功(状态2)
                if((key_o_dclick_flag==1)&&(key_o_dclick_cnt<=KEY_DCLICKTIME))
                    key_o_dclick_flag=2;

                //双击判定在双击限定时间（KEY_DCLICKTIME）内没再次按下则判定双击失败(保持状态0)
                if((key_o_dclick_flag==0)&&(key_o_dclick_cnt>KEY_DCLICKTIME))
                    key_o_sclick_flag=0;//短按释放
            }
            if(key_o_lclick_flag==1)//长按释放
                key_o_lclick_flag=0;
        }
    }

    //按键高级处理程序
    if(key_o_cnt>KEY_DESSHACKING)//短按启动后
    {
        //单击未双击未长按
        if((key_o_sclick_flag==0)&&(key_o_dclick_cnt>KEY_DCLICKTIME)&&(key_o_lclick_flag<KEY_LONGTIME))
        {
            analyze_result=1;
            //清除标志
            key_o_cnt=0;
            key_o_dclick_cnt=0;
            key_o_dclick_flag=0;
        }
        //双击
        if((key_o_dclick_flag==2)&&(key_o_dclick_cnt>0)&&(key_o_dclick_cnt<=KEY_DCLICKTIME))
        {
            analyze_result=2;
            //清除标志
            key_o_dclick_cnt=0;
            key_o_cnt=0;
            key_o_sclick_flag=0;
            key_o_dclick_flag=0;
        }
        //长按
        if((key_o_lclick_flag==0)&&(key_o_cnt>=KEY_LONGTIME))
        {
            analyze_result=3;
            //清除标志
            key_o_cnt=0;
            key_o_dclick_cnt=0;
        }
        //连续长按
        if((key_o_lclick_flag==1)&&(key_o_cnt>=KEY_LONGTIME))
        {
            TMP_Value++;
            if(TMP_Value%KEY_LONG_REPEAT==0)
            {
                TMP_Value=0;
                analyze_result=4;
            }
        }
    }
    return analyze_result;
}
