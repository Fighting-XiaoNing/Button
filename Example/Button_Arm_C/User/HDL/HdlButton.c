/**
 * @file      HdlButton.c
 * @author    XiaoNing
 * @post      Embedded Software Engineer
 * @brief     按键
 * @version   1.0.0
 * @date      2023-03-21
 * @copyright Copyright (c) 2023
 */
#include "HdlButton.h"
#include "stm32f1xx_hal.h"

static void HDL_Button_Init(void);
static void HDL_Button_DeInit(void);
static void HDL_Button_EnterLowPower(void);
static void HDL_Button_ExitLowPower(void);
static bool HDL_Button_ReadStatus(uint8_t buttonID);

const HdlButton_TypeDef hdlButton = {
    HDL_Button_Init,
    HDL_Button_DeInit,
    HDL_Button_EnterLowPower,
    HDL_Button_ExitLowPower,
    HDL_Button_ReadStatus
};

/**
 * @brief  按键初始化
 * @param  None
 * @retval None
 */
static void HDL_Button_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    /*Configure GPIO pins : PE3 PE4 */
    GPIO_InitStruct.Pin = GPIO_PIN_3 | GPIO_PIN_4;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /*Configure GPIO pin : PA0 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief  按键反初始化
 * @param  None
 * @retval None
 */
static void HDL_Button_DeInit(void)
{}

/**
 * @brief  按键进入低功耗
 * @param  None
 * @retval None
 */
static void HDL_Button_EnterLowPower(void)
{}

/**
 * @brief  按键退出低功耗
 * @param  None
 * @retval None
 */
static void HDL_Button_ExitLowPower(void)
{}

/**
 * @brief  按键读取电平
 * @param  buttonID 按键号
 * @retval true - 有效电平
 * @retval false - 无效电平
 */
static bool HDL_Button_ReadStatus(uint8_t buttonID)
{
    bool bitstatus = false;

    switch (buttonID)
    {
        case 0:
            bitstatus = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) ? false : true;     // PE4低电平有效
            break;
        case 1:
            bitstatus = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) ? false : true;     // PE3低电平有效
            break;
        case 2:
            bitstatus = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) ? true : false;     // PA0高电平有效
            break;
        default:
            break;
    }
    return bitstatus;
}
