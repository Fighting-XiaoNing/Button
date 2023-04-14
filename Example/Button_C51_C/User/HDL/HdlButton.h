/**
 * @file      HdlButton.h
 * @author    XiaoNing
 * @post      Embedded Software Engineer
 * @brief     °´¼ü
 * @version   1.0.0
 * @date      2023-03-16
 * @copyright Copyright (c) 2023
 */
#ifndef __HDL_BUTTON_H
#define __HDL_BUTTON_H

#include <stdbool.h>
#include <stdint.h>

typedef struct HdlButton
{
    void (*Init)(void);
    void (*DeInit)(void);
    void (*EnterLowPower)(void);
    void (*ExitLowPower)(void);
    bool (*ReadStatus)(uint8_t buttonID);
} HdlButton_TypeDef;

extern const HdlButton_TypeDef hdlButton;

#endif /* __HDL_BUTTON_H */
