#ifndef CONFIG_H
#define CONFIG_H

#define BWidth 30     //每一个格子宽度

#define B_W_Count 36   //水平方向的格子数
#define B_H_Count 22

#define MWidth BWidth*B_W_Count     //贪吃蛇移动区域宽度width
#define MHeight BWidth*B_H_Count

//苹果，香蕉，毒药数量
#define appleCount 2
#define bananaCount 2
#define drugCount 5
#define totalNum appleCount+bananaCount+drugCount

#define timerNum 150   //计时器速率

//商城中使用
#define columnNum 3    //蛇头，蛇身，背景

#endif // CONFIG_H
