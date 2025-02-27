#include "Driver_USART.h"
#include "Inf_W24C02.h"

int main()
{
    Driver_USART1_Init();
    printf("尚硅谷 I2C 硬件 实验开始....\r\n");

    Inf_W24C02_Init();

    Inf_W24C02_WriteByte(0x00, 'x');
    Inf_W24C02_WriteByte(0x01, 'y');
    Inf_W24C02_WriteByte(0x02, 'z');
    uint8_t byte1 = Inf_W24C02_ReadByte(0x00);
    uint8_t byte2 = Inf_W24C02_ReadByte(0x01);
    uint8_t byte3 = Inf_W24C02_ReadByte(0x02);

    printf("%c\r\n", byte1);
    printf("%c\r\n", byte2);
    printf("%c\r\n", byte3);

    Inf_W24C02_WriteBytes(0x00, "123456", 6);

    uint8_t buff[100] = {0};
    Inf_W24C02_ReadBytes(0x00, buff, 6);
    printf("%s\r\n", buff);

    // 清零缓冲区
    memset(buff, 0, sizeof(buff));
    Inf_W24C02_WriteBytes(0x00, "0123456789abcdefghijk", 21);
    Inf_W24C02_ReadBytes(0x00, buff, 21);
    printf("%s\r\n", buff);
    while (1)
    {
    }
}
