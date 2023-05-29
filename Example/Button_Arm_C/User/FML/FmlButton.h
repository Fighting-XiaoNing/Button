/**
 * @file      FmlButton.h
 * @author    XiaoNing
 * @post      Embedded Software Engineer
 * @brief     按键
 * @version   1.0.0
 * @date      2023-05-29
 * @copyright Copyright (c) 2023
 */
#ifndef __FML_BUTTON_H
#define __FML_BUTTON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SCAN_TIME 5                                                             // 扫描时间（ms）
#define DEBOUNCE_TIME (15 / SCAN_TIME)                                          // 消抖时间
#define CLICK_TIME (300 /  SCAN_TIME)                                           // 点击间隔时间
#define LONG_PRESS_TIME (1000 / SCAN_TIME)                                      // 长按所需时间

typedef enum ButtonEvent
{
    EVENT_PRESS_DOWN,                                                           // 按下
    EVENT_RELEASE,                                                              // 释放
    EVENT_CLICK,                                                                // 点击
    EVENT_LONG_PRESS,                                                           // 长按
    EVENT_PRESS_HOLD,                                                           // 按住
    EVENT_END,
    EVENT_NONE_BTN                                                              // 无事件
} ButtonEvent_ENUM;

typedef struct ButtonField
{
    uint16_t ticks;                                                             // 计时
    uint8_t buttonID;                                                           // 按键ID            //位域4[0, 15]
    uint8_t debounceCnt;                                                        // 消抖计数          //位域4[0, 15]
    uint8_t clickCnt;                                                           // 点击次数          //位域3[0, 7]
    uint8_t event;                                                              // 事件              //位域3[0, 7]
    uint8_t buttonFsmVar;                                                       // 状态机变量        //位域2[0, 3]
    bool buttonTrigger;                                                         // 消抖后按键触发状态 //位域1[0, 1]
    bool (*GetButtonStatus)(uint8_t buttonID);                                  // 获取按键状态函数
    void (*callBackArray[EVENT_END])(void);                                     // 存放事件回调函数
    struct ButtonField *next;                                                   // 链表指针域
} ButtonField_TypeDef;

extern void FML_Button_Init(ButtonField_TypeDef *handle,
                            void (*Init)(void),
                            bool (*GetButtonStatus)(uint8_t buttonID),
                            uint8_t buttonID);
extern void FML_Button_RegEventCallBack(ButtonField_TypeDef *handle,
                                        ButtonEvent_ENUM event,
                                        void (*callback)(void));
extern ButtonEvent_ENUM FML_Button_GetEvent(ButtonField_TypeDef *handle);
extern void FML_Button_Scan(void);

#endif /* __FML_BUTTON_H */
