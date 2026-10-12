#include "./myMain.h"

#include "zst_ultr.h"
#include "register.h"
#include "uart.h"
#include "time.h"
#include "sys_config_and_flash.h"

static const uint8_t filter_depth = 3;
static const uint8_t ultr_start = 0xA0;

static uint8_t get_echo_pin_level(struct _ultr * ultr);
static void set_trig_pin_level(struct _ultr * ultr, uint8_t level);
static uint32_t get_countVal(struct _ultr * ultr);

ultr_t ultr;
uint8_t need_update_flag = 0;

void setup(void)
{
    sys_config_info_init(); 
    rgb_set_color(0, 0, 255);
    sys_register.reg[1] = 255;
    sys_register.reg[2] = 0;
    sys_register.reg[3] = 0;
    sys_register.reg[4] = 255;

    ultr.get_echo_pin_level = get_echo_pin_level,
    ultr.set_trig_pin_level = set_trig_pin_level,
    ultr.get_countVal = get_countVal,
    ultr_init(&ultr, 1, 65535);
}

void loop(void)
{
    static uint8_t uart_rev_buffer[filter_depth + 1][3] = {0};
    static uint8_t uart_rev[filter_depth][3] = {0};

    if (1 == need_update_flag)
    {
        need_update_flag = 0;
        uint8_t need_xmodem = 1;
        set_sys_config_info(SYSCONF_MEM_LIST_NEED_XMODEM,&need_xmodem,1);
		sync_sys_config_info();
    }
    
    
//    if (1 == sys_register.changle_flag)
//    {
//        sys_register.changle_flag = 0;
        uint8_t r = (uint32_t)sys_register.reg[2 + 0] * sys_register.reg[1] / 255;
        uint8_t g = (uint32_t)sys_register.reg[2 + 1] * sys_register.reg[1] / 255;
        uint8_t b = (uint32_t)sys_register.reg[2 + 2] * sys_register.reg[1] / 255;

        rgb_set_color(r, g, b);
//    }

    // uart_send_bytes(&huart2, (uint8_t *)&ultr_start, 1);
    // if (uart_receive_bytes(&huart2, uart_rev_buffer[filter_depth], 3, 200) == 0)
    // {
    //     for (uint8_t i = 0; i < filter_depth; i++)
    //     {
    //         uart_rev_buffer[i][0] = uart_rev_buffer[i + 1][0];
    //         uart_rev_buffer[i][1] = uart_rev_buffer[i + 1][1];
    //         uart_rev_buffer[i][2] = uart_rev_buffer[i + 1][2];

    //         uart_rev[i][0] = uart_rev_buffer[i][0];
    //         uart_rev[i][1] = uart_rev_buffer[i][1];
    //         uart_rev[i][2] = uart_rev_buffer[i][2];
    //     }

    //     // 对uart_rev[x]冒泡排序
    //     for (uint8_t i = 0; i < filter_depth; i++)
    //     {
    //         uint32_t temp_val = uart_rev[i][0] << 16 | uart_rev[i][1] << 8 | uart_rev[i][2];
    //         for (uint8_t j = i + 1; j < filter_depth; j++)
    //         {
    //             uint32_t next_val = uart_rev[j][0] << 16 | uart_rev[j][1] << 8 | uart_rev[j][2];
    //             if (next_val < temp_val)
    //             {
    //                 // 交换
    //                 uint8_t tmp[3] = {uart_rev[i][0], uart_rev[i][1], uart_rev[i][2]};
    //                 uart_rev[i][0] = uart_rev[j][0];
    //                 uart_rev[i][1] = uart_rev[j][1];
    //                 uart_rev[i][2] = uart_rev[j][2];
    //                 uart_rev[j][0] = tmp[0];
    //                 uart_rev[j][1] = tmp[1];
    //                 uart_rev[j][2] = tmp[2];
    //                 temp_val = next_val;
    //             }
    //         }
    //     }

    //     ultr_register.reg[0] = uart_rev[filter_depth / 2][0];
    //     ultr_register.reg[1] = uart_rev[filter_depth / 2][1];
    //     ultr_register.reg[2] = uart_rev[filter_depth / 2][2];
    // }
    uint32_t distance = ultr_get_distance(&ultr) * 10000 / 10;
    ultr_register.reg[0] = (distance >> 16) & 0xFF;
    ultr_register.reg[1] = (distance >> 8) & 0xFF;
    ultr_register.reg[2] = distance & 0xFF;
    SysTickDelay(70);
}

static uint8_t get_echo_pin_level(struct _ultr * ultr)
{
    return GPIO_ReadPin(ULTR_ECHO_GPIO_Port, ULTR_ECHO_GPIO_Pin);
}

static void set_trig_pin_level(struct _ultr * ultr, uint8_t level)
{
    GPIO_WritePin(ULTR_TRIG_GPIO_Port, ULTR_TRIG_GPIO_Pin, (GPIO_PinState)level);
}

static uint32_t get_countVal(struct _ultr * ultr)
{
    return CW_BTIM1->CNT;
}
