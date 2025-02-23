// SWE-450 Milestone 3 (Doodle Jump Game)
// Dima Bondar
// 02/23/2025

#ifndef SWITCHES_H
#define SWITCHES_H

#include <stdbool.h>

// Initialize switches
void initializeSwitches(volatile int *switch_ptr);

// Get switch state
int getSwitchState();

#endif