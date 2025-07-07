#ifndef __OLED_DRIVER_H
#define __OLED_DRIVER_H



#include "stm32f10x.h"         
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <stdarg.h>
#include "stdio.h"
//ʹ�ú궨�壬�ٶȸ��죨�Ĵ�����ʽ��
#define OLED_SCL_Clr()  (GPIOB->BRR = GPIO_Pin_8)   // ��λ SCL (�� GPIOB �� 8 ����������)
#define OLED_SCL_Set()  (GPIOB->BSRR = GPIO_Pin_8)  // ��λ SCL (�� GPIOB �� 8 ����������)

#define OLED_SDA_Clr()  (GPIOB->BRR = GPIO_Pin_9)   // ��λ SDA (�� GPIOB �� 9 ����������)
#define OLED_SDA_Set()  (GPIOB->BSRR = GPIO_Pin_9)  // ��λ SDA (�� GPIOB �� 9 ����������)

#define OLED_RES_Clr()  (GPIOB->BRR = GPIO_Pin_5)   // ��λ RES (�� GPIOB �� 5 ����������)
#define OLED_RES_Set()  (GPIOB->BSRR = GPIO_Pin_5)  // ��λ RES (�� GPIOB �� 5 ����������)

#define OLED_DC_Clr()   (GPIOB->BRR = GPIO_Pin_6)   // ��λ DC (�� GPIOB �� 6 ����������)
#define OLED_DC_Set()   (GPIOB->BSRR = GPIO_Pin_6)  // ��λ DC (�� GPIOB �� 6 ����������)

#define OLED_CS_Clr()   (GPIOB->BRR = GPIO_Pin_7)   // ��λ CS (�� GPIOB �� 7 ����������)
#define OLED_CS_Set()   (GPIOB->BSRR = GPIO_Pin_7)  // ��λ CS (�� GPIOB �� 7 ����������)



#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����


//	oled��ʼ������
void OLED_Init(void);
//	oledȫ��ˢ�º���
void OLED_Update(void);
//	oled�ֲ�ˢ�º���
void OLED_UpdateArea(uint8_t X, uint8_t Y, uint8_t Width, uint8_t Height);
// ������ɫģʽ
void OLED_SetColorMode(bool colormode);
// OLED �������Ⱥ���
void OLED_Brightness(int16_t Brightness);





#endif







