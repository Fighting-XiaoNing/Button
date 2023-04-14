/**
 * @file      main.c
 * @author    XiaoNing
 * @post      Embedded Software Engineer
 * @brief     主函数
 * @version   1.0.0
 * @date      2023-04-03
 * @copyright Copyright (c) 2023
 */
#include <reg52.h>
#include <stdint.h>
#include "FmlButton.h"
#include "HdlButton.h"

/////////////////////////////////// 测试用 ///////////////////////////////////
static void Delay_Ms(uint16_t nms)
{
    uint16_t x = 0, y = 0;
    for (x = nms; x > 0; x--)
        for (y = 114; y > 0; y--);
}
void SendData(uint8_t dat)
{
    SBUF = dat;
    while (!TI);
    TI = 0;
}

#pragma OT(0)
void SendString(char *s)
{
    while (*s)
    {
        SendData(*s++);
    }
}
#pragma OT(8)

/////////////////////////////////// 有用代码 ///////////////////////////////////
ButtonField_TypeDef xdata button0 = {0};
ButtonField_TypeDef xdata button1 = {0};
ButtonField_TypeDef xdata button2 = {0};
/////////////////////////////////// Button 0 ///////////////////////////////////
void Btn0PressDown(void)
{
    SendString("Button0 ");
    SendString("Down\r\n");
}
void Btn0Release(void)
{
    SendString("Button0 ");
    SendString("Up\r\n");
}
void Btn0Click(void)
{
    SendString("Button0 ");
    SendData(button0.clickCnt + 0x30);
    SendString("击\r\n");
}
void Btn0LongPress(void)
{
    SendString("Button0 ");
    SendString("长按\r\n");
}
void Btn0PressHold(void)
{
    SendString("Button0 ");
    SendString("Hold\r\n");
}
/////////////////////////////////// Button 1 ///////////////////////////////////
void Btn1PressDown(void)
{
    SendString("Button1 ");
    SendString("Down\r\n");
}
void Btn1Release(void)
{
    SendString("Button1 ");
    SendString("Up\r\n");
}
void Btn1Click(void)
{
    SendString("Button1 ");
    SendData(button1.clickCnt + 0x30);
    SendString("击\r\n");
}
void Btn1LongPress(void)
{
    SendString("Button1 ");
    SendString("长按\r\n");
}
void Btn1PressHold(void)
{
    SendString("Button1 ");
    SendString("Hold\r\n");
}
/////////////////////////////////// Button 2 ///////////////////////////////////
void Btn2PressDown(void)
{
    SendString("Button2 ");
    SendString("Down\r\n");
}
void Btn2Release(void)
{
    SendString("Button2 ");
    SendString("Up\r\n");
}
void Btn2Click(void)
{
    SendString("Button2 ");
    SendData(button2.clickCnt + 0x30);
    SendString("击\r\n");
}
void Btn2LongPress(void)
{
    SendString("Button2 ");
    SendString("长按\r\n");
}
void Btn2PressHold(void)
{
    SendString("Button2 ");
    SendString("Hold\r\n");
}

/**
 * @brief  主函数
 * @param  None
 * @retval int
 */
int main(void)
{
    SCON = 0x50;                               // 8-bit variable UART
    TMOD = 0x20;                               // Set Timer1 as 8-bit auto reload mode
    TH1 = TL1 = -(11059200L / 12 / 32 / 9600); // Set auto-reload vaule
    TR1 = 1;                                   // Timer1 start run

    FML_Button_RegEventCallBack(&button0, EVENT_PRESS_DOWN, Btn0PressDown);
    FML_Button_RegEventCallBack(&button0, EVENT_RELEASE, Btn0Release);
    FML_Button_RegEventCallBack(&button0, EVENT_CLICK, Btn0Click);
    FML_Button_RegEventCallBack(&button0, EVENT_LONG_PRESS, Btn0LongPress);
    FML_Button_RegEventCallBack(&button0, EVENT_PRESS_HOLD, Btn0PressHold);
    FML_Button_Init(&button0, hdlButton.Init, hdlButton.ReadStatus, 0);
    FML_Button_RegEventCallBack(&button1, EVENT_PRESS_DOWN, Btn1PressDown);
    FML_Button_RegEventCallBack(&button1, EVENT_RELEASE, Btn1Release);
    FML_Button_RegEventCallBack(&button1, EVENT_CLICK, Btn1Click);
    FML_Button_RegEventCallBack(&button1, EVENT_LONG_PRESS, Btn1LongPress);
    FML_Button_RegEventCallBack(&button1, EVENT_PRESS_HOLD, Btn1PressHold);
    FML_Button_Init(&button1, hdlButton.Init, hdlButton.ReadStatus, 1);
    FML_Button_RegEventCallBack(&button2, EVENT_PRESS_DOWN, Btn2PressDown);
    FML_Button_RegEventCallBack(&button2, EVENT_RELEASE, Btn2Release);
    FML_Button_RegEventCallBack(&button2, EVENT_CLICK, Btn2Click);
    FML_Button_RegEventCallBack(&button2, EVENT_LONG_PRESS, Btn2LongPress);
    FML_Button_RegEventCallBack(&button2, EVENT_PRESS_HOLD, Btn2PressHold);
    FML_Button_Init(&button2, hdlButton.Init, hdlButton.ReadStatus, 2);

    SendString("开始测试\r\n");
    while (1)
    {
        Delay_Ms(5);
        FML_Button_Scan();
    }
}
