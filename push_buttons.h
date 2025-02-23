// SWE-450 Milestone 3 (Doodle Jump Game)
// Dima Bondar
// 02/23/2025

#ifndef PUSH_BUTTONS_H
#define PUSH_BUTTONS_H

#include <stdbool.h>

// Push button memory addresses
void initializePushButtons(volatile int *button_ptr);

// Function to check button press
int getPressedButton();

#endif