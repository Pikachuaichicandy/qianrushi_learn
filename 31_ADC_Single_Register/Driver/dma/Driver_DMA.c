#include "Driver_DMA.h"

/**
 * @description: 初始化DMA1_Channel4
 */
void Driver_DMA1_Init(void)
{
    /* 1. 开启 DAM 时钟 */
    RCC->AHBENR |= RCC_AHBENR_DMA1EN;

    /* 2. DMA相关的配置 */
    /* 2.1 DMA方向: 从存储器(RAM)到串口(外设) . 通道4*/
    DMA1_Channel4->CCR |= DMA_CCR4_DIR; // 0: 从外设读 1:从存储器读

    /* 2.2 设置存储器和外设的数据宽度:  8位(一个字节) 16位(半字) 32位(字) */
    DMA1_Channel4->CCR &= ~DMA_CCR4_PSIZE; /* 00:8位 01:16位 10:32位 11:保留 */
    DMA1_Channel4->CCR &= ~DMA_CCR4_MSIZE;
    /* 2.3 设置外设和存储器的地址是否自增 */
    DMA1_Channel4->CCR &= ~DMA_CCR4_PINC; /* 串口外设地址不能自增 */
    DMA1_Channel4->CCR |= DMA_CCR4_MINC; /* 存储器地址自增 */
    /* 2.4 开启传输完成的中断 */
    DMA1_Channel4->CCR |= DMA_CCR4_TCIE;
    /* 2.5 使能串口的DMA传输 */
    USART1->CR3 |= USART_CR3_DMAT;
    /* 2.6 开启循环模式 */
    DMA1_Channel4->CCR |= DMA_CCR4_CIRC;

    /* 2.5 NVIC的设置 */
    NVIC_SetPriorityGrouping(3);
    NVIC_SetPriority(DMA1_Channel4_IRQn, 3);
    NVIC_EnableIRQ(DMA1_Channel4_IRQn);
}
/**
 * @description: 启动DMA传输数据
 * @param {uint32_t} srcAddr 源地址
 * @param {uint32_t} desAddr 目的地址
 * @param {uint16_t} dataLen 要传输的数据长度
 */
void Driver_DMA1_TransimitData(uint32_t srcAddr,
                               uint32_t desAddr,
                               uint16_t dataLen)
{
    /* 1. 设置外设地址 */
    DMA1_Channel4->CPAR = desAddr;
    /* 2. 设置存储器地址 */
    DMA1_Channel4->CMAR = srcAddr;

    /* 3. 设置要传输的数据量 */;
    DMA1_Channel4->CNDTR = dataLen;

    /* 4. 开启通道,开始传输 */
    DMA1_Channel4->CCR |= DMA_CCR4_EN;
}

uint8_t isTransmitFinished = 0;
void DMA1_Channel4_IRQHandler(void)
{
    if (DMA1->ISR & DMA_ISR_TCIF4)
    {
        /* 清除传输完成中断完成标志位 */
        DMA1->IFCR |= DMA_IFCR_CGIF4;

        /* 通道不需要了,可以关闭 */
        //DMA1_Channel4->CCR &= ~DMA_CCR4_EN;
        isTransmitFinished = 1;
    }
}