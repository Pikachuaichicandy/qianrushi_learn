#include "Driver_USART.h"
#include "Delay.h"
#include "string.h"

uint8_t buff[100] = {0};
uint8_t len = 0;
int main()
{
    Driver_USART1_Init();
    // Driver_USART1_SendChar('a');
    while (1)
    {
        // uint8_t *str = "Hello atguigu!\r\n";
        // Driver_USART1_SendString(str, strlen((char *)str));

        /* uint8_t *str = "ÉÐ¹è¹È\r\n";
        Driver_USART1_SendString(str, strlen((char *)str));
        Delay_s(1); */

        // uint8_t c =  Driver_USART1_ReceiveChar();
        // Driver_USART1_SendChar(c);

        Driver_USART1_ReceiveString(buff, &len);
        Driver_USART1_SendString(buff, len);
    }

}
