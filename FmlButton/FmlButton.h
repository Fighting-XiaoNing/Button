/**
 * @file      FmlButton.h
 * @author    XiaoNing
 * @post      Embedded Software Engineer
 * @brief     ����
 * @version   1.0.0
 * @date      2023-05-29
 * @copyright Copyright (c) 2023
 */
#ifndef __FML_BUTTON_H
#define __FML_BUTTON_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define SCAN_TIME 5                                                             // ɨ��ʱ�䣨ms��
#define DEBOUNCE_TIME (15 / SCAN_TIME)                                          // ����ʱ��
#define CLICK_TIME (300 /  SCAN_TIME)                                           // ������ʱ��
#define LONG_PRESS_TIME (1000 / SCAN_TIME)                                      // ��������ʱ��

typedef enum ButtonEvent
{
    EVENT_PRESS_DOWN,                                                           // ����
    EVENT_RELEASE,                                                              // �ͷ�
    EVENT_CLICK,                                                                // ���
    EVENT_LONG_PRESS,                                                           // ����
    EVENT_PRESS_HOLD,                                                           // ��ס
    EVENT_END,
    EVENT_NONE_BTN                                                              // ���¼�
} ButtonEvent_ENUM;

typedef struct ButtonField
{
    uint16_t ticks;                                                             // ��ʱ
    uint8_t buttonID;                                                           // ����ID            //λ��4[0, 15]
    uint8_t debounceCnt;                                                        // ��������          //λ��4[0, 15]
    uint8_t clickCnt;                                                           // �������          //λ��3[0, 7]
    uint8_t event;                                                              // �¼�              //λ��3[0, 7]
    uint8_t buttonFsmVar;                                                       // ״̬������        //λ��2[0, 3]
    bool buttonTrigger;                                                         // �����󰴼�����״̬ //λ��1[0, 1]
    bool (*GetButtonStatus)(uint8_t buttonID);                                  // ��ȡ����״̬����
    void (*callBackArray[EVENT_END])(void);                                     // ����¼��ص�����
    struct ButtonField *next;                                                   // ����ָ����
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
