/*
 * application.c
 *
 *  Created on: May 3, 2021
 *      Author: Aniel
 */

#include "application.h"

//---------------------------------------------------------------------------//
//	variables
//---------------------------------------------------------------------------//
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim3;

// alias
#define htim_ui		htim6
#define htim_pwm	htim3


//---------------------------------------------------------------------------//
//	public functions
//---------------------------------------------------------------------------//

void App_Initialize()
{
	// clear all flags
	app_flags.start = 0;
	app_flags.stop = 0;
	app_flags.ui_timer = 0;

	// initialize app_data to zero
	app_data.up_counter = 10;

	// init peripherals
	HAL_TIM_Base_Start_IT(&htim_ui);
	HAL_GPIO_TogglePin(LD_HB_GPIO_Port, LD_HB_Pin);

	HAL_TIM_PWM_Start(&htim_pwm, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim_pwm, TIM_CHANNEL_1, 0);
}


void App_Loop()
{
	if (app_flags.start == 1)
	{
		__HAL_TIM_SET_COMPARE(&htim_pwm, TIM_CHANNEL_1, PWM_DUTY_MAX/3);

		app_flags.start = 0;
	}
	else if (app_flags.stop == 1)
	{
		__HAL_TIM_SET_COMPARE(&htim_pwm, TIM_CHANNEL_1, 0);

		app_flags.stop = 0;
	}
	else if (app_flags.ui_timer == 1)
	{
		HAL_GPIO_TogglePin(LD_HB_GPIO_Port, LD_HB_Pin);

		app_data.up_counter += 1;
		if (app_data.up_counter >= 1000) app_data.up_counter = 0;

		app_flags.ui_timer = 0;
	}
}
