// SWE-450 Milestone 2 (Doodle Jump Game)
// Dima Bondar
// 02/09/2025

// Import libraries
#include "hardware.h"
#include "lcd.h"
#include "push_buttons.h"
#include "game.h"
#include "seven_seg.h"
#include <unistd.h>
#include <stdio.h>

int main() {
    // Initialize hardware
    if (!initializeHardware()) {
        printf("Hardware initialization failed!\n");
        return 1;
    }

    // Initialize the game
    initializeGame();

    // Trigger the game loop
    playGame();

    // Cleanup hardware before exiting
    cleanupHardware();
    return 0;
}