#ifndef __EC11_H
#define __EC11_H

#include "main.h"
#include "tim.h"

// 获取按键状态
#define GET_SW_STATE HAL_GPIO_ReadPin(ENC_S_GPIO_Port, ENC_S_Pin)


void Encoder_Scanf(void);
uint8_t Encoder_Sw_Down(void);//�������Ƿ���

extern volatile int16_t encoder_value;
//extern volatile int16_t last_encoder_value;
//extern volatile int16_t direction; // 1:��ת, -1:��ת, 0:δ�� 

#endif
