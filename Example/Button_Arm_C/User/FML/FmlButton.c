/**
 * @file      FmlButton.c
 * @author    XiaoNing
 * @post      Embedded Software Engineer
 * @brief     按键
 * @version   1.0.0
 * @date      2023-06-28
 * @copyright Copyright (c) 2023
 */
#include "FmlButton.h"

enum ButtonFsmStatus
{
    BUTTON_PRESSDOWN_IDLE,
    BUTTON_RELEASE_LONGPRESS,
    BUTTON_PRESSDOWN_CLICK,
    BUTTON_HOLD_RELEASE
};

static ButtonField_TypeDef *gpButtonLinkList = NULL;                            // 按键链表头指针

static void FML_Button_Debounce(ButtonField_TypeDef *handle);
static void FML_Button_Handle(ButtonField_TypeDef *handle);

/**
 * @brief  按键初始化
 * @param  handle 按键句柄
 * @param  Init 初始化函数
 * @param  GetButtonStatus 获取按键状态函数（是否有效/触发）
 * @param  buttonID 按键ID
 * @retval None
 */
void FML_Button_Init(ButtonField_TypeDef *handle,
                     void (*Init)(void),
                     bool (*GetButtonStatus)(uint8_t buttonID),
                     uint8_t buttonID)
{
    ButtonField_TypeDef *p = NULL;

    handle->buttonID = buttonID;                                                // 按键ID
    handle->event = EVENT_NONE_BTN;                                             // 无事件
    handle->buttonTrigger = false;                                              // 无触发
    handle->buttonFsmVar = BUTTON_PRESSDOWN_IDLE;                               // 初始化状态机为空闲态
    handle->GetButtonStatus = GetButtonStatus;                                  // 注册按键状态获取函数
    if (Init != NULL)
    {
        Init();                                                                 // 初始化按键
    }

    if (gpButtonLinkList == NULL)                                               // 为空链表
    {
        gpButtonLinkList = handle;                                              // 头指针指向首元结点
        gpButtonLinkList->next = NULL;                                          // 首元节点标记为尾结点
    }
    else
    {
        p = gpButtonLinkList;                                                   // 指向按键链表
        while (p->next)                                                         // 查找尾结点（判断结点指针域是否为空）
        {
            p = p->next;                                                        // 指向下一个结点
        }
        p->next = handle;                                                       // 链接新结点
        handle->next = NULL;                                                    // 新节点标记为尾结点
    }
}

/**
 * @brief  按键注册事件回调函数
 * @param  handle 按键句柄
 * @param  event 事件类型
 * @param  callback 回调函数
 * @retval None
 */
void FML_Button_RegEventCallBack(ButtonField_TypeDef *handle,
                                 ButtonEvent_ENUM event,
                                 void (*callback)(void))
{
    handle->callBackArray[event] = callback;                                    // 将回调函数存入函数指针数组
}

/**
 * @brief  获取按键事件
 * @param  handle 按键句柄
 * @retval 事件 - 见ButtonEvent_ENUM枚举体
 */
ButtonEvent_ENUM FML_Button_GetEvent(ButtonField_TypeDef *handle)
{
    return (ButtonEvent_ENUM)(handle->event);                                   // 事件
}

/**
 * @brief  按键消抖
 * @param  handle 按键句柄
 * @retval None
 */
static void FML_Button_Debounce(ButtonField_TypeDef *handle)
{
    bool curButtonLevel = false;

    if (handle->GetButtonStatus != NULL)                                        // 已注册按键电平读取函数
    {
        curButtonLevel = handle->GetButtonStatus(handle->buttonID);             // 读取当前按键电平
    }
    if (curButtonLevel != handle->buttonTrigger)                                // 与滤波后的稳定电平不一致（电平跳变）
    {
        if (++handle->debounceCnt >= DEBOUNCE_TIME)                             // 消抖计数，并判断消抖是否完成
        {
            handle->buttonTrigger = curButtonLevel;                             // 保存消抖后的电平
            handle->debounceCnt = 0;                                            // 清零消抖计数值
        }
    }
    else                                                                        // 与滤波后的稳定电平一致（电平无变化）
    {
        handle->debounceCnt = 0;
    }
}

/**
 * @brief  按键处理
 * @param  handle 按键句柄
 * @retval None
 * @note   消抖并没有处理完再进行处理，而是边消抖边处理，会导致处理过程中多计数
 */
static void FML_Button_Handle(ButtonField_TypeDef *handle)
{
    FML_Button_Debounce(handle);                                                // 按键消抖
    if ((handle->buttonFsmVar != BUTTON_PRESSDOWN_IDLE)                         // 按键状态非空闲态
     && (handle->ticks < UINT16_MAX))
    {
        handle->ticks++;                                                        // 滴答节拍值
    }

    switch (handle->buttonFsmVar)
    {
        case BUTTON_PRESSDOWN_IDLE://///////////////////////////////////////////// 处理：空闲or按下
            if (handle->buttonTrigger == true)                                  // 按下按键
            {
                handle->event = EVENT_PRESS_DOWN;                               // 按下事件
                if (handle->callBackArray[EVENT_PRESS_DOWN])                    // 已注册事件回调函数
                {
                    handle->callBackArray[EVENT_PRESS_DOWN]();                  // 调用事件回调函数
                }
                if (handle->clickCnt < UINT8_MAX)
                {
                    handle->clickCnt++;                                         // 点击计数
                }
                handle->ticks = 0;
                handle->buttonFsmVar = BUTTON_RELEASE_LONGPRESS;
            }
            else
            {
                handle->event = EVENT_NONE_BTN;                                 // 无按键事件
            }
            break;
        case BUTTON_RELEASE_LONGPRESS://////////////////////////////////////////// 处理：释放or长按
            if (handle->buttonTrigger == false)                                 // 松开按键
            {
                handle->event = EVENT_RELEASE;                                  // 释放事件
                if (handle->callBackArray[EVENT_RELEASE])                       // 已注册事件回调函数
                {
                    handle->callBackArray[EVENT_RELEASE]();                     // 调用事件回调函数
                }
                handle->ticks = 0;
                handle->buttonFsmVar = BUTTON_PRESSDOWN_CLICK;
            }
            else if (handle->ticks >= LONG_PRESS_TIME)                          // 达到长按时间
            {
                handle->event = EVENT_LONG_PRESS;                               // 长按事件
                if (handle->callBackArray[EVENT_LONG_PRESS])                    // 已注册事件回调函数
                {
                    handle->callBackArray[EVENT_LONG_PRESS]();                  // 调用事件回调函数
                }
                handle->clickCnt = 0;
                handle->ticks = 0;
                handle->buttonFsmVar = BUTTON_HOLD_RELEASE;
            }
            break;
        case BUTTON_PRESSDOWN_CLICK:////////////////////////////////////////////// 处理：按下or点击
            if (handle->buttonTrigger == true)                                  // 按键按下
            {
                handle->event = EVENT_PRESS_DOWN;                               // 按下事件
                if (handle->callBackArray[EVENT_PRESS_DOWN])                    // 已注册事件回调函数
                {
                    handle->callBackArray[EVENT_PRESS_DOWN]();                  // 调用事件回调函数
                }
                if (handle->clickCnt < UINT8_MAX)
                {
                    handle->clickCnt++;                                         // 点击计数
                }
                handle->ticks = 0;
                handle->buttonFsmVar = BUTTON_RELEASE_LONGPRESS;
            }
            else if (handle->ticks >= CLICK_TIME)                               // 松手达到点击时间
            {
                handle->event = EVENT_CLICK;                                    // 点击事件
                if (handle->callBackArray[EVENT_CLICK])                         // 已注册事件回调函数
                {
                    handle->callBackArray[EVENT_CLICK]();                       // 调用事件回调函数
                }
                handle->clickCnt = 0;
                handle->ticks = 0;
                handle->buttonFsmVar = BUTTON_PRESSDOWN_IDLE;
            }
            break;
        case BUTTON_HOLD_RELEASE:///////////////////////////////////////////////// 处理：按住or松开
            if (handle->buttonTrigger == false)                                 // 松开按键
            {
                handle->event = EVENT_RELEASE;                                  // 释放事件
                if (handle->callBackArray[EVENT_RELEASE])                       // 已注册事件回调函数
                {
                    handle->callBackArray[EVENT_RELEASE]();                     // 调用事件回调函数
                }
                handle->clickCnt = 0;
                handle->ticks = 0;
                handle->buttonFsmVar = BUTTON_PRESSDOWN_IDLE;
            }
            else if (handle->ticks >= PRESS_HOLD_TIME)                          // 按住达到触发时间
            {
                handle->event = EVENT_PRESS_HOLD;                               // 按住事件
                if (handle->callBackArray[EVENT_PRESS_HOLD])                    // 已注册事件回调函数
                {
                    handle->callBackArray[EVENT_PRESS_HOLD]();                  // 调用事件回调函数
                }
                handle->ticks = 0;
            }
            break;
        default:
            break;
    }
}

/**
 * @brief  按键扫描
 * @param  None
 * @retval None
 */
void FML_Button_Scan(void)
{
    ButtonField_TypeDef *curButton = NULL;

    for (curButton = gpButtonLinkList; curButton; curButton = curButton->next)
    {
        FML_Button_Handle(curButton);                                           // 处理当前按键
    }
}
