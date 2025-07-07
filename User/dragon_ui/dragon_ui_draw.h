//
// Created by Fir on 24-11-29.
//

#ifndef __DRAGON_UI_DRAW_H_
#define __DRAGON_UI_DRAW_H_

#include "main.h"
#include "string.h"
#include "stdio.h"
#include "spi.h"
#include "oled_cmd.h"
#include "../u8g2/u8g2.h"

#define OLED_SPI &hspi2

#define ui_max_font u8g2_font_dragon_ui_chinese_max
#define ui_nor_font u8g2_font_dragon_ui_chinese
#define ui_mid_font u8g2_font_dragon_ui_chinese_m
#define ui_min_font u8g2_font_dragon_ui_chinese_n

extern u8g2_t u8g2;

#define OLED_HEIGHT 64
#define OLED_WIDTH 128

#define get_ticks() HAL_GetTick()
#define delay(ms) HAL_Delay(ms)
#define oled_set_font(font) u8g2_SetFont(&u8g2, font)
#define oled_draw_str(x, y, str) u8g2_DrawStr(&u8g2, x, y, str)
#define oled_draw_UTF8(x, y, str) u8g2_DrawUTF8(&u8g2, x, y, str)
#define oled_get_str_width(str) u8g2_GetStrWidth(&u8g2, str)
#define oled_get_UTF8_width(str) u8g2_GetUTF8Width(&u8g2, str)
#define oled_get_str_height() u8g2_GetMaxCharHeight(&u8g2)
#define oled_draw_pixel(x, y) u8g2_DrawPixel(&u8g2, x, y)
#define oled_draw_circle(x, y, r) u8g2_DrawCircle(&u8g2, x, y, r, U8G2_DRAW_ALL)
#define oled_draw_R_box(x, y, w, h, r) u8g2_DrawRBox(&u8g2, x, y, w, h, r)
#define oled_draw_box(x, y, w, h) u8g2_DrawBox(&u8g2, x, y, w, h)
#define oled_draw_button_frame(x, y, flags, tw, ph, pv) u8g2_DrawButtonFrame(&u8g2, x, y, flags, tw, ph, pv)
#define oled_draw_frame(x, y, w, h) u8g2_DrawFrame(&u8g2, x, y, w, h)
#define oled_draw_R_frame(x, y, w, h, r) u8g2_DrawRFrame(&u8g2, x, y, w, h, r)
#define oled_draw_H_line(x, y, l) u8g2_DrawHLine(&u8g2, x, y, l)
#define oled_draw_V_line(x, y, h) u8g2_DrawVLine(&u8g2, x, y, h)
#define oled_draw_line(x1, y1, x2, y2) u8g2_DrawLine(&u8g2, x1, y1, x2, y2)
// #define oled_draw_H_dotted_line(x, y, l) u8g2_DrawHDottedLine(&u8g2, x, y, l)
// #define oled_draw_V_dotted_line(x, y, h) u8g2_DrawVDottedLine(&u8g2, x, y, h)
#define oled_draw_bMP(x, y, w, h, bitMap) u8g2_DrawBMP(&u8g2, x, y, w, h, bitMap)
#define oled_set_draw_color(color) u8g2_SetDrawColor(&u8g2, color)
#define oled_set_font_mode(mode) u8g2_SetFontMode(&u8g2, mode)
#define oled_set_font_direction(dir) u8g2_SetFontDirection(&u8g2, dir)
#define oled_draw_pixel(x, y) u8g2_DrawPixel(&u8g2, x, y)
#define oled_clear_buffer() u8g2_ClearBuffer(&u8g2)
#define oled_send_buffer() u8g2_SendBuffer(&u8g2)
#define oled_send_area_buffer(x, y, w, h) u8g2_UpdateDisplayArea(&u8g2, x, y, w, h)

void dragon_ui_driver_init(void);
void oled_transmit_cmd(const unsigned char _cmd);
void draw(void);
#endif //__DRAGON_UI_DRAW_H_
