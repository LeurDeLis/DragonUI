/*
 * usermain.c
 *
 *  Created on: Nov 27, 2024
 *      Author: tonny
 */
#include "usermain.h"
#include "stm32g0xx_hal.h"
#include "main.h"
#include "OLED_UI.h"
#include "OLED_UI_MenuData.h"
#include "misc.h"

// 定时器句柄
extern TIM_HandleTypeDef htim16;

void usermain()
{
	// init
	//开启外围器件
	// 注意：开启外围器件可能导致电压波动，不要在此操作之后立即读写Flash
	HAL_GPIO_WritePin(PWRON_GPIO_Port, PWRON_Pin, 0);

	BTN_init();


	OLED_UI_Init(&MainMenuPage);

	while(1)
	{
		BtnTask();
		OLED_UI_MainLoop();
	}
}
