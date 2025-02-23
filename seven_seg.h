// SWE-450 Milestone 3 (Doodle Jump Game)
// Dima Bondar
// 02/23/2025

#ifndef SEVEN_SEG_H
#define SEVEN_SEG_H

// Initialize the seven-segment display hardware
void initializeSevenSeg(volatile int *display_ptr);

// Show a number on the seven-segment displays
void displayNumber(int number);

// Clear all display segments
void clearSevenSeg();

#endif