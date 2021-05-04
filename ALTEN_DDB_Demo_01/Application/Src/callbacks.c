/*
 * callbacks.c
 *
 *  Created on: May 3, 2021
 *      Author: Aniel
 */


#include "application.h"

//---------------------------------------------------------------------------//
//	encoders
//---------------------------------------------------------------------------//
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_0)
	{
		app_flags.encoder_isr = 1;
	}
}

//---------------------------------------------------------------------------//
//	timers
//---------------------------------------------------------------------------//

#define MAX_TICK_COUNT	0xFFFFFFFF

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    switch((uint32_t)htim->Instance)
    {
        case (uint32_t)TIM6:
        {
        	app_flags.ui_isr = 1;
            break;
        }
        case (uint32_t)TIM7:
        {
        	app_data.tick_count += 1;
        	if (app_data.tick_count >= MAX_TICK_COUNT)
        	{
        		app_data.tick_count = 0;
        		app_flags.stop = 1;
        	}

        	break;
        }
        case (uint32_t)TIM16:
        {
        	app_flags.pid_isr = 1;
        	break;
        }
    }
}
