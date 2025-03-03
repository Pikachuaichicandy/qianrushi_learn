#include "Driver_USART.h"

#include "Delay.h"
#include "Driver_SysTick.h"
#include "Driver_LED.h"
#include "Driver_TIM6.h"

int main()
{
    Driver_USART1_Init();
    printf("系统定时器实验开始...\r\n");
    Driver_LED_Init();
    
    Driver_TIM6_Init();

    while (1)
    {
       
    }
}
