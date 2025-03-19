#include "Driver_USART.h"

#include "Delay.h"
#include "Driver_ADC.h"

int main()
{
    Driver_USART1_Init();
    printf("ADC转换实验: 单通道\r\n");
    Driver_ADC1_Init();
    Driver_ADC1_StartConvert();

    while (1)
    {
        double v = Driver_ADC1_ReadV();
        printf("v = %.2f\r\n", v);
        Delay_s(1);
    }
}
