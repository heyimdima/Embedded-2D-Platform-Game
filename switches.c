// SWE-450 Milestone 2 (Doodle Jump Game)
// Dima Bondar
// 02/09/2025

#include "switches.h"
#include "address_map_arm.h"
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

// Pointer for switch state
static volatile int *SW_ptr;

// Initialize switches
void initializeSwitches(volatile int *switch_ptr) {
    SW_ptr = switch_ptr;
}

// Function to get switch state
int getSwitchState() {
    return *SW_ptr;
}