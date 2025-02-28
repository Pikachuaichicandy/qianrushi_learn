
#include "Inf_W24C02.h"
#include "stdio.h"

void Inf_W24C02_Init(void)
{
    MX_I2C2_Init();
}

void Inf_W24C02_WriteByte(uint8_t innerAddr, uint8_t byte)
{
    HAL_I2C_Mem_Write(&hi2c2, ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, &byte, 1, 2000);

    HAL_Delay(5);
}

uint8_t Inf_W24C02_ReadByte(uint8_t innerAddr)
{
    uint8_t byte;
    HAL_I2C_Mem_Read(&hi2c2, ADDR + 1, innerAddr, I2C_MEMADD_SIZE_8BIT, &byte, 1, 2000);
    return byte;
}

/**
 * @description: 页写入.一次写入多个字节
 * @param {uint8_t} innerAddr
 * @param {uint8_t} *bytes
 * @param {uint8_t} len
 * @return {*}
 */
void Inf_W24C02_WriteBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
    HAL_I2C_Mem_Write(&hi2c2, ADDR, innerAddr, I2C_MEMADD_SIZE_8BIT, bytes, len, 2000);
    HAL_Delay(5);
}

/**
 * @description: 一次性读取多个字节的数据
 * @param {uint8_t} innerAddr 起始位置
 * @param {uint8_t} *bytes 存储读到的数据
 * @param {uint8_t} len 读取的字节数
 * @return {*}
 */
void Inf_W24C02_ReadBytes(uint8_t innerAddr, uint8_t *bytes, uint8_t len)
{
    HAL_I2C_Mem_Read(&hi2c2, ADDR + 1, innerAddr, I2C_MEMADD_SIZE_8BIT, bytes, len, 2000);
}
