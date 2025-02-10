// SWE-450 Milestone 2 (Doodle Jump Game)
// Dima Bondar
// 02/09/2025

#include "game.h"
#include "push_buttons.h"
#include "lcd.h"
#include "seven_seg.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>

// Game constants
#define SQUARE_SIZE 8    // Size of the square
#define STEP_SIZE 3      // Pixels per move
#define SCREEN_WIDTH 128 // LCD width
#define SCREEN_HEIGHT 64 // LCD height

// Button masks
#define KEY3_MASK 0x8  // Button 3
#define KEY2_MASK 0x4  // Button 2
#define KEY0_MASK 0x1  // Button 0 (EXIT)

// Set initial x and y position of the square
int x_position = SCREEN_WIDTH / 2 - SQUARE_SIZE / 2; // Start in the middle
int y_position = SCREEN_HEIGHT / 2 - SQUARE_SIZE / 2; // Center of the screen

void initializeGame(void) {
    printf("-- initializing game...\n");
    // Clear the screen
    lcd_clear();

    // Initialize score (12345 for the testing purposes)
    displayNumber(12345); // Show 12345 on the seven-segment display
    printf("-- finished initializing game...\n");
}

void playGame(void) {
    while (1) {
    // Check if the exit button (KEY0) is pressed
    if (isButtonPressed(KEY0_MASK)) {
        // Wait until it is released before exiting
        while(isButtonPressed(KEY0_MASK));

        // Notify the user that we are exiting a game on the LCD
        lcd_clear();
        lcd_write_centered("exiting...", 20);

        // 2-second delay (To be able to read the LCD message)
        usleep(2000000);
        
        // Exit the game loop
        break;
    }

    // Check for left movement (KEY3)
    if (isButtonPressed(KEY3_MASK)) {
        // Move the square to the left
        x_position -= STEP_SIZE;
        // Ensure the square does not go off the screen to the left
        if (x_position <= 0) x_position = 0;
    }

    // Check for right movement (KEY2)
    if (isButtonPressed(KEY2_MASK)) {
        // Move the square to the right
        x_position += STEP_SIZE;
        // Ensure the square does not go off the screen to the right
        if (x_position >= SCREEN_WIDTH - SQUARE_SIZE)
            x_position = SCREEN_WIDTH - SQUARE_SIZE;
    }

    // Redraw the square at the new position
    lcd_clear();
    lcd_draw_rectangle(x_position, y_position,
                       x_position + SQUARE_SIZE,
                       y_position + SQUARE_SIZE);

    // Refresh the LCD
    lcd_refresh();

    // Delay to control the speed of the game and CPU (25ms)
    usleep(25000);
    }
}