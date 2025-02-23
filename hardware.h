// SWE-450 Milestone 3 (Doodle Jump Game)
// Dima Bondar
// 02/23/2025

#ifndef HARDWARE_H
#define HARDWARE_H

#include "seven_seg.h"
#include "push_buttons.h"
#include "switches.h"
#include <stdbool.h>

// Initialize hardware
bool initializeHardware(void);

// Cleanup hardware
void cleanupHardware(void);

#endif
