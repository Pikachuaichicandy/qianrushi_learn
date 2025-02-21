#include "Driver_USART.h"

#include "Delay.h"

int main()
{
    Driver_USART1_Init();

    while (1)
    {
        printf("hello atguigu\r\n");
        Delay_ms(500);
    }
}
