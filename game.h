// SWE-450 Milestone 3 (Doodle Jump Game)
// Dima Bondar
// 02/23/2025
// Reference: A lot of the ideas of score tracking/game loop/platform generation come from my Python Doodle Jump clone I did 4 years ago.
// Link: https://github.com/heyimdima/FoxJumpGame

#ifndef BLACKJACK_H
#define BLACKJACK_H

#include <stdbool.h>

// Initialize the game
void initializeGame(void);

// Initialize platforms
void initializePlatforms(void);

// Main game loop
void playGame(void);

#endif
