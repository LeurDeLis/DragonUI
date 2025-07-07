#include "ec11.h"
volatile int16_t encoder_value = 0;
volatile int16_t last_encoder_value = 0;
volatile int16_t direction = 0; // 1:正转, -1:反转, 0:未动

/**
 * @brief 编码器编码值扫描
 * @note 无阻塞
 */
void Encoder_Scanf(void)
{
    encoder_value = (int16_t)__HAL_TIM_GET_COUNTER(&htim2) / 2;
    if(encoder_value > last_encoder_value)
    {
        // direction = 1; // 正转
        public_menu_page->TargetMenuID++;
        if(public_menu_page->MenuItems[public_menu_page->TargetMenuID].item_name == NULL) {
            public_menu_page->TargetMenuID = get_menu_list_length(public_menu_page->MenuItems) - 1;
        }

    }
    else if(encoder_value < last_encoder_value)
    {
        // direction = -1; // 反转
        public_menu_page->TargetMenuID--;
        if(public_menu_page->TargetMenuID < 0)
            // public_menu_page->TargetMenuID = get_menu_list_length(public_menu_page->MenuItems) - 1;
                public_menu_page->TargetMenuID = 0;
        // public_menu_page->TargetMenuID = 0;
    }
    // else
    // {
    //     direction = 0; // 未动
    // }

    last_encoder_value = encoder_value;
}

