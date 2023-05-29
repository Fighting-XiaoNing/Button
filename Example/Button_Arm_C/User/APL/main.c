/**
 * @file      main.c
 * @author    XiaoNing
 * @post      Embedded Software Engineer
 * @brief     ������
 * @version   1.0.0
 * @date      2023-05-29
 * @copyright Copyright (c) 2023
 */
#include "stm32f1xx_hal.h"
#include <stdint.h>
#include <stdio.h>
#include "FmlButton.h"
#include "HdlButton.h"

static void SystemClock_Config(void);
void Error_Handler(void);

UART_HandleTypeDef huart1 = {0};

void MX_USART1_UART_Init(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        Error_Handler();
    }
}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (uartHandle->Instance == USART1)
    {
        __HAL_RCC_USART1_CLK_ENABLE();
        __HAL_RCC_GPIOA_CLK_ENABLE();

        GPIO_InitStruct.Pin = GPIO_PIN_9;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        GPIO_InitStruct.Pin = GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *uartHandle)
{
    if (uartHandle->Instance == USART1)
    {
        __HAL_RCC_USART1_CLK_DISABLE();
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);
    }
}
/////////////////////////////////// ���ô��� ///////////////////////////////////
ButtonField_TypeDef button0 = {0};
ButtonField_TypeDef button1 = {0};
ButtonField_TypeDef button2 = {0};
/////////////////////////////////// Button 0 ///////////////////////////////////
void Btn0PressDown(void)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button0 Down\r\n", 14, 0xFFFF);
}
void Btn0Release(void)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button0 Up\r\n", 12, 0xFFFF);
}
void Btn0Click(void)
{
    char array[2] = {0};
    sprintf(array, "%d", button0.clickCnt);
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button0 ", 8, 0xFFFF);
    HAL_UART_Transmit(&huart1, (uint8_t *)array, 1, 0xFFFF);
    HAL_UART_Transmit(&huart1, (uint8_t *)"Click\r\n", 7, 0xFFFF);
}
void Btn0LongPress(void)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button0 LongPress\r\n", 19, 0xFFFF);
}
void Btn0PressHold(void)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button0 Hold\r\n", 14, 0xFFFF);
}
/////////////////////////////////// Button 1 ///////////////////////////////////
void Btn1PressDown(void)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button1 Down\r\n", 14, 0xFFFF);
}
void Btn1Release(void)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button1 Up\r\n", 12, 0xFFFF);
}
void Btn1Click(void)
{
    char array[2] = {0};
    sprintf(array, "%d", button1.clickCnt);
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button1 ", 8, 0xFFFF);
    HAL_UART_Transmit(&huart1, (uint8_t *)array, 1, 0xFFFF);
    HAL_UART_Transmit(&huart1, (uint8_t *)"Click\r\n", 7, 0xFFFF);
}
void Btn1LongPress(void)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button1 LongPress\r\n", 19, 0xFFFF);
}
void Btn1PressHold(void)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button1 Hold\r\n", 14, 0xFFFF);
}
/////////////////////////////////// Button 2 ///////////////////////////////////
void Btn2PressDown(void)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button2 Down\r\n", 14, 0xFFFF);
}
void Btn2Release(void)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button2 Up\r\n", 12, 0xFFFF);
}
void Btn2Click(void)
{
    char array[2] = {0};
    sprintf(array, "%d", button2.clickCnt);
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button2 ", 8, 0xFFFF);
    HAL_UART_Transmit(&huart1, (uint8_t *)array, 1, 0xFFFF);
    HAL_UART_Transmit(&huart1, (uint8_t *)"Click\r\n", 7, 0xFFFF);
}
void Btn2LongPress(void)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button2 LongPress\r\n", 19, 0xFFFF);
}
void Btn2PressHold(void)
{
    HAL_UART_Transmit(&huart1, (uint8_t *)"Button2 Hold\r\n", 14, 0xFFFF);
}

/**
 * @brief  ������
 * @param  None
 * @retval int
 */
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_USART1_UART_Init();
    FML_Button_RegEventCallBack(&button0, EVENT_PRESS_DOWN, Btn0PressDown);
    FML_Button_RegEventCallBack(&button0, EVENT_RELEASE, Btn0Release);
    FML_Button_RegEventCallBack(&button0, EVENT_CLICK, Btn0Click);
    FML_Button_RegEventCallBack(&button0, EVENT_LONG_PRESS, Btn0LongPress);
    FML_Button_RegEventCallBack(&button0, EVENT_PRESS_HOLD, Btn0PressHold);
    FML_Button_Init(&button0, HDL_Button_Init, HDL_Button_ReadStatus, 0);
    FML_Button_RegEventCallBack(&button1, EVENT_PRESS_DOWN, Btn1PressDown);
    FML_Button_RegEventCallBack(&button1, EVENT_RELEASE, Btn1Release);
    FML_Button_RegEventCallBack(&button1, EVENT_CLICK, Btn1Click);
    FML_Button_RegEventCallBack(&button1, EVENT_LONG_PRESS, Btn1LongPress);
    FML_Button_RegEventCallBack(&button1, EVENT_PRESS_HOLD, Btn1PressHold);
    FML_Button_Init(&button1, HDL_Button_Init, HDL_Button_ReadStatus, 1);
    FML_Button_RegEventCallBack(&button2, EVENT_PRESS_DOWN, Btn2PressDown);
    FML_Button_RegEventCallBack(&button2, EVENT_RELEASE, Btn2Release);
    FML_Button_RegEventCallBack(&button2, EVENT_CLICK, Btn2Click);
    FML_Button_RegEventCallBack(&button2, EVENT_LONG_PRESS, Btn2LongPress);
    FML_Button_RegEventCallBack(&button2, EVENT_PRESS_HOLD, Btn2PressHold);
    FML_Button_Init(&button2, HDL_Button_Init, HDL_Button_ReadStatus, 2);
    HAL_UART_Transmit(&huart1, (uint8_t *)"Start Test!\r\n", 13, 0xFFFF);
    while (1)
    {
        HAL_Delay(5);
        FML_Button_Scan();
    }
}

static void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

void Error_Handler(void)
{
    __disable_irq();
    while (1)
    {
    }
}
