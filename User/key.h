// //
// // Created by fukan on 25-6-9.
// //
//
// #ifndef KEY_H
// #define KEY_H
//
// #include "main.h"
//
// // 按键事件标志位（可用于按位操作）
// #define KEY_HOLD    0x80    // 持续按下
// #define KEY_DOWN    0x40    // 按键按下（下降沿）
// #define KEY_UP      0x20    // 按键释放（上升沿）
// #define KEY_SINGLE  0x10    // 单击事件
// #define KEY_LONG    0x08    // 长按事件
// #define KEY_REPEAT  0x04    // 连发事件（长按后重复触发）
// // 下面两个保留（可扩展双击/三击功能）
// #define KEY_DOUBLE  0x02
// #define KEY_TRIPLE  0x01
//
// // 函数声明
// void Key_Init(void);                  // 初始化按键GPIO（如果需要）
// uint8_t Key_GetState(void);           // 获取当前按键状态
// uint8_t Key_Check(const uint8_t Flag); // 检查是否有指定按键事件发生
// void Key_Tick(void);                  // 每1ms调用一次，用于更新按键状态
//
// #endif //KEY_H



#ifndef KEY_H
#define KEY_H

#include "main.h"

#define KEY_COUNT       4   // 按键数量（根据需要调整）

// 按键事件标志
#define KEY_HOLD        0x80
#define KEY_DOWN        0x40
#define KEY_UP          0x20
#define KEY_SINGLE      0x10
#define KEY_DOUBLE      0x08
#define KEY_LONG        0x04
#define KEY_REPEAT      0x02

// 状态定义
#define KEY_PRESSED     1
#define KEY_UNPRESSED   0

#define KEY_TIME_LONG   2000
#define KEY_TIME_REPEAT 100
#define KEY_TIME_DOUBLE 200


// 多个按键硬件映射（实际引脚请按需修改）
typedef struct {
	GPIO_TypeDef* port;
	uint16_t pin;
} KeyHardware;

typedef enum {
	KEY_0 = 0,
	KEY_1,
	KEY_2,
	KEY_3
} Key_t;

// 按键内部状态结构
typedef struct {
	uint8_t curr_state;
	uint8_t prev_state;
	uint8_t s;              // 状态机状态
	uint32_t last_time;     // 状态机状态切换起始时间
	uint8_t flag;           // 事件标志
} KeyState;


// 初始化与接口
void Key_Init(void);
void Key_Tick(void);
uint8_t Key_Check(uint8_t key_id, uint8_t flag);
uint8_t Key_GetState(uint8_t key_id);

#endif
