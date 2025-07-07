//
// Created by fukan on 25-6-4.
//

#ifndef OLED_CMD_H
#define OLED_CMD_H

#include "main.h"

// 定义OLED初始化命令结构体
typedef struct
{
    uint8_t cmd;      // 命令字节
    const char* desc; // 命令描述
} OLED_Init_Command;

extern const OLED_Init_Command oled_init_commands[27];

#endif //OLED_CMD_H
