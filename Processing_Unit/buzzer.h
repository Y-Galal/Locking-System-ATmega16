
#ifndef _BUZZER_H
#define _BUZZER_H

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

void buzzerInit(void);
void buzzerToggle();
void buzzerStop();

#endif