#include "Driver_TIM4.h"
#include "stdio.h"

void Driver_TIM4_Init(void)
{
    /* 1. 开启时钟 */
    /* 1.1 TIM4的时钟 */
    RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
    /* 1.2 把引脚(PB6) 对应的端口开启时钟*/
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    /* 2. PB6设置为输入: 浮空输入 CNF=01 MODE=00 */
    GPIOB->CRL &= ~(GPIO_CRL_CNF6_1 | GPIO_CRL_MODE6);
    GPIOB->CRL |= GPIO_CRL_CNF6_0;

    /* 3. 配置定时器的时基部分 */
    /* 3.1 预分频系数配置 分频后计数器的时钟时1MHz 周期:1us*/
    TIM4->PSC = 72 - 1;
    /* 3.2 自动重装载寄存器的值: 设置最大,尽量避免溢出 */
    TIM4->ARR = 65535;
    /* 3.3 计数方向: 向上计数 */
    TIM4->CR1 &= ~TIM_CR1_DIR;

    /* 4. 输入捕获部分 */
    /* 4.1 输入信号不滤波 */
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;
    /* 4.2 配置边缘检测: 去IC1上升沿 去IC2的下降沿 */
    TIM4->CCER &= ~TIM_CCER_CC1P;
    TIM4->CCER |= TIM_CCER_CC2P;
    /* 4.3 把 IC1映射到TI1 CCMR1_CC1S=01 上升沿*/
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
    /* 4.4 把 IC2映射到TI1 CCMR1_CC2S=10 下降沿*/
    TIM4->CCMR1 |= TIM_CCMR1_CC2S_1;
    TIM4->CCMR1 &= ~TIM_CCMR1_CC2S_0;
    /* 4.5 IC1和IC2的预分频 都不分频 */
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;
    TIM4->CCMR1 &= ~TIM_CCMR1_IC2PSC;

    /* 4.6 配置TRGI信号:  TI1FP1   SMCR_TS=101*/
    TIM4->SMCR |= (TIM_SMCR_TS_2 | TIM_SMCR_TS_0);
    TIM4->SMCR &= ~TIM_SMCR_TS_1;

    /* 4.7 配置从模式: 为复位模式 SMCR_SMS=100*/
    TIM4->SMCR |= TIM_SMCR_SMS_2;
    TIM4->SMCR &= ~(TIM_SMCR_SMS_1 | TIM_SMCR_SMS_0);

    /* 4.8 使能通道1捕获和通道2捕获 */
    TIM4->CCER |= TIM_CCER_CC1E;
    TIM4->CCER |= TIM_CCER_CC2E;
}

void Driver_TIM4_Start(void)
{
    TIM4->CR1 |= TIM_CR1_CEN; /* 计数器使能 */
}

void Driver_TIM4_Stop(void)
{
    TIM4->CR1 &= ~TIM_CR1_CEN; /* 计数器失能 */
}


/* 返回PWM的周期 ms*/
double Driver_TIM4_GetPWMCycle(void)
{
    return TIM4->CCR1 / 1000.0;
}

/* 返回PWM的频率 */
double Driver_TIM4_GetPWMFreq(void)
{
    return 1000000.0 / TIM4->CCR1;
}

// 返回占空比
double Driver_TIM4_GetDutyCycle(void)
{
    return TIM4->CCR2 * 1.0 / TIM4->CCR1;
}