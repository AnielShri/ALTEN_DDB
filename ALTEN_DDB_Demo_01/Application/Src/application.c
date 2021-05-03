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


//---------------------------------------------------------------------------//
//	public functions
//---------------------------------------------------------------------------//

void App_Initialize()
{
	// clear all flags
	for (uint8_t n_flag = 0; n_flag < APP_FLAGS_MAX; n_flag++)
	{
		app_flags[n_flag] = 0;
	}

	// initialize app_data to zero
	app_data.up_counter = 0;

	// init peripherals
	HAL_TIM_Base_Start_IT(&htim6);
	HAL_GPIO_TogglePin(LD_HB_GPIO_Port, LD_HB_Pin);
}


void App_Loop()
{
	if (app_flags[FLAG_SECONDS_TIMER] == 1)
	{
		HAL_GPIO_TogglePin(LD_HB_GPIO_Port, LD_HB_Pin);


		app_flags[FLAG_SECONDS_TIMER] = 0;
	}
}
