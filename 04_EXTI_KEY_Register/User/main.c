#include "Driver_LED.h"
#include "Delay.h"
#include "Driver_Key.h"

int main()
{
    
    /* 1. 初始化LED */
    Driver_LED_Init();

    /* 2. 初始化按键 */
    Driver_Key_Init();

    while (1)
    {
       
    }
}
