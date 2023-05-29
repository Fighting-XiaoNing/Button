/**
 * @file      HdlButton.h
 * @author    XiaoNing
 * @post      Embedded Software Engineer
 * @brief     °´¼ü
 * @version   1.0.0
 * @date      2023-05-29
 * @copyright Copyright (c) 2023
 */
#ifndef __HDL_BUTTON_H
#define __HDL_BUTTON_H

#include <stdbool.h>
#include <stdint.h>

extern void HDL_Button_Init(void);
extern void HDL_Button_DeInit(void);
extern void HDL_Button_EnterLowPower(void);
extern void HDL_Button_ExitLowPower(void);
extern bool HDL_Button_ReadStatus(uint8_t buttonID);

#endif /* __HDL_BUTTON_H */
