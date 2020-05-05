

#ifndef _MOTOR_H_
#define _MOTOR_H_
#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

#define START 11
#define REVERSE 12
#define STOP 13

void motor(uint8 condition);
void motorInit();

#endif