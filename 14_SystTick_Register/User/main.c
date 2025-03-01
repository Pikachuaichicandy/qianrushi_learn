#include "Driver_USART.h"

#include "Delay.h"
#include "Driver_SysTick.h"
#include "Driver_LED.h"

int main()
{
    Driver_USART1_Init();
    printf("系统定时器实验开始...\r\n");
    Driver_LED_Init();
    Driver_SysTick_Init();

    while (1)
    {
       
    }
}
