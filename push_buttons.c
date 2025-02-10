// SWE-450 Milestone 2 (Doodle Jump Game)
// Dima Bondar
// 02/09/2025

#include "push_buttons.h"
#include <stdio.h>
#include <unistd.h>

// Button hardware pointer
volatile int *KEY_ptr = NULL;

// Initialize push buttons
void initializePushButtons(volatile int *button_ptr) {
    KEY_ptr = button_ptr;
}

// Returns nonzero if the specified button is currently pressed.
int isButtonPressed(int button_mask) {
    return (*KEY_ptr & button_mask) != 0;
}