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
extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim16;

// alias
#define htim_pwm	htim3
#define htim_ui		htim6
#define htim_ticks	htim7
#define htim_pid	htim16


//---------------------------------------------------------------------------//
//	public functions
//---------------------------------------------------------------------------//

void App_Initialize()
{
	// clear all flags
	app_flags.start = 0;
	app_flags.stop = 0;
	app_flags.ui_isr = 0;
	app_flags.encoder_isr = 0;

	// initialize app_data to zero
	app_data.true_speed = 0;
	app_data.set_speed = 0;
	app_data.tick_count = 0;

	// set default config
	app_config.encoder = CONFIG_ENCODER_FAST;
	app_data.set_speed = 100;

	// init peripherals
	HAL_TIM_Base_Start_IT(&htim_ui);
	HAL_GPIO_TogglePin(LD_HB_GPIO_Port, LD_HB_Pin);

	HAL_TIM_PWM_Start(&htim_pwm, TIM_CHANNEL_1);
	__HAL_TIM_SET_COMPARE(&htim_pwm, TIM_CHANNEL_1, 0);
}


void App_Loop()
{
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

	if (app_flags.pid_isr == 1)
	{
		if (app_config.controller == CONFIG_CONTROLLER_OPENLOOP)
		{
			const float max_speed = 120;
			float open_loop = app_data.set_speed / max_speed;
			__HAL_TIM_SET_COMPARE(&htim_pwm, TIM_CHANNEL_1, PWM_DUTY_MAX * open_loop);
		}
		else
		{
			// do PID
			const float dt = 5e-3;
			const float Kp = 1;
			const float Ki = 2 * dt;
			const float Kd = 0.01 / dt;
			const float max_speed = 120;

			float error = (float)app_data.set_speed - (float)app_data.true_speed;

			app_data.sum_error += Ki * error;
			if (app_data.sum_error >= max_speed) app_data.sum_error = 100;
			else if (app_data.sum_error <= -1*max_speed) app_data.sum_error = -1*max_speed;

			float rate_error = (float)error - (float)app_data.prev_error;

			float duty_cycle = (Kp * error) + app_data.sum_error + (Kd * rate_error);

			if (app_config.controller == CONFIG_CONTROLLER_HIBRID)
			{
				duty_cycle += app_data.set_speed;
			}

			if (duty_cycle >= max_speed) duty_cycle = max_speed;
			if (duty_cycle <= 0) duty_cycle = 0;
			duty_cycle = duty_cycle / max_speed;


			__HAL_TIM_SET_COMPARE(&htim_pwm, TIM_CHANNEL_1, PWM_DUTY_MAX * duty_cycle);

			app_data.prev_error = error;
		}

		app_flags.pid_isr = 0;

	}

	if (app_flags.start == 1)
	{
		app_data.tick_count = 0;
		app_data.true_speed = 1;
		app_data.prev_error = 0;
		app_data.sum_error = 0;
		HAL_TIM_Base_Start_IT(&htim_ticks);

		app_config.enabled = 1;
//		__HAL_TIM_SET_COMPARE(&htim_pwm, TIM_CHANNEL_1, PWM_DUTY_NOMINAL);
		HAL_TIM_Base_Start_IT(&htim_pid);

		app_flags.start = 0;
	}
	else if (app_flags.stop == 1)
	{
		app_config.enabled = 0;
		__HAL_TIM_SET_COMPARE(&htim_pwm, TIM_CHANNEL_1, 0);

		HAL_TIM_Base_Stop_IT(&htim_ticks);
		HAL_TIM_Base_Stop_IT(&htim_pid);

		app_flags.stop = 0;
	}
	else if (app_flags.ui_isr == 1)
	{
		HAL_GPIO_TogglePin(LD_HB_GPIO_Port, LD_HB_Pin);

//		if (app_config.enabled == 1)
//		{
//			uint32_t duty_cycle = PWM_DUTY_NOMINAL * (app_data.set_speed / 100.0);
//			__HAL_TIM_SET_COMPARE(&htim_pwm, TIM_CHANNEL_1, duty_cycle);
//		}

		app_flags.ui_isr = 0;
	}
}
