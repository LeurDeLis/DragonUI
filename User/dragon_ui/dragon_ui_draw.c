//
// Created by Fir on 24-11-30.
//

#include "dragon_ui_draw.h"

u8g2_t u8g2;

void Delay_us(const uint32_t us)
{
    const uint32_t sys_tick_freq = HAL_RCC_GetSysClockFreq() / 1000; // 每 ms 的 SysTick ticks
    const uint32_t tick_per_us = sys_tick_freq / 1000;               // 每 us 的 SysTick ticks
    const uint32_t start = SysTick->VAL;

    while(start - SysTick->VAL < us * tick_per_us);
}

uint8_t u8x8_byte_4wire_hw_spi(u8x8_t* u8x8, const uint8_t msg, const uint8_t arg_int, void* arg_ptr)
{
    const uint8_t* p = (uint8_t*) arg_ptr;
    switch(msg)
    {
        case U8X8_MSG_BYTE_SEND:
            for(int i = 0; i < arg_int; i++)
                HAL_SPI_Transmit(&hspi2,
                                 (const uint8_t*)(p + i),
                                 1,
                                 1000);
            break;
        case U8X8_MSG_BYTE_SET_DC:
            if(arg_int) HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
            else
                HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
        case U8X8_MSG_BYTE_END_TRANSFER:
            break;
        default:
            return 0;
    }
    return 1;
}

uint8_t u8x8_gpio_and_delay_hw_stm32(U8X8_UNUSED u8x8_t* u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void* arg_ptr)
{
    switch(msg)
    {
        //Initialize SPI peripheral
        case U8X8_MSG_GPIO_AND_DELAY_INIT:
            MX_SPI2_Init();
            break;
        //Function which implements a delay, arg_int contains the amount of ms
        case U8X8_MSG_DELAY_MILLI:
            HAL_Delay(arg_int);
            break;
        //Function which delays 10us
        case U8X8_MSG_DELAY_10MICRO:
            Delay_us(arg_int);
            break;
        //Function which delays 100ns
        case U8X8_MSG_DELAY_100NANO:
            __NOP();
            break;
        // Function to define the logic level of the CS line
        case U8X8_MSG_GPIO_CS:
            HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, (GPIO_PinState)arg_int);
            break;
        //Function to define the logic level of the Data/ Command line
        case U8X8_MSG_GPIO_DC:
            HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, (GPIO_PinState)arg_int);
            break;
        //Function to define the logic level of the RESET line
        case U8X8_MSG_GPIO_RESET:
            HAL_GPIO_WritePin(OLED_RES_GPIO_Port, OLED_RES_Pin, (GPIO_PinState)arg_int);
            break;
        default:
            return 0; //A message was received which is not implemented, return 0 to indicate an error
    }
    return 1; // command processed successfully.
}

void oled_transmit_cmd(const unsigned char _cmd)
{
    //NOLINT
    // HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_RESET);
    HAL_SPI_Transmit_DMA(OLED_SPI, &_cmd, 1);
    HAL_SPI_Transmit(OLED_SPI, &_cmd, 1, 200);
}

void oled_transmit_data(unsigned char _data, const unsigned char _mode)
{
    //NOLINT
    if(!_mode) _data = ~_data;
    // HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, GPIO_PIN_SET);
    HAL_SPI_Transmit_DMA(OLED_SPI, &_data, 1);
    HAL_SPI_Transmit(OLED_SPI, &_data, 1, 200);
}

void oled_set_cursor(const unsigned char _x, const unsigned char _y)
{
    oled_transmit_cmd(0xB0 | _y);
    oled_transmit_cmd(0x10 | (_x & 0xF0) >> 4);
    oled_transmit_cmd(0x00 | (_x & 0x0F));
}

void oled_fill(const unsigned char _data)
{
    for(uint8_t k = 0; k < 8; k++)
    {
        oled_transmit_cmd(0xb0 + k);
        oled_transmit_cmd(0x00);
        oled_transmit_cmd(0x10);
        for(uint8_t n = 0; n < 128; n++)
            oled_transmit_data(_data, 1);
    }
}

// 初始化OLED
void oled_init(void)
{
    for(int i = 0; i < sizeof(oled_init_commands) / sizeof(oled_init_commands[0]); i++)
    {
        oled_transmit_cmd(oled_init_commands[i].cmd);
    }
    oled_fill(0);
    oled_transmit_cmd(0xAF); /* display on */
}

void u8g2_init(u8g2_t* u8g2)
{
    u8g2_Setup_ssd1306_128x64_noname_f(u8g2,
                                       U8G2_R0,
                                       u8x8_byte_4wire_hw_spi,
                                       u8x8_gpio_and_delay_hw_stm32);
    u8g2_InitDisplay(u8g2);
    u8g2_SetPowerSave(u8g2, 0);
    u8g2_ClearBuffer(u8g2);
    u8g2_SetFontMode(u8g2, 1);
    u8g2_SetFontDirection(u8g2, 0);
    u8g2_SetFont(u8g2, ui_nor_font);

}


void dragon_ui_driver_init(void)
{
    oled_init();
    u8g2_init(&u8g2);
}

/*官方logo的Demo*/
void draw(void)
{
    oled_set_font_mode(1);         // 设置字体模式：透明背景
    oled_set_font_direction(0);    // 设置字体方向：水平
    oled_set_font(u8g2_font_inb24_mf);  // 设置字体：inb24_mf
    oled_draw_str(0, 20, "U");     // 绘制字符 U

    oled_set_font_direction(1);    // 设置字体方向：垂直（旋转90°）
    oled_set_font(u8g2_font_inb30_mn);  // 设置字体：inb30_mn
    oled_draw_str(21, 8, "8");     // 绘制字符 8

    oled_set_font_direction(0);    // 恢复字体方向为水平
    oled_set_font(u8g2_font_inb24_mf);  // 设置字体：inb24_mf
    oled_draw_str(51, 30, "g");    // 绘制字符 g
    oled_draw_str(67, 30, "\xb2"); // 绘制特殊符号 μ 或其他（取决于字库）

    // 绘制两条平行的水平线
    oled_draw_H_line(2, 35, 47);
    oled_draw_H_line(3, 36, 47);

    // 绘制两条平行的垂直线
    oled_draw_V_line(45, 32, 12);
    oled_draw_V_line(46, 33, 12);

    // 设置小字体并绘制网址
    oled_set_font(u8g2_font_4x6_tr);
    oled_draw_str(1, 54, "github.com/olikraus/&u8g2");
    oled_send_buffer();
}

