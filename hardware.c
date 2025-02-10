// SWE-450 Milestone 2 (Doodle Jump Game)
// Dima Bondar
// 02/09/2025

#include "hardware.h"
#include "address_map_arm.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/mman.h>

// Global variables for memory-mapped I/O
static volatile int *KEY_ptr;     // KEY address
static volatile int *JP1_ptr;     // JP1 address
static volatile int *SW_ptr;      // SW address

// Function to initialize hardware
bool initializeHardware() {
    printf("--> initializing hardware..\n");

    int fd = open("/dev/mem", O_RDWR);
    if (fd == -1) {
        printf("Error opening /dev/mem\n");
        return false;
    }

    // Get virtual base address for FPGA Devices
    void *virtual_base = mmap(NULL, LW_BRIDGE_SPAN, PROT_READ | PROT_WRITE, 
                            MAP_SHARED, fd, LW_BRIDGE_BASE);
    if (virtual_base == MAP_FAILED) {
        printf("Error: mmap() failed...\n");
        close(fd);
        return false;
    }

    // Initialize pointers to I/O ports
    KEY_ptr = (int *)(virtual_base + KEY_BASE);
    JP1_ptr = (int *)(virtual_base + JP1_BASE);
    SW_ptr = (int *)(virtual_base + SW_BASE);

    // Initialize push buttons
    initializePushButtons(KEY_ptr);

    // Initialize seven-segment display
    initializeSevenSeg(JP1_ptr);

    // Initialize switches
    initializeSwitches(SW_ptr);

    // Initialize LCD
    if (!lcd_initialize()) {
        printf("Error: LCD initialization failed\n");
        munmap(virtual_base, LW_BRIDGE_SPAN);
        close(fd);
        return false;
    }

    // Clear displays
    clearSevenSeg();
    lcd_clear();

    printf("--> hardware initialized successfully.\n");
    
    return true;
}

// Function to cleanup hardware
void cleanupHardware() {
    printf("--> cleaning up hardware..\n");

    // Clear the seven segment display
    clearSevenSeg();
    
    // Turn off LCD backlight and clear display
    lcd_backlight(false);
    lcd_clear();
    
    // Cleanup LCD resources
    lcd_cleanup();
    
    printf("--> hardware cleanup completed.\n");
}
