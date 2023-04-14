/**
 * @file      FmlButton.c
 * @author    XiaoNing
 * @post      Embedded Software Engineer
 * @brief     ����
 * @version   1.0.0
 * @date      2023-04-03
 * @copyright Copyright (c) 2023
 */
#include "FmlButton.h"
#include "HdlButton.h"

enum ButtonFsmStatus
{
    BUTTON_PRESSDOWN_IDLE,
    BUTTON_RELEASE_LONGPRESS,
    BUTTON_PRESSDOWN_CLICK,
    BUTTON_HOLD_RELEASE
};

static ButtonField_TypeDef *gpButtonLinkList = NULL;                            // ��������ͷָ��

static void FML_Button_Init(ButtonField_TypeDef *handle,
                            void (*Init)(void),
                            bool (*GetButtonStatus)(uint8_t buttonID),
                            uint8_t buttonID);
static void FML_Button_DeInit(void);
static void FML_Button_EnterLowPower(void);
static void FML_Button_ExitLowPower(void);
static void FML_Button_RegEventCallBack(ButtonField_TypeDef *handle,
                                        ButtonEvent_ENUM event,
                                        void (*callback)(void));
static ButtonEvent_ENUM FML_Button_GetEvent(ButtonField_TypeDef *handle);
static void FML_Button_Debounce(ButtonField_TypeDef *handle);
static void FML_Button_Handle(ButtonField_TypeDef *handle);
static void FML_Button_Scan(void);

const FmlButton_TypeDef fmlButton = {
    FML_Button_Init,
    FML_Button_DeInit,
    FML_Button_EnterLowPower,
    FML_Button_ExitLowPower,
    FML_Button_RegEventCallBack,
    FML_Button_GetEvent,
    FML_Button_Scan
};

/**
 * @brief  ������ʼ��
 * @param  handle �������
 * @param  Init ��ʼ������
 * @param  GetButtonStatus ��ȡ����״̬�������Ƿ���Ч/������
 * @param  buttonID ����ID
 * @retval None
 */
static void FML_Button_Init(ButtonField_TypeDef *handle,
                            void (*Init)(void),
                            bool (*GetButtonStatus)(uint8_t buttonID),
                            uint8_t buttonID)
{
    ButtonField_TypeDef *p = NULL;

    handle->buttonID = buttonID;                                                // ����ID
    handle->event = EVENT_NONE_BTN;                                             // ���¼�
    handle->buttonTrigger = false;                                              // �޴���
    handle->buttonFsmVar = BUTTON_PRESSDOWN_IDLE;                               // ��ʼ��״̬��Ϊ����̬
    handle->GetButtonStatus = GetButtonStatus;                                  // ע�ᰴ��״̬��ȡ����
    Init();                                                                     // ��ʼ������

    if (gpButtonLinkList == NULL)                                               // Ϊ������
    {
        gpButtonLinkList = handle;                                              // ͷָ��ָ����Ԫ���
        gpButtonLinkList->next = NULL;                                          // ��Ԫ�ڵ���Ϊβ���
    }
    else
    {
        p = gpButtonLinkList;                                                   // ָ�򰴼�����
        while (p->next)                                                         // ����β��㣨�жϽ��ָ�����Ƿ�Ϊ�գ�
        {
            p = p->next;                                                        // ָ����һ�����
        }
        p->next = handle;                                                       // �����½��
        handle->next = NULL;                                                    // �½ڵ���Ϊβ���
    }
}

/**
 * @brief  ��������ʼ��
 * @param  None
 * @retval None
 */
static void FML_Button_DeInit(void)
{
    hdlButton.DeInit();                                                         // ����ʼ������
}

/**
 * @brief  ��������͹���
 * @param  None
 * @retval None
 */
static void FML_Button_EnterLowPower(void)
{
    hdlButton.EnterLowPower();                                                  // ��������͹���
}

/**
 * @brief  �����˳��͹���
 * @param  None
 * @retval None
 */
static void FML_Button_ExitLowPower(void)
{
    hdlButton.ExitLowPower();                                                   // �����˳��͹���
}

/**
 * @brief  ����ע���¼��ص�����
 * @param  handle �������
 * @param  event �¼�����
 * @param  callback �ص�����
 * @retval None
 */
static void FML_Button_RegEventCallBack(ButtonField_TypeDef *handle,
                                        ButtonEvent_ENUM event,
                                        void (*callback)(void))
{
    handle->callBackArray[event] = callback;                                    // ���ص��������뺯��ָ������
}

/**
 * @brief  ��ȡ�����¼�
 * @param  handle �������
 * @retval �¼� - ��ButtonEvent_ENUMö����
 */
static ButtonEvent_ENUM FML_Button_GetEvent(ButtonField_TypeDef *handle)
{
    return (ButtonEvent_ENUM)(handle->event);                                   // �¼�
}

/**
 * @brief  ��������
 * @param  handle �������
 * @retval None
 */
static void FML_Button_Debounce(ButtonField_TypeDef *handle)
{
    bool curButtonLevel = false;

    curButtonLevel = handle->GetButtonStatus(handle->buttonID);                 // ��ȡ��ǰ������ƽ
    if (curButtonLevel != handle->buttonTrigger)                                // ���˲�����ȶ���ƽ��һ�£���ƽ���䣩
    {
        if (++handle->debounceCnt >= DEBOUNCE_TIME)                             // �������������ж������Ƿ����
        {
            handle->buttonTrigger = curButtonLevel;                             // ����������ĵ�ƽ
            handle->debounceCnt = 0;                                            // ������������ֵ
        }
    }
    else                                                                        // ���˲�����ȶ���ƽһ�£���ƽ�ޱ仯��
    {
        handle->debounceCnt = 0;
    }
}

/**
 * @brief  ��������
 * @param  handle �������
 * @retval None
 * @note   ������û�д������ٽ��д������Ǳ������ߴ����ᵼ�´�������ж����
 */
static void FML_Button_Handle(ButtonField_TypeDef *handle)
{
    FML_Button_Debounce(handle);                                                // ��������
    if ((handle->buttonFsmVar != BUTTON_PRESSDOWN_IDLE)                         // ����״̬�ǿ���̬
     && (handle->ticks < UINT16_MAX))
    {
        handle->ticks++;                                                        // �δ����ֵ
    }

    switch (handle->buttonFsmVar)
    {
        case BUTTON_PRESSDOWN_IDLE://///////////////////////////////////////////// ��������or����
            if (handle->buttonTrigger == true)                                  // ���°���
            {
                handle->event = EVENT_PRESS_DOWN;                               // �����¼�
                if (handle->callBackArray[EVENT_PRESS_DOWN])                    // ��ע���¼��ص�����
                {
                    handle->callBackArray[EVENT_PRESS_DOWN]();                  // �����¼��ص�����
                }
                if (handle->clickCnt < UINT8_MAX)
                {
                    handle->clickCnt++;                                         // �������
                }
                handle->ticks = 0;
                handle->buttonFsmVar = BUTTON_RELEASE_LONGPRESS;
            }
            else
            {
                handle->event = EVENT_NONE_BTN;                                 // �ް����¼�
            }
            break;
        case BUTTON_RELEASE_LONGPRESS://////////////////////////////////////////// �����ͷ�or����
            if (handle->buttonTrigger == false)                                 // �ɿ�����
            {
                handle->event = EVENT_RELEASE;                                  // �ͷ��¼�
                if (handle->callBackArray[EVENT_RELEASE])                       // ��ע���¼��ص�����
                {
                    handle->callBackArray[EVENT_RELEASE]();                     // �����¼��ص�����
                }
                handle->ticks = 0;
                handle->buttonFsmVar = BUTTON_PRESSDOWN_CLICK;
            }
            else if (handle->ticks >= LONG_PRESS_TIME)                          // �ﵽ����ʱ��
            {
                handle->event = EVENT_LONG_PRESS;                               // �����¼�
                if (handle->callBackArray[EVENT_LONG_PRESS])                    // ��ע���¼��ص�����
                {
                    handle->callBackArray[EVENT_LONG_PRESS]();                  // �����¼��ص�����
                }
                handle->clickCnt = 0;
                handle->ticks = 0;
                handle->buttonFsmVar = BUTTON_HOLD_RELEASE;
            }
            break;
        case BUTTON_PRESSDOWN_CLICK:////////////////////////////////////////////// ��������or���
            if (handle->buttonTrigger == true)                                  // ��������
            {
                handle->event = EVENT_PRESS_DOWN;                               // �����¼�
                if (handle->callBackArray[EVENT_PRESS_DOWN])                    // ��ע���¼��ص�����
                {
                    handle->callBackArray[EVENT_PRESS_DOWN]();                  // �����¼��ص�����
                }
                if (handle->clickCnt < UINT8_MAX)
                {
                    handle->clickCnt++;                                         // �������
                }
                handle->ticks = 0;
                handle->buttonFsmVar = BUTTON_RELEASE_LONGPRESS;
            }
            else if (handle->ticks >= CLICK_TIME)                               // ���ִﵽ���ʱ��
            {
                handle->event = EVENT_CLICK;                                    // ����¼�
                if (handle->callBackArray[EVENT_CLICK])                         // ��ע���¼��ص�����
                {
                    handle->callBackArray[EVENT_CLICK]();                       // �����¼��ص�����
                }
                handle->clickCnt = 0;
                handle->ticks = 0;
                handle->buttonFsmVar = BUTTON_PRESSDOWN_IDLE;
            }
            break;
        case BUTTON_HOLD_RELEASE:///////////////////////////////////////////////// ������סor�ɿ�
            if (handle->buttonTrigger == true)                                  // ��������
            {
                handle->event = EVENT_PRESS_HOLD;                               // ��ס�¼�
                if (handle->callBackArray[EVENT_PRESS_HOLD])                    // ��ע���¼��ص�����
                {
                    handle->callBackArray[EVENT_PRESS_HOLD]();                  // �����¼��ص�����
                }
            }
            else
            {
                handle->event = EVENT_RELEASE;                                  // �ͷ��¼�
                if (handle->callBackArray[EVENT_RELEASE])                       // ��ע���¼��ص�����
                {
                    handle->callBackArray[EVENT_RELEASE]();                     // �����¼��ص�����
                }
                handle->clickCnt = 0;
                handle->ticks = 0;
                handle->buttonFsmVar = BUTTON_PRESSDOWN_IDLE;
            }
            break;
        default:
            break;
    }
}

/**
 * @brief  ����ɨ��
 * @param  None
 * @retval None
 */
static void FML_Button_Scan(void)
{
    ButtonField_TypeDef *curButton = NULL;

    for (curButton = gpButtonLinkList; curButton; curButton = curButton->next)
    {
        FML_Button_Handle(curButton);                                           // ����ǰ����
    }
}
