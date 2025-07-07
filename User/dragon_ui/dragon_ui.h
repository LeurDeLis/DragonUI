//
// Created by fukan on 25-6-4.
//

#ifndef DRAGON_UI_H
#define DRAGON_UI_H

#include <sys/types.h>

#include "dragon_ui_draw.h"
#include "../u8g2/u8g2.h"
#include <math.h>
// #define MENU_NUM 4 // 菜单列表数量

// 矩形选项框的圆角、高
#define FRAME_R 5
// #define FRAME_H (oled_get_str_height() + 5)

// 菜单列表的起始坐标
#define ITEM_START_X 8
// #define ITEM_START_Y (oled_get_str_height())
#define ITEM_SPACE_Y 1

#define AnimationTotalStep 15


#ifndef CLAMP
#define CLAMP(x, low, high) (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))
#endif

#ifndef ABS
#define ABS(x) ((x) >= 0 ? (x) : -(x))
#endif

typedef enum {
    UI_MODE_MENU,     // 主菜单
    UI_MODE_SUBMENU,  // 子菜单
} MenuItemType;

typedef enum {
    MENU_ITEM_TYPE_NORMAL,     // 普通菜单项（仅显示）
    MENU_ITEM_TYPE_SETTING,    // 可设置项（点击进入设置页面）
    MENU_ITEM_TYPE_FUNCTION,   // 功能项（点击执行特定功能）
    MENU_ITEM_TYPE_SHOW_INFO,   // 显示信息项（点击显示特定信息）
    MENU_END
} Menu_Type;

typedef enum {
    ui_next_page = 1,
    ui_pre_page,
}ShiftPage;

// 定义选项框属性
typedef struct {
    // 选项框属性
    uint8_t x;    // 选项框的起始x坐标
    uint8_t y;    // 选项框的起始y坐标
    uint8_t w;    // 选项框的宽度
    uint8_t h;    // 选项框的高度
    uint8_t r;    // 圆角半径（保持不变，只用于绘制）
} Frame;

typedef struct {
    // 选项框属性
    uint8_t x;    // 选项框的起始x坐标
    uint8_t y;    // 选项框的起始y坐标
    uint8_t flags;
    uint8_t w;    // 选项框的宽度
    uint8_t ph;    // 选项框的高度
    uint8_t pv;    // 圆角半径（保持不变，只用于绘制）
    uint8_t r;    // 圆角半径（保持不变，只用于绘制）
} ButtonFrame;

// 定义菜单项属性
typedef struct {
    uint8_t x;
    uint8_t y;
}Ui_Point;

// 定义菜单项属性
typedef struct {
    uint8_t x;
    uint8_t y;
    uint8_t w;
    uint8_t h;
} Ui_Area;

typedef enum {
    font_max,
    font_nor,
    font_mid,
    font_min
}Font_Size;

// 动画速度枚举类型
typedef enum {
    ANIMATION_SPEED_FAST = 10,
    ANIMATION_SPEED_MEDIUM = 20,
    ANIMATION_SPEED_SLOW = 30
}Animation_Speed;

typedef enum {
    ANIMATION_STYLE_SMOOTH,// 平滑
    ANIMATION_STYLE_JUMP// 跳跃
}Animation_Style;


// 表示单个菜单页面的结构体
typedef struct MenuPage {
    MenuItemType MenuType;  // 菜单类型
    Animation_Speed MovingSpeed;  // 光标或选择框移动的速度（例如：快、慢）
    uint8_t CursorStyle;  // 光标样式（例如：箭头、块状光标等）
    Animation_Style MoveStyle;  // 移动样式（例如：平滑移动、跳跃移动等）
    Frame moving_frame;   // 当前动画帧状态
    ButtonFrame moving_button_frame;   // 当前动画帧状态
    int roll_offset;

    Font_Size TextFontStyle;  // 菜单文本的字体样式、大小

    struct MenuPage* ParentMenuPage;  // 指向父菜单页面的指针（用于返回上一级菜单）
    struct MenuItem* MenuItems;  // 指向当前页面菜单项的指针数组
    uint8_t LineSpace;  // 菜单项之间的垂直间距
    void (*_ShowAuxiliaryFunction)(void);  // 辅助显示函数指针，用于在菜单页面上显示附加信息

    Ui_Area List_MenuArea;  // 显示区域，定义了菜单显示的区域范围
    int DrawStart_X;  // 菜单开始绘制的X坐标（屏幕位置）
    int DrawStart_Y;  // 菜单开始绘制的Y坐标（屏幕位置）

    int8_t ActiveMenuID;  // 当前活动菜单的ID
    int8_t TargetMenuID;  // 目标菜单的ID
    Ui_Point StartPoint;  // 菜单开始绘制的起点位置（坐标点）
} MenuPage;

// 定义菜单组件属性结构体
typedef struct MenuItem{
    MenuItemType item_type;
    uint8_t item_id;
    char* item_name;
    char* text_content;
    int8_t start_x;
    int8_t start_y;
    const uint8_t* Tiles_Icon;  // 菜单项的图标（如果有的话），可能是一个字节数组（例如：图像数据）

    void (*General_callback)(void);  // 菜单项的回调函数，用户选择该项时调用的函数
    MenuPage* child_MenuPage;  // 当前菜单项对应的子菜单页面（如果有的话）
} MenuItem;

// 函数声明
uint8_t get_menu_list_length(const MenuItem* menu_item);
uint8_t get_font_height(void);

void dragon_ui_init(MenuPage* menu_page);
void dragon_ui_draw_item(const MenuPage* menu_page);
// Frame get_tar_frame(const uint8_t _item_id, const MenuItem* _item_name);
void update_option_frame(MenuPage* _menu_page, const int16_t total_steps, const uint32_t ui_refresh_speed);
void oled_draw_hvw_line(const uint8_t x, const uint8_t y, const uint8_t len, const uint8_t w, const uint8_t dir);

void switch_page(const MenuItemType flag);
void judge_item_to_edge_screen(void);

void u8g2_DrawArc1(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, float start_deg, float end_deg);
void u8g2_DrawButtonRFrame(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t flags, u8g2_uint_t text_width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, uint8_t radius);

// 全局变量声明
extern MenuPage* public_menu_page;
extern Font_Size page_font;

// 定义菜单项（MenuItem）数组
extern MenuItem home_item[];
extern MenuItem about_host_info_item[];
extern MenuItem setting_item[];
extern MenuItem mode_item[];
extern MenuItem language_item[];
extern MenuItem font_item[];
extern MenuItem test_item[];

extern MenuPage home_page;
extern MenuPage about_host_info_page;
extern MenuPage setting_page;
extern MenuPage mode_page;
extern MenuPage language_page;
extern MenuPage font_page;
extern MenuPage test_page;




#endif //DRAGON_UI_H
