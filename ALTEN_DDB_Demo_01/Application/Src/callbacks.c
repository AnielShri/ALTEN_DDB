/*
 * callbacks.c
 *
 *  Created on: May 3, 2021
 *      Author: Aniel
 */


#include "application.h"

//---------------------------------------------------------------------------//
//	timers
//---------------------------------------------------------------------------//

extern TIM_HandleTypeDef htim6;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    switch((uint32_t)htim->Instance)
    {
        case (uint32_t)TIM6:
        	app_flags[FLAG_SECONDS_TIMER] = 1;
            break;
    }
}
