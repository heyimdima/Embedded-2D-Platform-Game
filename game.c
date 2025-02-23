// SWE-450 Milestone 3 (Doodle Jump Game)
// Dima Bondar
// 02/23/2025
// Reference: A lot of the ideas of score tracking/game loop/platform generation come from my Python Doodle Jump clone I did 4 years ago.
// Link: https://github.com/heyimdima/FoxJumpGame

#include "game.h"
#include "push_buttons.h"
#include "lcd.h"
#include "seven_seg.h"
#include "switches.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

// -------------------
// Game Configuration
// -------------------

#define SQUARE_SIZE    8      // Size of the player square (in pixels)
#define STEP_SIZE      3      // Number of pixels the player moves horizontally per step
#define SCREEN_WIDTH   128    // Width of the LCD screen (in pixels)
#define SCREEN_HEIGHT  64     // Height of the LCD screen (in pixels)

#define GRAVITY        1      // Downward acceleration added each frame
#define JUMP_VELOCITY  -12    // Upward velocity given when the player jumps (negative for upward motion)
#define NUM_PLATFORMS  7      // Total number of platforms in the game

// --------------
// Button Masks
// --------------

#define KEY3_MASK      0x8    // Button for moving left
#define KEY2_MASK      0x4    // Button for moving right
#define KEY1_MASK      0x2    // Button for replaying after a game over
#define KEY0_MASK      0x1    // Button for exiting the game

// -------------------
// Platform Structure
// -------------------

typedef struct {
    int x;       // X coordinate (horizontal position) of the platform
    int y;       // Y coordinate (vertical position) of the platform
    int width;   // Width of the platform (in pixels)
} Platform;

// ----------------------
// Global Game Variables
// ----------------------

// Set the initial player position to be centered on the screen.
int x_position = SCREEN_WIDTH / 2 - SQUARE_SIZE / 2;
int y_position = SCREEN_HEIGHT / 2 - SQUARE_SIZE / 2;
int verticalVelocity = 0;   // Player's current vertical speed
int score = 0;              // Player's current score

// Array to hold all platforms rendered in the game.
Platform platforms[NUM_PLATFORMS];

void initializePlatforms(void) {
    int spacing = SCREEN_HEIGHT / NUM_PLATFORMS;  // Vertical gap between platforms
    for (int i = 0; i < NUM_PLATFORMS; i++) {
        platforms[i].width = 30; // Each platform is 30 pixels wide
        // Randomly set the horizontal position within screen bounds.
        platforms[i].x = rand() % (SCREEN_WIDTH - platforms[i].width);
        platforms[i].y = i * spacing; // Set the vertical position based on its index
    }
    // Place the first platform right under the player.
    platforms[0].width = 30;
    platforms[0].x = x_position - (platforms[0].width - SQUARE_SIZE) / 2;
    if (platforms[0].x < 0)
        platforms[0].x = 0;  // Ensure the platform doesn't go off the left edge.
    platforms[0].y = y_position + SQUARE_SIZE + 1;
}

void initializeGame(void) {
    printf("-- initializing game...\n");
    lcd_clear();  // Clear the LCD display

    // Reset player state (position, velocity, and score)
    x_position = SCREEN_WIDTH / 2 - SQUARE_SIZE / 2;
    y_position = SCREEN_HEIGHT / 2 - SQUARE_SIZE / 2;
    verticalVelocity = 0;
    score = 0;
    
    // Update the seven-segment display with the current score.
    displayNumber(score);
    
    // Initialize platforms for the new game.
    initializePlatforms();
    
    printf("-- finished initializing game...\n");
}

/*
 * The main game loop:
 *  - Processes player input for moving left/right and handling pause/exit.
 *  - Applies gravity and updates the player's vertical movement.
 *  - Checks for collisions between the player and platforms.
 *  - Moves platforms down when the player moves above mid-screen to simulate scrolling.
 *  - Renders the player and platforms on the LCD screen.
 *  - Handles game-over condition.
 */
void playGame(void) {
    while (1) {
        // ----------------------
        // Handle Exit and Pause
        // ----------------------

        // Check if the exit button is pressed.
        if (isButtonPressed(KEY0_MASK)) {
            while(isButtonPressed(KEY0_MASK));  // Wait for button release
            lcd_clear();
            lcd_write_centered("Exiting...", 20);
            usleep(2000000);  // Pause for 2 seconds
            break;  // Exit the game loop
        }
        
        // Check if the game is paused (switch SW0 is engaged).
        if (getSwitchState() & 0x1) {
            lcd_clear();
            lcd_write_centered("PAUSED", 20);
            // Wait until the pause switch is turned off.
            while (getSwitchState() & 0x1) {
                usleep(100000);  // Poll every 100ms
            }
            lcd_clear();  // Clear the pause message
        }

        // ----------------------------
        // Handle Horizontal Movement
        // ----------------------------

        // Move left if KEY3 is pressed.
        if (isButtonPressed(KEY3_MASK)) {
            x_position -= STEP_SIZE;
            if (x_position < 0)
                x_position = 0;  // Prevent moving off-screen to the left
        }
        // Move right if KEY2 is pressed.
        if (isButtonPressed(KEY2_MASK)) {
            x_position += STEP_SIZE;
            if (x_position > SCREEN_WIDTH - SQUARE_SIZE)
                x_position = SCREEN_WIDTH - SQUARE_SIZE;  // Prevent moving off-screen to the right
        }
        
        // --------------------------------------------
        // Apply Gravity and Update Vertical Position
        // --------------------------------------------

        verticalVelocity += GRAVITY;        // Increase downward speed due to gravity
        y_position += verticalVelocity;  // Update player's vertical position
        
        // -----------------------------
        // Platform Collision Detection
        // -----------------------------

        // Only check for collisions when the player is falling (so the player does not collide with platforms on the way up).
        if (verticalVelocity > 0) {
            for (int i = 0; i < NUM_PLATFORMS; i++) {
                // Check if the bottom of the player is within 5 pixels of a platform's top.
                // Also check if the player's horizontal position overlaps with the platform.
                if ((y_position + SQUARE_SIZE >= platforms[i].y) &&
                    (y_position + SQUARE_SIZE <= platforms[i].y + 5) &&
                    (x_position + SQUARE_SIZE > platforms[i].x) &&
                    (x_position < platforms[i].x + platforms[i].width)) {
                    // Collision detected: make the player jump by setting the jump velocity.
                    verticalVelocity = JUMP_VELOCITY;
                    // Position the player so that they sit on top of the platform.
                    y_position = platforms[i].y - SQUARE_SIZE;
                }
            }
        }
         
        // ------------------------------------
        // Platform Scrolling and Score Update
        // ------------------------------------

        // When the player moves above the middle of the screen, shift all platforms downward to simulate upward movement.
        if (y_position < SCREEN_HEIGHT / 2) {
            int delta = SCREEN_HEIGHT / 2 - y_position;
            y_position = SCREEN_HEIGHT / 2;  // Keep the player at mid-screen
            for (int i = 0; i < NUM_PLATFORMS; i++) {
                platforms[i].y += delta;
                // If a platform goes off the bottom of the screen, reposition it at the top.
                if (platforms[i].y > SCREEN_HEIGHT) {
                    platforms[i].y = 0;  // Reset platform to the top
                    platforms[i].x = rand() % (SCREEN_WIDTH - platforms[i].width);  // Randomize new horizontal position
                    score += 10;  // Increase the score for successfully moving past a platform
                    displayNumber(score);  // Update the score display on the seven-seg screen
                }
            }
        }
        
        // ----------------------
        // Game Over Condition
        // ----------------------

        // If the player falls below the bottom of the screen, the game is over.
        if (y_position > SCREEN_HEIGHT) {
            lcd_clear();
            lcd_write_centered("Game Over", 20);
            lcd_write_centered("KEY1: Replay", 30);
            lcd_write_centered("KEY0: Exit", 40);
            
            // Wait for the player to replay or exit the game.
            while (1) {
                if (isButtonPressed(KEY0_MASK)) {  // EXIT
                    while (isButtonPressed(KEY0_MASK));
                    lcd_clear();
                    lcd_write_centered("Exiting...", 20);
                    usleep(2000000);
                    return;  // Exit the playGame function
                }
                if (isButtonPressed(KEY1_MASK)) {  // PLAY AGAIN
                    while (isButtonPressed(KEY1_MASK));
                    initializeGame();  // Initialize a new game
                    break;  // Resume the game
                }
                usleep(100000); // Check for button presses every 100ms
            }
        }
        
        // ---------------------------
        // Rendering the Game Screen
        // ---------------------------

        lcd_clear();  // Clear previous frame drawings
        
        // Draw the player as a square at the current position.
        lcd_draw_rectangle(x_position, y_position,
                           x_position + SQUARE_SIZE,
                           y_position + SQUARE_SIZE);
        
        // Draw platforms with a set height of 3 pixels.
        for (int i = 0; i < NUM_PLATFORMS; i++) {
            lcd_draw_rectangle(platforms[i].x, platforms[i].y,
                               platforms[i].x + platforms[i].width,
                               platforms[i].y + 3);
        }
        
        // Update the LCD.
        lcd_refresh();
        
        // Delay to control the game speed (25ms per frame).
        usleep(25000);
    }
}
