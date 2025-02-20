#include "Driver_USART.h"


/* 缓冲接收到的数据 */
extern uint8_t buff[100];
/* 存储接收到的字节的长度 */
extern uint8_t len;
extern uint8_t isToSend;

int main()
{
    Driver_USART1_Init();
    Driver_USART1_SendString("abc", 3);
    while (1)
    {
        if(isToSend){
            Driver_USART1_SendString(buff, len);
            isToSend = 0;
            len = 0;
        }
    }
}
