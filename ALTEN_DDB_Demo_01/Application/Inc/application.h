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
//	types
//---------------------------------------------------------------------------//

typedef enum
{
	FLAG_SECONDS_TIMER,
	APP_FLAGS_MAX
}AppFlagsEnum;

typedef struct
{
	uint32_t	up_counter;
}AppMonitorStruct;


//---------------------------------------------------------------------------//
//	globals
//---------------------------------------------------------------------------//

uint8_t 			app_flags[APP_FLAGS_MAX];
AppMonitorStruct	app_data;


//---------------------------------------------------------------------------//
//	functions
//---------------------------------------------------------------------------//

void App_Initialize();
void App_Loop();


#endif /* APPLICATION_INC_APPLICATION_H_ */
