#include "Driver_USART.h"

#include "Delay.h"
#include "Driver_SysTick.h"

#include "Driver_TIM5.h"
#include "Driver_TIM4.h"
int main()
{
    Driver_USART1_Init();
    printf("系统定时器实验开始...\r\n");

    Driver_TIM5_Init();
    Driver_TIM5_Start();

    Driver_TIM4_Init();
    Driver_TIM4_Start();
    double t, f;
    while (1)
    {
        t = Driver_TIM4_GetPWMCycle();
        f = Driver_TIM4_GetPWMFreq();

        printf("t=%.4fms, f=%.4fHz\r\n", t, f);

        Delay_ms(1000);
    }
}
