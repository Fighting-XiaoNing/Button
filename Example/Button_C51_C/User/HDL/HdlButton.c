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
#include <reg52.h>

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
            bitstatus = (P3 & 0x08) ? false : true;                             // P3.3�͵�ƽ��Ч
            break;
        case 1:
            bitstatus = (P3 & 0x04) ? false : true;                             // P3.2�͵�ƽ��Ч
            break;
        case 2:
            bitstatus = (P2 & 0x01) ? true : false;                             // P2.0�ߵ�ƽ��Ч
            break;
        default:
            break;
    }
    return bitstatus;
}
