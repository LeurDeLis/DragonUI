//
// Created by fukan on 25-6-4.
//
#include "oled_cmd.h"
// ==================================================================================
// 初始化命令列表（使用宏定义代替原始数值）
// ==================================================================================


// 初始化命令列表
const OLED_Init_Command oled_init_commands[27] =
{
    {0xAE, "--turn off oled panel"},
    {0x00, "---set low column address"},
    {0x10, "---set high column address"},
    {0x40, "--set start line address (0x00~0x3F)"},
    {0x81, "--set contrast control register"},
    {0xCF, "Set SEG Output Current Brightness"},
    {0xA1, "--Set SEG/Column Mapping (0xA0=reverse, 0xA1=normal)"},
    {0xC8, "--Set COM/Row Scan Direction (0xC0=reverse, 0xC8=normal)"},
    {0xA6, "--set normal display"},
    {0xA8, "--set multiplex ratio (1 to 64)"},
    {0x3F, "--1/64 duty"},
    {0xD3, "-set display offset (Shift Mapping RAM Counter 0x00~0x3F)"},
    {0x00, "-not offset"},
    {0xD5, "--set display clock divide ratio/oscillator frequency"},
    {0x80, "--set divide ratio (Set Clock as 100 Frames/Sec)"},
    {0xD9, "--set pre-charge period"},
    {0xF1, "Set Pre-Charge as 15 Clocks & Discharge as 1 Clock"},
    {0xDA, "--set com pins hardware configuration"},
    {0x12, ""},
    {0xDB, "--set vcomh"},
    {0x40, "Set VCOM Deselect Level"},
    {0x20, "-Set Page Addressing Mode (0x00/0x01/0x02)"},
    {0x02, ""},
    {0x8D, "--set Charge Pump enable/disable"},
    {0x14, "--enable charge pump (0x10:disable)"},
    {0xA4, "Disable Entire Display On (0xA4/0xA5)"},
    {0xA6, "Disable Inverse Display On (0xA6/0xA7)"}
//    {0xAF, "--turn on oled panel"}
};
