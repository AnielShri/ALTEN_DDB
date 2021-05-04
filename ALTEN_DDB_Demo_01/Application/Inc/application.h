/*
 * application.h
 *
 *  Created on: May 3, 2021
 *      Author: Aniel
 */

#ifndef APPLICATION_INC_APPLICATION_H_
#define APPLICATION_INC_APPLICATION_H_

#include "main.h"

//---------------------------------------------------------------------------//
//	constants
//---------------------------------------------------------------------------//

#define PWM_DUTY_MAX		2500
#define PWM_DUTY_NOMINAL	(PWM_DUTY_MAX / 120 * 100)

//---------------------------------------------------------------------------//
//	types
//---------------------------------------------------------------------------//

typedef enum
{
	CONFIG_ENCODER_HZ,
	CONFIG_ENCODER_FAST,
	CONFIG_ENCODER_FILTER
}AppConfigEncoderEnum;

typedef struct
{
	uint8_t ui_timer;
	uint8_t	encoder_isr;
	uint8_t start;
	uint8_t	stop;
}AppFlagsStruct;

typedef struct
{
	uint32_t true_speed;
	uint32_t set_speed;
	uint32_t tick_count;
}AppMonitorStruct;

typedef struct
{
	uint8_t		speed;
	uint8_t		encoder;
	uint8_t		controller;
}AppConfigStruct;


//---------------------------------------------------------------------------//
//	globals
//---------------------------------------------------------------------------//

//uint8_t 			app_flags[APP_FLAGS_MAX];
AppFlagsStruct		app_flags;
AppConfigStruct		app_config;
AppMonitorStruct	app_data;


//---------------------------------------------------------------------------//
//	functions
//---------------------------------------------------------------------------//

void App_Initialize();
void App_Loop();


#endif /* APPLICATION_INC_APPLICATION_H_ */
