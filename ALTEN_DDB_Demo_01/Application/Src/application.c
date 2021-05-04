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
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim3;

// alias
#define htim_ui		htim6
#define htim_ticks	htim7
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
	app_flags.encoder_isr = 0;

	// initialize app_data to zero
	app_data.true_speed = 0;
	app_data.set_speed = 0;
	app_data.tick_count = 0;

	// set default config
	app_config.encoder = CONFIG_ENCODER_FAST;

	// init peripherals
	HAL_TIM_Base_Start_IT(&htim_ui);
	HAL_GPIO_TogglePin(LD_HB_GPIO_Port, LD_HB_Pin);

	HAL_TIM_PWM_Start(&htim_pwm, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim_pwm, TIM_CHANNEL_1, 0);
}


void App_Loop()
{
//	__HAL_TIM_SET_AUTORELOAD()

	if (app_flags.encoder_isr == 1)
	{
		uint32_t delta_ticks = app_data.tick_count;
		app_data.tick_count = 0;

		uint32_t speed = 0;
		if (delta_ticks >= 5) speed = 15.75e3 / delta_ticks;

		switch (app_config.encoder) {
			case CONFIG_ENCODER_FAST:
			{
				app_data.true_speed = speed;
				break;
			}
			case CONFIG_ENCODER_FILTER:
			{
				uint32_t old_speed = app_data.true_speed;
				const float k = 0.35;
				uint32_t ema_speed = (k * speed) + ((1 - k) * old_speed);
				app_data.true_speed = ema_speed;
				break;
			}
		}

		app_flags.encoder_isr = 0;
	}

	if (app_flags.start == 1)
	{
		app_data.tick_count = 0;
		app_data.true_speed = 1;
		HAL_TIM_Base_Start_IT(&htim_ticks);

		__HAL_TIM_SET_COMPARE(&htim_pwm, TIM_CHANNEL_1, PWM_DUTY_NOMINAL);

		app_flags.start = 0;
	}
	else if (app_flags.stop == 1)
	{
		__HAL_TIM_SET_COMPARE(&htim_pwm, TIM_CHANNEL_1, 0);

		HAL_TIM_Base_Stop_IT(&htim_ticks);

		app_flags.stop = 0;
	}
	else if (app_flags.ui_timer == 1)
	{
		HAL_GPIO_TogglePin(LD_HB_GPIO_Port, LD_HB_Pin);

		app_flags.ui_timer = 0;
	}
}
