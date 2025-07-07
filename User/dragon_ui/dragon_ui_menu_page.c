//
// Created by fukan on 25-6-14.
//
#include "dragon_ui.h"

MenuPage* public_menu_page = NULL;
// Font_Size page_font = font_nor;

// 定义菜单项（MenuItem）数组
MenuItem home_item[] = {
    {.item_type = MENU_ITEM_TYPE_SETTING, .item_id = 0, .item_name = "菜单设置", .child_MenuPage = &setting_page},
    {.item_type = MENU_ITEM_TYPE_SETTING, .item_id = 1, .item_name = "模式设置", .child_MenuPage = &mode_page},
    {.item_type = MENU_ITEM_TYPE_SETTING, .item_id = 2, .item_name = "语言设置", .child_MenuPage = &language_page},
    {.item_type = MENU_ITEM_TYPE_SETTING, .item_id = 3, .item_name = "字体设置", .child_MenuPage = &font_page},
    {.item_type = MENU_ITEM_TYPE_SETTING, .item_id = 4, .item_name = "测试页面A", .child_MenuPage = &test_page},
    {.item_type = MENU_ITEM_TYPE_SETTING, .item_id = 5, .item_name = "测试页面B", .child_MenuPage = &test_page},
    {.item_name = NULL}
};

MenuItem setting_item[] = {
	{.item_type = MENU_ITEM_TYPE_NORMAL, .item_id = 0, .item_name = "屏幕", },
	{.item_type = MENU_ITEM_TYPE_SETTING, .item_id = 1, .item_name = "曝光",},
	{.item_type = MENU_ITEM_TYPE_SETTING, .item_id = 2, .item_name = "按键",},
	{.item_type = MENU_ITEM_TYPE_SHOW_INFO, .item_id = 3, .item_name = "关于本机", .child_MenuPage = &about_host_info_page},
	{.item_name = NULL}
};

MenuItem about_host_info[] = {
	{.item_type = MENU_ITEM_TYPE_SHOW_INFO,
		.item_id = 0,
		.text_content = "STM32F103C8T6",
	},
	{.item_name = NULL}
};

MenuItem mode_item[] = {
	{MENU_ITEM_TYPE_SETTING, .item_id = 0, .item_name = "黑色"},
	{MENU_ITEM_TYPE_SETTING, .item_id = 1, .item_name = "白色"},
	{.item_name = NULL}
};

MenuItem language_item[] = {
    {MENU_ITEM_TYPE_SETTING, .item_id = 0, .item_name = "中文"},
    {MENU_ITEM_TYPE_SETTING, .item_id = 1, .item_name = "English"},
    {.item_name = NULL}
};

MenuItem font_item[] = {
    {MENU_ITEM_TYPE_SETTING, .item_id = 0, .item_name = "微软雅黑超大"},
    {MENU_ITEM_TYPE_SETTING, .item_id = 1, .item_name = "微软雅黑常规"},
    {MENU_ITEM_TYPE_SETTING, .item_id = 2, .item_name = "微软雅黑超小"},
    {MENU_ITEM_TYPE_SETTING, .item_id = 3, .item_name = "微软雅黑迷你"},
    {.item_name = NULL}
};

MenuItem test_item[] = {
	{MENU_ITEM_TYPE_SETTING, .item_id = 0, .item_name = "测试1"},
	{MENU_ITEM_TYPE_SETTING, .item_id = 1, .item_name = "测试2"},
	{MENU_ITEM_TYPE_SETTING, .item_id = 2, .item_name = "测试3"},
	{MENU_ITEM_TYPE_SETTING, .item_id = 3, .item_name = "测试4"},
	{MENU_ITEM_TYPE_SETTING, .item_id = 4, .item_name = "测试5"},
	{MENU_ITEM_TYPE_SETTING, .item_id = 5, .item_name = "测试6"},
	{.item_name = NULL}
};


MenuPage home_page = {
    .MenuType = UI_MODE_MENU,
    .MovingSpeed = ANIMATION_SPEED_MEDIUM,  // 光标或选择框移动的速度（例如：快、慢）
    .CursorStyle = 0, // 光标样式（例如：箭头、块状光标等）
    .MoveStyle = ANIMATION_STYLE_SMOOTH,// 移动样式（例如：平滑移动、跳跃移动等）
    .moving_frame = {0},
    .TextFontStyle = font_nor, // 菜单文本的字体样式、大小
	.roll_offset = 0,

    .ParentMenuPage = NULL,  // 指向父菜单页面的指针（用于返回上一级菜单）
    .MenuItems = home_item,  // 指向当前页面菜单项的指针数组
    .LineSpace = 8,  // 菜单项之间的垂直间距
    NULL,  // 辅助显示函数指针，用于在菜单页面上显示附加信息

    .List_MenuArea = {128, 64}, // 显示区域，定义了菜单显示的区域范围
    .DrawStart_X = 0,  // 菜单开始绘制的X坐标
    .DrawStart_Y = 0,  // 菜单开始绘制的Y坐标

    .ActiveMenuID = 0,  // 当前活动菜单的ID
    .TargetMenuID = 0,  // 目标菜单的ID

    .StartPoint = {0, 0} // 菜单开始绘制的起点位置（坐标点）
};

MenuPage setting_page = {
	.MenuType = UI_MODE_MENU,
	.MovingSpeed = ANIMATION_SPEED_MEDIUM,  // 光标或选择框移动的速度（例如：快、慢）
	.CursorStyle = 0, // 光标样式（例如：箭头、块状光标等）
	.MoveStyle = ANIMATION_STYLE_SMOOTH,// 移动样式（例如：平滑移动、跳跃移动等）
	.moving_frame = {0},
	.TextFontStyle = font_nor, // 菜单文本的字体样式、大小
	.roll_offset = 0,

	.ParentMenuPage = &home_page,  // 指向父菜单页面的指针（用于返回上一级菜单）
	.MenuItems = setting_item,  // 指向当前页面菜单项的指针数组
	.LineSpace = 8,  // 菜单项之间的垂直间距
	NULL,  // 辅助显示函数指针，用于在菜单页面上显示附加信息

	.List_MenuArea = {128, 64}, // 显示区域，定义了菜单显示的区域范围
	.DrawStart_X = 0,  // 菜单开始绘制的X坐标
	.DrawStart_Y = 0,  // 菜单开始绘制的Y坐标

	.ActiveMenuID = 0,  // 当前活动菜单的ID
	.TargetMenuID = 0,  // 目标菜单的ID

	.StartPoint = {0, 0} // 菜单开始绘制的起点位置（坐标点）
};

MenuPage about_host_info_page = {
	.MenuType = UI_MODE_MENU,
	.TextFontStyle = font_nor, // 菜单文本的字体样式、大小
	.ParentMenuPage = &setting_page,  // 指向父菜单页面的指针（用于返回上一级菜单）
	.MenuItems = about_host_info,  // 指向当前页面菜单项的指针数组
};

MenuPage mode_page = {
	.MenuType = UI_MODE_MENU,
	.MovingSpeed = ANIMATION_SPEED_MEDIUM,  // 光标或选择框移动的速度（例如：快、慢）
	.CursorStyle = 0, // 光标样式（例如：箭头、块状光标等）
	.MoveStyle = ANIMATION_STYLE_SMOOTH,// 移动样式（例如：平滑移动、跳跃移动等）
	.moving_frame = {0},
	.TextFontStyle = font_nor, // 菜单文本的字体样式、大小
	.roll_offset = 0,

	.ParentMenuPage = &home_page,  // 指向父菜单页面的指针（用于返回上一级菜单）
	.MenuItems = mode_item,  // 指向当前页面菜单项的指针数组
	.LineSpace = 8,  // 菜单项之间的垂直间距
	NULL,  // 辅助显示函数指针，用于在菜单页面上显示附加信息

	.List_MenuArea = {128, 64}, // 显示区域，定义了菜单显示的区域范围
	.DrawStart_X = 0,  // 菜单开始绘制的X坐标
	.DrawStart_Y = 0,  // 菜单开始绘制的Y坐标

	.ActiveMenuID = 0,  // 当前活动菜单的ID
	.TargetMenuID = 0,  // 目标菜单的ID

	.StartPoint = {0, 0} // 菜单开始绘制的起点位置（坐标点）
};

MenuPage language_page = {
    .MenuType = UI_MODE_SUBMENU,
    .MovingSpeed = ANIMATION_SPEED_MEDIUM,  // 光标或选择框移动的速度（例如：快、慢）
    .CursorStyle = 0, // 光标样式（例如：箭头、块状光标等）
    .moving_frame = {0},
    .MoveStyle = ANIMATION_STYLE_SMOOTH,// 移动样式（例如：平滑移动、跳跃移动等）
    .TextFontStyle = font_nor, // 菜单文本的字体样式、大小
	.roll_offset = 0,

    .ParentMenuPage = &home_page,  // 指向父菜单页面的指针（用于返回上一级菜单）
    .MenuItems = language_item,  // 指向当前页面菜单项的指针数组
    .LineSpace = 8,  // 菜单项之间的垂直间距
    NULL,  // 辅助显示函数指针，用于在菜单页面上显示附加信息

    .List_MenuArea = {128, 64}, // 显示区域，定义了菜单显示的区域范围
    .DrawStart_X = 0,  // 菜单开始绘制的X坐标
    .DrawStart_Y = 0,  // 菜单开始绘制的Y坐标

    .ActiveMenuID = 0,  // 当前活动菜单的ID
    .TargetMenuID = 0,  // 目标菜单的ID
    .StartPoint = {0, 0} // 菜单开始绘制的起点位置（坐标点）
};


MenuPage font_page = {
	.MenuType = UI_MODE_SUBMENU,
	.MovingSpeed = ANIMATION_SPEED_MEDIUM,  // 光标或选择框移动的速度（例如：快、慢）
	.CursorStyle = 0, // 光标样式（例如：箭头、块状光标等）
	.moving_frame = {0},
	.MoveStyle = ANIMATION_STYLE_SMOOTH,// 移动样式（例如：平滑移动、跳跃移动等）
	.TextFontStyle = font_nor, // 菜单文本的字体样式、大小
	.roll_offset = 0,

	.ParentMenuPage = &home_page,  // 指向父菜单页面的指针（用于返回上一级菜单）
	.MenuItems = font_item,  // 指向当前页面菜单项的指针数组
	.LineSpace = 8,  // 菜单项之间的垂直间距
	NULL,  // 辅助显示函数指针，用于在菜单页面上显示附加信息

	.List_MenuArea = {128, 64}, // 显示区域，定义了菜单显示的区域范围
	.DrawStart_X = 0,  // 菜单开始绘制的X坐标
	.DrawStart_Y = 0,  // 菜单开始绘制的Y坐标

	.ActiveMenuID = 0,  // 当前活动菜单的ID
	.TargetMenuID = 0,  // 目标菜单的ID
	.StartPoint = {0, 0} // 菜单开始绘制的起点位置（坐标点）
};


MenuPage test_page = {
	.MenuType = UI_MODE_SUBMENU,
	.MovingSpeed = ANIMATION_SPEED_MEDIUM,  // 光标或选择框移动的速度（例如：快、慢）
	.CursorStyle = 0, // 光标样式（例如：箭头、块状光标等）
	.moving_frame = {0},
	.MoveStyle = ANIMATION_STYLE_SMOOTH,// 移动样式（例如：平滑移动、跳跃移动等）
	.TextFontStyle = font_nor, // 菜单文本的字体样式、大小
	.roll_offset = 0,

	.ParentMenuPage = &home_page,  // 指向父菜单页面的指针（用于返回上一级菜单）
	.MenuItems = test_item,  // 指向当前页面菜单项的指针数组
	.LineSpace = 8,  // 菜单项之间的垂直间距
	NULL,  // 辅助显示函数指针，用于在菜单页面上显示附加信息

	.List_MenuArea = {128, 64}, // 显示区域，定义了菜单显示的区域范围
	.DrawStart_X = 0,  // 菜单开始绘制的X坐标
	.DrawStart_Y = 0,  // 菜单开始绘制的Y坐标

	.ActiveMenuID = 0,  // 当前活动菜单的ID
	.TargetMenuID = 0,  // 目标菜单的ID
	.StartPoint = {0, 0} // 菜单开始绘制的起点位置（坐标点）
};