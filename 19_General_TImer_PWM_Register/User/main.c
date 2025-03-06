#include "Driver_USART.h"

#include "Delay.h"
#include "Driver_SysTick.h"
#include "Driver_LED.h"
#include "Driver_TIM5.h"

int main()
{
    Driver_USART1_Init();
    printf("系统定时器实验开始...\r\n");

    //Driver_LED_Init();

    Driver_TIM5_Init();

    Driver_TIM5_Start();

    uint8_t dutyCycle = 0;
    uint8_t dir = 0; // 变大

    Driver_TIM5_SetDutyCycle(dutyCycle);

    while (1)
    {
        if (dir == 0)
        {
            dutyCycle += 1;
            if (dutyCycle >= 99)
            {
                dir = 1;
            }
        }
        else
        {
            dutyCycle -= 1;
            if (dutyCycle <= 1)
            {
                dir = 0;
            }
        }

        Driver_TIM5_SetDutyCycle(dutyCycle);
        Delay_ms(10);
    }
}
