/**
 * @file      HdlButton.c
 * @author    XiaoNing
 * @post      Embedded Software Engineer
 * @brief     ����
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
 * @brief  ������ʼ��
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
 * @brief  ��������ʼ��
 * @param  None
 * @retval None
 */
static void HDL_Button_DeInit(void)
{}

/**
 * @brief  ��������͹���
 * @param  None
 * @retval None
 */
static void HDL_Button_EnterLowPower(void)
{}

/**
 * @brief  �����˳��͹���
 * @param  None
 * @retval None
 */
static void HDL_Button_ExitLowPower(void)
{}

/**
 * @brief  ������ȡ��ƽ
 * @param  buttonID ������
 * @retval true - ��Ч��ƽ
 * @retval false - ��Ч��ƽ
 */
static bool HDL_Button_ReadStatus(uint8_t buttonID)
{
    bool bitstatus = false;

    switch (buttonID)
    {
        case 0:
            bitstatus = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4) ? false : true;     // PE4�͵�ƽ��Ч
            break;
        case 1:
            bitstatus = HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3) ? false : true;     // PE3�͵�ƽ��Ч
            break;
        case 2:
            bitstatus = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) ? true : false;     // PA0�ߵ�ƽ��Ч
            break;
        default:
            break;
    }
    return bitstatus;
}
