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
    /* 4.1 TI1的选择: 把CH1直接连接到TI1 */
    TIM4->CR2 &= ~TIM_CR2_TI1S;
    /* 4.2 信号的输入滤波器: 不滤波  CCMR1_IC1F=0000 */
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1F;
    /* 4.3 配置边缘检测器: 上升沿 CCER_CC1P=0  1下降沿捕获*/
    TIM4->CCER &= ~TIM_CCER_CC1P;
    /* 4.4 通道1配置为输入,并把信号映射到IC1  CC1S=01*/
    TIM4->CCMR1 &= ~TIM_CCMR1_CC1S_1;
    TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
    /* 4.5 对信号做分频(信号频率比较高的时候) CCMR1_IC1PSC=00不分频 01=2分频 10=4分频 11=8分频*/
    TIM4->CCMR1 &= ~TIM_CCMR1_IC1PSC;
    /* 4.6 通道1输入捕获使能 CCER_CC1E=1 */
    TIM4->CCER |= TIM_CCER_CC1E;
    /* 4.7 开启捕获中断 DIER_CC1IE=1 */
    TIM4->DIER |= TIM_DIER_CC1IE;

    /* 5. NVIC的配置 */
    /* 5.1 优先级组 */
    NVIC_SetPriorityGrouping(3);
    /* 5.2 优先级 */
    NVIC_SetPriority(TIM4_IRQn, 3);
    /* 5.3 使能中断 */
    NVIC_EnableIRQ(TIM4_IRQn);
}

void Driver_TIM4_Start(void)
{
    TIM4->CR1 |= TIM_CR1_CEN; /* 计数器使能 */
}

void Driver_TIM4_Stop(void)
{
    TIM4->CR1 &= ~TIM_CR1_CEN; /* 计数器失能 */
}

/* 记录上升沿的个数 */
uint8_t raiseEdgeCount = 0;
/* 存储信号的周期 */
uint16_t t = 0;

/* TIM4中断服务函数 */
void TIM4_IRQHandler(void)
{
  
    /* 判断是否是TIM4的通道1发生了捕获中断 */
    if (TIM4->SR & TIM_SR_CC1IF)
    {
        /* 中断标记位清除 */
        TIM4->SR &= ~TIM_SR_CC1IF;

        /* 上升沿个数 */
        raiseEdgeCount++;
        /* 如果是第1个上升沿,则清零计数器,让计数器从0开始计数 */
        if (raiseEdgeCount == 1)
        {
            TIM4->CNT = 0; /* 计数器清零 */
        }
        else if (raiseEdgeCount == 2)
        {
            /* 读取捕获寄存器的值,就是周期 单位是us*/
            t = TIM4->CCR1;
            /* 上升沿的计数从0重新计数 */
            raiseEdgeCount = 0;
        }
    }
}

/* 返回PWM的周期 ms*/
double Driver_TIM4_GetPWMCycle(void)
{
    return t / 1000.0;
}

/* 返回PWM的频率 */
double Driver_TIM4_GetPWMFreq(void)
{
    return 1000000 / t;
}
