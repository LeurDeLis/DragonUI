#include "dragon_ui.h"

// 获取菜单列表的长度
/*
 * @breif 获取菜单列表的长度
 * @param menu_item MenuItem类型的菜单项数组
 */
uint8_t get_menu_list_length(const MenuItem* menu_item) {
    uint16_t i = 0;
    for (i = 0; menu_item[i].item_name != NULL; ++i) {}
    return i;
}

// 获取字体高度
/*
 * @breif 获取字体高度
 * @param menu_page MenuList* 类型的菜单列表指针
 */
uint8_t get_font_height(void) {
    uint8_t font_height = 0;
    switch (public_menu_page->TextFontStyle)
    {
        case font_max:
            font_height = 16;
            break;
        case font_nor:
            font_height = 12;
            break;
    	case font_mid:
    		font_height = 10;
    		break;
        case font_min:
            font_height = 8;
            break;
        default:
    		break;
    }
    return font_height;
}

// 获取选项框目标位置
ButtonFrame get_tar_button_frame(const MenuItem* _item_name) {
	ButtonFrame _button_frame = {0};
	// 设置选项框的坐标、属性
	_button_frame.x = _item_name->start_x;
	_button_frame.y = _item_name->start_y - 1;
	_button_frame.flags = U8G2_BTN_BW1 | U8G2_BTN_HCENTER;
	_button_frame.w = oled_get_UTF8_width(_item_name->item_name);
	_button_frame.ph = 2;
	_button_frame.pv = public_menu_page->LineSpace / 2 - 1;
	_button_frame.r = FRAME_R;

	return _button_frame;
}

void oled_draw_hvw_line(const uint8_t x, const uint8_t y, const uint8_t len, const uint8_t w, const uint8_t dir) {
	if (dir == 0) {
		for (uint8_t i = 0; i < w; ++i)
			oled_draw_H_line(x, y + i, len);
	} else if (dir == 1) {
		for (uint8_t i = 0; i < w; ++i)
			oled_draw_V_line(x + i, y, len);
	}
}

/**
 * @brief 初始化 UI 界面，设置每个菜单项的动画帧参数
 *
 * @param menu_page MenuList* 类型的菜单列表指针
 */
void dragon_ui_init(MenuPage* menu_page)
{
	// char str[20] = {0};
    public_menu_page = menu_page;
	uint8_t font_height = get_font_height();
    MenuItem* item = public_menu_page->MenuItems;
	public_menu_page->roll_offset = 0;
	// 获取菜单项之间的坐标偏移
    int item_offset = (int)public_menu_page->LineSpace + font_height;

    for(uint8_t i = 0; item[i].item_name != NULL; ++i)
    {
        // 设置菜单项的起始坐标
        item[i].start_x = ITEM_START_X;
        item[i].start_y = item_offset - public_menu_page->LineSpace / 2 + i * item_offset + public_menu_page->roll_offset;
        // 在指定坐标上绘制字符串
        oled_draw_UTF8(item[i].start_x, item[i].start_y, item[i].item_name);

    }
    // 设置选项框的坐标、属性
    menu_page->moving_button_frame = get_tar_button_frame(&item[0]);
    // 绘制选项框
    oled_draw_button_frame(
    	menu_page->moving_button_frame.x,
    	menu_page->moving_button_frame.y,
    	menu_page->moving_button_frame.flags,
    	menu_page->moving_button_frame.w,
    	menu_page->moving_button_frame.ph,
    	menu_page->moving_button_frame.pv
    	);
	oled_draw_hvw_line(124, 0, 64, 2, 1);
	oled_draw_box(122, menu_page->moving_button_frame.y - font_height + menu_page->moving_button_frame.pv + 1, 6, font_height - 1);
    oled_send_buffer();
}

/**
 * @brief 绘制整个菜单界面及当前选中项的高亮框
 *
 * @param menu_page MenuList* 类型的菜单列表指针
 */
void dragon_ui_draw_item(const MenuPage* menu_page)
{
	MenuItem* _item = menu_page->MenuItems;
	if (_item->item_type == MENU_ITEM_TYPE_SHOW_INFO) {
		oled_draw_UTF8(0, 32, _item->text_content);
	}
	if (_item->item_type == MENU_ITEM_TYPE_SETTING || _item->item_type == MENU_ITEM_TYPE_NORMAL) {
		judge_item_to_edge_screen();
		update_option_frame(public_menu_page, AnimationTotalStep, public_menu_page->MovingSpeed);
		const uint8_t font_height = get_font_height();
		const int item_offset = public_menu_page->LineSpace + font_height;
		// 循环绘制每个菜单项的文字内容
		for (uint8_t i = 0; _item[i].item_name != NULL; ++i) {
			// 在指定坐标上绘制字符串
			_item[i].start_x = ITEM_START_X;
			_item[i].start_y = item_offset - public_menu_page->LineSpace / 2 + i * item_offset + public_menu_page->roll_offset;
			oled_draw_UTF8(_item[i].start_x, _item[i].start_y, _item[i].item_name);
		}
		oled_draw_hvw_line(124, 0, 64, 2, 1);
	}

	char str[20] = {0};
	memset(str, 0, sizeof(str));
	sprintf(str, "%d", public_menu_page->ActiveMenuID);
	oled_draw_str(100, 24, str);
	// 发送缓存区
	oled_send_buffer();
	// 清空 OLED 显示缓冲区
	oled_clear_buffer();
}

	/**
 * @brief 更新选项框的位置和大小，实现平滑动画效果（无阻塞）
 *
 * @param _menu_page MenuPage* 类型的菜单页面指针
 * @param total_steps 动画总步数
 * @param ui_refresh_speed 动画刷新间隔（单位：ms）
 */

void update_option_frame(MenuPage* _menu_page, const int16_t total_steps, const uint32_t ui_refresh_speed)
{
    static uint32_t last_update_time = 0;
    const uint32_t current_time = get_ticks();
	const uint8_t text_h = get_font_height();

    // 如果还没到刷新时间，跳过更新
    if (current_time - last_update_time < ui_refresh_speed || _menu_page->ActiveMenuID == _menu_page->TargetMenuID) {
        const ButtonFrame _frame = _menu_page->moving_button_frame;
    	oled_draw_button_frame(
			_frame.x,
			_frame.y,
			_frame.flags,
			_frame.w,
			_frame.ph,
			_frame.pv
		);
    	// char str[20] = {0};
    	// sprintf(str, "%d", _frame.y);
    	// oled_draw_UTF8(100, 48, str);

    	oled_draw_box(122, _frame.y - text_h + _frame.pv + 1, 6, text_h - 1);
        return;
    }

    // 获取目标帧和当前移动帧
    const ButtonFrame _tar_frame = get_tar_button_frame(&_menu_page->MenuItems[_menu_page->TargetMenuID]);
    ButtonFrame* _frame = &_menu_page->moving_button_frame;

    // 计算剩余变化量
    const int32_t dy = _tar_frame.y - _frame->y;
    const int32_t dw = _tar_frame.w - _frame->w;

    // 如果已经到位，直接设置为目标帧并返回
    if (dy == 0 && dw == 0) {
        _menu_page->ActiveMenuID = _menu_page->TargetMenuID;
    	oled_draw_button_frame(
			_tar_frame.x,
			_tar_frame.y,
			_tar_frame.flags,
			_tar_frame.w,
			_tar_frame.ph,
			_tar_frame.pv
		);
    	oled_draw_box(122, _tar_frame.y - text_h + _tar_frame.pv + 1, 6, text_h - 1);
        return;
    }

    // 第一次进入动画时，记录剩余步数
    static int16_t remaining_steps = 0;
    if (remaining_steps <= 0) {
        remaining_steps = total_steps;
    }

    // 计算每步的变化量（使用定点数模拟除法）
    const int32_t step_y = (dy + (dy >= 0 ? remaining_steps / 2 : -remaining_steps / 2)) / remaining_steps;
    const int32_t step_w = (dw + (dw >= 0 ? remaining_steps / 2 : -remaining_steps / 2)) / remaining_steps;

    // 更新位置
    _frame->y += step_y;
    _frame->w += step_w;

    // 防止越过目标值
    if ((step_y >= 0 && _frame->y >= _tar_frame.y) || (step_y < 0 && _frame->y <= _tar_frame.y)) {
        _frame->y = _tar_frame.y;
    }
    if ((step_w >= 0 && _frame->w >= _tar_frame.w) || (step_w < 0 && _frame->w <= _tar_frame.w)) {
        _frame->w = _tar_frame.w;
    }

    // 绘制当前高亮框
	oled_draw_button_frame(
		_frame->x,
		_frame->y,
		_frame->flags,
		_frame->w,
		_frame->ph,
		_frame->pv
	);

	oled_draw_box(122, _frame->y - text_h + _frame->pv + 1, 6, text_h - 1);

    // 减少剩余步数
    remaining_steps--;

    // 如果步数走完或已到达目标，更新当前菜单项ID
    if (remaining_steps <= 0 || (_frame->y == _tar_frame.y && _frame->w == _tar_frame.w)) {
        _menu_page->ActiveMenuID = _menu_page->TargetMenuID;
        _menu_page->moving_button_frame = get_tar_button_frame(&_menu_page->MenuItems[_menu_page->ActiveMenuID]);
        remaining_steps = 0; // 重置剩余步数
    }

    // 更新上次更新时间
    last_update_time = current_time;
}

// 切换页面函数
void switch_page(const MenuItemType flag) {
	const MenuItem _item = public_menu_page->MenuItems[public_menu_page->ActiveMenuID];
	switch (flag)
	{
		case ui_next_page:
			if (_item.item_type == MENU_ITEM_TYPE_SETTING ||  _item.item_type == MENU_ITEM_TYPE_SHOW_INFO && _item.child_MenuPage != NULL)
			{
				public_menu_page = _item.child_MenuPage;
				dragon_ui_init(public_menu_page);
			}
			break;

		case ui_pre_page:
			if (public_menu_page->ParentMenuPage != NULL)
			{
				public_menu_page = public_menu_page->ParentMenuPage;
			}
			break;
		default:
			// 可以加入日志或断点提示未知 flag 值
			break;
	}
}

// 判断列表的目标项是否超过屏幕顶端和底部
void judge_item_to_edge_screen(void)
{
	const int item_h = get_font_height() + public_menu_page->LineSpace;
	const uint8_t half_line_space = public_menu_page->LineSpace / 2;
	const int screen_top = get_font_height() + half_line_space;
	const int screen_bottom = OLED_HEIGHT - half_line_space;

	const int item_y = public_menu_page->MenuItems[public_menu_page->TargetMenuID].start_y;

	int offset = public_menu_page->roll_offset;

	// 如果目标项超出屏幕底部
	if (item_y > screen_bottom) {
		offset -= item_y - screen_bottom + half_line_space;
	}
	// 如果目标项超出屏幕顶部
	else if (item_y < screen_top ) {
		offset += screen_top + item_h - item_y + 1;
	}
	// 平滑滚动（每帧调整一个像素或几像素）
	int max_step = half_line_space + 1;
	int delta = offset - public_menu_page->roll_offset;
	if (delta > max_step) delta = max_step;
	if (delta < -max_step) delta = -max_step;

	public_menu_page->roll_offset += delta;
}





//
// /* enable rounded corners */
// #define U8G2_BTN_ROUND 0x80
//
// #ifndef M_PI
// #define M_PI 3.14159265358979323846
// #endif
//
// static float normalize_angle(float angle_deg)
// {
//   while (angle_deg < 0.0f)
//     angle_deg += 360.0f;
//   while (angle_deg >= 360.0f)
//     angle_deg -= 360.0f;
//   return angle_deg;
// }
//
// static uint8_t is_angle_in_range(float angle, float start, float end)
// {
//   if (start < end)
//     return angle >= start && angle <= end;
//   return angle >= start || angle <= end;
//   // 跨越 360°
// }
//
// static void u8g2_draw_arc_deg(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, float start_deg, float end_deg)
// {
//   u8g2_int_t x = 0;
//   u8g2_int_t y = rad;
//   u8g2_int_t d = rad - 1;
//
//   float angle;
//   start_deg = normalize_angle(start_deg);
//   end_deg = normalize_angle(end_deg);
//
//   while (y >= x)
//   {
//     // 计算每个象限点的真实角度
//     struct { int dx, dy; } dirs[8] = {
//       { +y, -x }, { +x, -y }, { -x, -y }, { -y, -x },
//       { -y, +x }, { -x, +y }, { +x, +y }, { +y, +x }
//     };
//
//     float angles[8] = {
//       atan2f(-x, +y), atan2f(-y, +x), atan2f(-y, -x), atan2f(-x, -y),
//       atan2f(+x, -y), atan2f(+y, -x), atan2f(+y, +x), atan2f(+x, +y)
//     };
//
//     for (int i = 0; i < 8; i++)
//     {
//       angle = angles[i] * 180.0f / M_PI;
//       angle = normalize_angle(angle);
//       if (is_angle_in_range(angle, start_deg, end_deg))
//         u8g2_DrawPixel(u8g2, x0 + dirs[i].dx, y0 + dirs[i].dy);
//     }
//
//     // 中点圆算法更新点
//     if (d >= 2 * x)
//     {
//       d = d - 2 * x - 1;
//       x = x + 1;
//     }
//     else if (d < 2 * (rad - y))
//     {
//       d = d + 2 * y - 1;
//       y = y - 1;
//     }
//     else
//     {
//       d = d + 2 * (y - x - 1);
//       y = y - 1;
//       x = x + 1;
//     }
//   }
// }
// /**
//  * 使用 u8g2_DrawPixel 绘制一段圆弧
//  * @param u8g2   U8g2 上下文指针
//  * @param x0     圆心 X 坐标
//  * @param y0     圆心 Y 坐标
//  * @param rad 半径
//  * @param start_deg 起始角度（-360~360）
//  * @param end_deg   结束角度（-360~360）
//  */
// void u8g2_DrawArc1(u8g2_t *u8g2, u8g2_uint_t x0, u8g2_uint_t y0, u8g2_uint_t rad, float start_deg, float end_deg)
// {
// #ifdef U8G2_WITH_INTERSECTION
//   if (u8g2_IsIntersection(u8g2, x0 - rad, y0 - rad, x0 + rad + 1, y0 + rad + 1) == 0)
//     return;
// #endif
//
//   u8g2_draw_arc_deg(u8g2, x0, y0, rad, start_deg, end_deg);
// }
//
//
//
// void u8g2_DrawButtonRFrame(u8g2_t *u8g2, u8g2_uint_t x, u8g2_uint_t y, u8g2_uint_t flags, u8g2_uint_t text_width, u8g2_uint_t padding_h, u8g2_uint_t padding_v, uint8_t radius)
// {
//   u8g2_uint_t w = text_width;
//
//   u8g2_uint_t xx, yy, ww, hh;
//
//   u8g2_uint_t gap_frame = U8G2_BTN_BW_MASK+1;
//
//   u8g2_uint_t border_width = flags & U8G2_BTN_BW_MASK;
//
//   int8_t a = u8g2_GetAscent(u8g2);
//   int8_t d = u8g2_GetDescent(u8g2);
//
//   uint8_t color_backup = u8g2->draw_color;
//
//   // 新增：判断是否为圆角按钮
//   uint8_t is_rounded = (flags & U8G2_BTN_ROUND) ? 1 : 0;
//   // 可选：自定义圆角半径
//   // u8g2_uint_t radius = 4;  // 默认 4 像素圆角
//
//   if ( flags & U8G2_BTN_XFRAME )
//   {
//     border_width++;
//     gap_frame = border_width;
//     border_width++;
//   }
//
//   for(;;)
//   {
//
//     xx = x;
//     xx -= padding_h;
//     xx -= border_width;
//     ww = w+2*padding_h+2*border_width;
//
//     yy = y;
//     yy += u8g2->font_calc_vref(u8g2);
//     yy -= a;
//     yy -= padding_v;
//     yy -= border_width;
//     hh = a-d+2*padding_v+2*border_width;
//     if ( border_width == 0 )
//       break;
//     if ( border_width == gap_frame )
//     {
//       u8g2_SetDrawColor(u8g2, color_backup == 0 ? 1 : 0);
//     }
//
//     // 使用圆角绘制
//     if (is_rounded)
//     {
//         u8g2_DrawRFrame(u8g2, xx, yy, ww, hh, radius);
//     }
//     else
//     {
//         u8g2_DrawFrame(u8g2, xx, yy, ww, hh);
//     }
//
//     u8g2_SetDrawColor(u8g2, color_backup);
//
//     if ( flags & U8G2_BTN_SHADOW_MASK )
//     {
//       if ( border_width == (flags & U8G2_BTN_BW_MASK) )
//       {
//         u8g2_uint_t i;
//         u8g2_uint_t shadow_gap = (flags & U8G2_BTN_SHADOW_MASK) >> U8G2_BTN_SHADOW_POS;
//         shadow_gap--;
//         for( i = 0; i < border_width; i++ )
//         {
//           u8g2_DrawHLine(u8g2, xx+border_width+shadow_gap,yy+hh+i+shadow_gap,ww);
//           u8g2_DrawVLine(u8g2, xx+ww+i+shadow_gap,yy+border_width+shadow_gap,hh);
//
//           u8g2_DrawArc(u8g2, xx+ww+i+shadow_gap, yy+hh+i+shadow_gap, radius, 0, 90);
//         }
//       }
//     }
//     border_width--;
//   } /* for */
//
//   if ( flags & U8G2_BTN_INV )
//   {
//     u8g2_SetDrawColor(u8g2, 2);         /* XOR */
//     if (is_rounded)
//     {
//         u8g2_DrawRBox(u8g2, xx, yy, ww, hh, radius);
//     }
//     else
//     {
//         u8g2_DrawBox(u8g2, xx, yy, ww, hh);
//     }
//     u8g2_SetDrawColor(u8g2, color_backup);
//   }
// }
//
//
//
