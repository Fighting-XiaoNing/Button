/**
 * @file      HdlButton.c
 * @author    XiaoNing
 * @post      Embedded Software Engineer
 * @brief     按键
 * @version   1.0.0
 * @date      2023-05-29
 * @copyright Copyright (c) 2023
 */
#include "HdlButton.h"
#include <reg52.h>

/**
 * @brief  按键初始化
 * @param  None
 * @retval None
 */
void HDL_Button_Init(void)
{
}

/**
 * @brief  按键反初始化
 * @param  None
 * @retval None
 */
void HDL_Button_DeInit(void)
{}

/**
 * @brief  按键进入低功耗
 * @param  None
 * @retval None
 */
void HDL_Button_EnterLowPower(void)
{}

/**
 * @brief  按键退出低功耗
 * @param  None
 * @retval None
 */
void HDL_Button_ExitLowPower(void)
{}

/**
 * @brief  按键读取电平
 * @param  buttonID 按键号
 * @retval true - 有效电平
 * @retval false - 无效电平
 */
bool HDL_Button_ReadStatus(uint8_t buttonID)
{
    bool bitstatus = false;

    switch (buttonID)
    {
        case 0:
            bitstatus = (P3 & 0x08) ? false : true;                             // P3.3低电平有效
            break;
        case 1:
            bitstatus = (P3 & 0x04) ? false : true;                             // P3.2低电平有效
            break;
        case 2:
            bitstatus = (P2 & 0x01) ? true : false;                             // P2.0高电平有效
            break;
        default:
            break;
    }
    return bitstatus;
}
