#include "Driver_USART.h"

#include "Driver_TIM1.h"

int main()
{
    Driver_USART1_Init();
    printf("高级定时器实验开始...\r\n");

    Driver_TIM1_Init();
    Driver_TIM1_Start();
    while (1)
    {  

    }
}
