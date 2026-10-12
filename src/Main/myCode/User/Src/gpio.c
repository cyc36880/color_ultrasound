#include "gpio.h"
#include "zst_ultr.h"

extern ultr_t ultr;

void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __SYSCTRL_GPIOB_CLK_ENABLE();

    // 初始化超声波引脚
    GPIO_InitStruct.IT = GPIO_IT_NONE;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pins = ULTR_TRIG_GPIO_Pin;
    GPIO_Init(ULTR_TRIG_GPIO_Port, &GPIO_InitStruct);


    GPIO_InitStruct.IT = GPIO_IT_RISING | GPIO_IT_FALLING;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pins = ULTR_ECHO_GPIO_Pin;
    GPIO_Init(ULTR_ECHO_GPIO_Port, &GPIO_InitStruct);
    //配置中断滤波
    GPIO_ConfigFilter(CW_GPIOB, bv6, GPIO_FLTCLK_HCLK2);
    //清除PA06中断标志并使能NVIC
    GPIOB_INTFLAG_CLR(bv6);
    NVIC_EnableIRQ(GPIOB_IRQn);
}

void GPIOB_IRQHandlerCallback(void)
{
    if (CW_GPIOB->ISR_f.PIN6)
    {
        GPIOB_INTFLAG_CLR(bv6);
        ultr_gpio_interrupt_callback(&ultr);
    }  
}
