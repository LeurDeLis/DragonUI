// //
// // Created by fukan on 25-6-9.
// //
//
// #include "key.h"
//
// // 定义按键状态常量
// #define KEY_PRESSED     1   // 表示按键被按下
// #define KEY_UNPRESSED   0   // 表示按键未被按下
//
// // 定义按键行为的时间阈值（单位：ms）
// #define KEY_TIME_LONG   2000    // 长按时间阈值
// #define KEY_TIME_REPEAT 100     // 连发间隔时间（长按后每隔多少毫秒触发一次连发）
//
// // 全局标志变量，用于记录当前按键事件（可被 main 或其他模块读取）
// uint8_t Key_Flag = 0;
//
// // 状态机相关静态变量（仅限本文件使用）
// static uint8_t CurrState = KEY_UNPRESSED;  // 当前按键状态
// static uint8_t PrevState = KEY_UNPRESSED;  // 上一次按键状态（用于边沿检测）
// static uint8_t S = 0;                       // 状态机状态
// static uint16_t Time = 0;                   // 时间计数器（用于判断长按、连发等）
//
// /**
//  * @brief 获取当前按键状态
//  * @return 返回按键是否被按下（KEY_PRESSED / KEY_UNPRESSED）
//  */
// uint8_t Key_GetState(void)
// {
//     // 使用HAL库函数读取ENC_SW按键的状态
//     if (HAL_GPIO_ReadPin(ENC_SW_GPIO_Port, ENC_SW_Pin) == GPIO_PIN_RESET)
//     {
//         return KEY_PRESSED;  // 如果按键接地并按下，则为低电平（RESET）
//     }
//     return KEY_UNPRESSED;
// }
//
// /**
//  * @brief 检查是否发生了某个按键事件
//  * @param Flag 要检查的事件标志（如 KEY_DOWN, KEY_LONG 等）
//  * @return 是否发生该事件（1表示发生，0表示未发生）
//  *
//  * 注意：除了 KEY_HOLD 外，其他事件在检查后会被清除。
//  */
// uint8_t Key_Check(const uint8_t Flag)
// {
//     if (Key_Flag & Flag)
//     {
//         if (Flag != KEY_HOLD)
//         {
//             Key_Flag &= ~Flag;  // 清除已处理的事件标志（KEY_HOLD 不清除）
//         }
//         return 1;
//     }
//     return 0;
// }
//
// /**
//  * @brief 主循环中定期调用此函数（建议每1ms调用一次）
//  *
//  * 功能：
//  * - 每隔约20ms采样一次按键状态
//  * - 更新按键事件标志
//  * - 实现按键状态机（单击、长按、连发等）
//  */
// void Key_Tick(void)
// {
//     // 记录上一次按键状态
//     PrevState = CurrState;
//
//     // 获取当前按键状态
//     CurrState = Key_GetState();
//
//     // 更新持续按下状态
//     if (CurrState == KEY_PRESSED)
//     {
//         Key_Flag |= KEY_HOLD;
//     }
//     else
//     {
//         Key_Flag &= ~KEY_HOLD;
//     }
//
//     // 检测按键按下事件（从释放变为按下）
//     if (CurrState == KEY_PRESSED && PrevState == KEY_UNPRESSED)
//     {
//         Key_Flag |= KEY_DOWN;
//     }
//
//     // 检测按键释放事件（从按下变为释放）
//     if (CurrState == KEY_UNPRESSED && PrevState == KEY_PRESSED)
//     {
//         Key_Flag |= KEY_UP;
//     }
//
//     // 状态机处理
//     switch (S)
//     {
//         // 初始状态：等待第一次按下
//         case 0:
//             if (CurrState == KEY_PRESSED)
//             {
//                 Time = KEY_TIME_LONG;  // 设置长按计时
//                 S = 1;                 // 进入状态1：等待长按或释放
//             }
//             break;
//
//             // 等待长按或释放
//         case 1:
//             if (CurrState == KEY_UNPRESSED)
//             {
//                 // 按键释放 -> 单击事件
//                 Key_Flag |= KEY_SINGLE;
//                 S = 0;  // 回到初始状态
//             }
//             else if (Time == 0)
//             {
//                 // 达到长按时间 -> 触发长按事件
//                 Time = KEY_TIME_REPEAT;      // 设置连发间隔
//                 Key_Flag |= KEY_LONG;
//                 S = 2;                       // 进入状态2：长按期间重复触发
//             }
//             break;
//
//             // 长按期间重复触发
//         case 2:
//             if (CurrState == KEY_UNPRESSED)
//             {
//                 // 按键释放 -> 回到初始状态
//                 S = 0;
//             }
//             else if (Time == 0)
//             {
//                 // 时间到 -> 再次触发连发事件
//                 Time = KEY_TIME_REPEAT;
//                 Key_Flag |= KEY_REPEAT;
//             }
//             break;
//
//         default: ;
//     }
// }

#include "key.h"

static const KeyHardware key_hw[KEY_COUNT] = {
    {ENC_S_GPIO_Port, ENC_S_Pin},
    {0},
    {0},
    {0}
};

static KeyState key_states[KEY_COUNT];

/**
 * @brief 获取某个按键的当前电平状态
 */
uint8_t Key_GetState(uint8_t key_id)
{
    if (key_id >= KEY_COUNT) return KEY_UNPRESSED;

    const GPIO_PinState pin_state = HAL_GPIO_ReadPin(key_hw[key_id].port, key_hw[key_id].pin);

    // 默认低电平有效
    return (pin_state == GPIO_PIN_RESET) ? KEY_PRESSED : KEY_UNPRESSED;
}

/**
 * @brief 检查某个按键是否产生了指定事件
 */
uint8_t Key_Check(uint8_t key_id, uint8_t flag)
{
    if (key_id >= KEY_COUNT) return 0;

    if (key_states[key_id].flag & flag) {
        if (flag != KEY_HOLD) {
            key_states[key_id].flag &= ~flag;
        }
        return 1;
    }
    return 0;
}

/**
 * @brief 主循环中定期调用（建议每 1ms）
 */
void Key_Tick(void)
{
    uint32_t now = HAL_GetTick();

    for (uint8_t i = 0; i < KEY_COUNT; i++) {
        KeyState* k = &key_states[i];
        k->prev_state = k->curr_state;
        k->curr_state = Key_GetState(i);

        // 更新按下状态
        if (k->curr_state == KEY_PRESSED) k->flag |= KEY_HOLD;
        else k->flag &= ~KEY_HOLD;

        // 检测按下/释放边沿
        if (k->curr_state == KEY_PRESSED && k->prev_state == KEY_UNPRESSED)
            k->flag |= KEY_DOWN;

        if (k->curr_state == KEY_UNPRESSED && k->prev_state == KEY_PRESSED)
            k->flag |= KEY_UP;

        switch (k->s) {
            case 0: // 初始状态
                if (k->curr_state == KEY_PRESSED) {
                    k->last_time = now;
                    k->s = 1;
                }
                break;

            case 1: // 判断是否为长按或等待释放
                if (k->curr_state == KEY_UNPRESSED) {
                    k->last_time = now;
                    k->s = 2; // 可能是单击，等待双击窗口
                } else if ((now - k->last_time) >= KEY_TIME_LONG) {
                    k->flag |= KEY_LONG;
                    k->last_time = now;
                    k->s = 4;
                }
                break;

            case 2: // 双击检测窗口
                if (k->curr_state == KEY_PRESSED) {
                    k->flag |= KEY_DOUBLE;
                    k->s = 3;
                } else if ((now - k->last_time) >= KEY_TIME_DOUBLE) {
                    k->flag |= KEY_SINGLE;
                    k->s = 0;
                }
                break;

            case 3: // 双击后等待释放
                if (k->curr_state == KEY_UNPRESSED) {
                    k->s = 0;
                }
                break;

            case 4: // 长按连发状态
                if (k->curr_state == KEY_UNPRESSED) {
                    k->s = 0;
                } else if ((now - k->last_time) >= KEY_TIME_REPEAT) {
                    k->last_time = now;
                    k->flag |= KEY_REPEAT;
                }
                break;
        }
    }
}

