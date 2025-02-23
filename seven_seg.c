// SWE-450 Milestone 3 (Doodle Jump Game)
// Dima Bondar
// 02/23/2025

#include "seven_seg.h"

// Pointer to the seven-segment GPIO
static volatile int *JP1_ptr;

// Initialize the seven-segment display
void initializeSevenSeg(volatile int *display_ptr) {
    JP1_ptr = display_ptr;
    *(JP1_ptr + 1) = 0x00FFFFFF;
}

// Display a number on the 7-segment displays
void displayNumber(int number) {
    unsigned int gpio_value = 0; // Combined GPIO value for the 7-segment displays
    int digit;
    int i;

    // Extract each digit of the number and map it to the appropriate GPIO bits
    for (i = 0; i < 6; i++) {
        digit = number % 10;               // Extract the least significant digit
        gpio_value |= (digit << (i * 4));  // Shift the digit to the correct GPIO group
        number /= 10;                      // Remove the least significant digit
    }

    // Write the combined value to the GPIO data register
    *JP1_ptr = gpio_value;
}

// Clear seven-segment display
void clearSevenSeg() {
    *JP1_ptr = 0x00FFFFFF; // Clear the seven-segment display
} 