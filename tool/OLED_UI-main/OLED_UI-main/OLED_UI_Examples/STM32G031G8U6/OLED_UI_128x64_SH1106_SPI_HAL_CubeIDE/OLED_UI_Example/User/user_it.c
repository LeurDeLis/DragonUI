#include <misc.h>
#include "main.h"
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_gpio.h"
#include "oled_ui.h"
#include "usermain.h"

// 定时器16中断回调函数

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == &htim16)
	{
		//中断函数
		OLED_UI_InterruptHandler();

	}
}
