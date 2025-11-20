# Doodle Jump - DE10-Standard FPGA Implementation

A simplified embedded implementation of the classic Doodle Jump game on the Intel DE10-Standard FPGA board, showcasing real-time hardware-software integration and embedded systems development.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [System Architecture](#system-architecture)
- [Game Mechanics](#game-mechanics)
- [Building and Running](#building-and-running)
- [Controls](#controls)
- [Project Structure](#project-structure)
- [Technical Highlights](#technical-highlights)

## Overview

This project implements a fully functional Doodle Jump game on the DE10-Standard FPGA board using bare-metal C programming. The player controls a character that continuously jumps upward, navigating randomly generated platforms while avoiding falling off the screen. The game demonstrates practical embedded systems concepts including memory-mapped I/O, real-time input processing, and efficient graphics rendering on resource-constrained hardware.

## Features

- **Real-time Gameplay**: Smooth game loop with gravity physics and collision detection
- **Dynamic Platform Generation**: Procedurally generated platforms for endless gameplay
- **Score Tracking**: Live score display on 7-segment displays
- **Pause Functionality**: Pause game using hardware switches
- **Hardware Integration**: Direct memory-mapped I/O for all peripherals
- **Game State Management**: Complete game initialization, play, pause, and game-over states

## Hardware Requirements

- **Intel DE10-Standard FPGA Development Board**
  - Altera Cyclone V SoC (5CSXFC6D6F31C6)
  - ARM Cortex-A9 dual-core HPS
- **Peripherals Used:**
  - 128x64 pixel LCD display (game rendering)
  - 6-digit 7-segment display (score display)
  - 4 push buttons (player input)
  - 10 slide switches (pause control)

## Software Requirements

- **Intel SoC EDS (Embedded Development Suite)**
- **ARM GNU Toolchain** (`arm-linux-gnueabihf-gcc`)
- **Quartus Prime Lite Edition 21.1** (for FPGA configuration)
- **FileZilla** or similar SFTP client (for file transfer)
- **PuTTY** or similar SSH client (for terminal access)

## System Architecture

<img width="1833" height="1284" alt="SWE-450 Class Diagram" src="https://github.com/user-attachments/assets/90a76081-3885-4924-bacc-4fee0d695e54" />

The project follows a modular architecture with clear separation of concerns:

### Hardware Abstraction Layer
- **hardware.c/h**: Central hardware initialization and cleanup
- **lcd.c/h**: LCD display driver wrapper
- **seven_seg.c/h**: 7-segment display controller
- **push_buttons.c/h**: Button input handler
- **switches.c/h**: Switch state reader

### Game Logic Layer
- **game.c/h**: Core game mechanics (physics, collision detection, platform generation)
- **app.c/h**: Main application entry point and game loop orchestration

### Supporting Libraries
- **LCD Driver Stack**: Low-level LCD hardware control (LCD_Hw, LCD_Driver, LCD_Lib)
- **Graphics Library**: Drawing primitives (lcd_graphic, font)

<img width="3632" height="3168" alt="System Design" src="https://github.com/user-attachments/assets/22ac17b3-6a5b-451d-86ea-42e3521b8e49" />

## Game Mechanics

### Physics System
- **Gravity**: Constant downward acceleration (1 pixel/frame²)
- **Jump Velocity**: -12 pixels/frame when landing on platforms
- **Movement**: 3 pixels/frame horizontal movement

### Platform System
- **Generation**: 7 platforms with random horizontal positions
- **Scrolling**: Platforms scroll down when player rises above mid-screen
- **Scoring**: +10 points for each platform successfully passed

### Collision Detection
- Bottom-edge collision detection between player and platforms
- 5-pixel collision tolerance for smooth gameplay
- Only active when player is falling (prevents mid-air collision)

## Controls

| Input | Function |
|-------|----------|
| **KEY3** (Left button) | Move player left |
| **KEY2** (Right button) | Move player right |
| **KEY1** (Replay button) | Restart game after game over |
| **KEY0** (Exit button) | Exit game |
| **SW0** (Switch) | Pause/Resume game |

## Project Structure

```
.
├── app.c/h                 # Main application entry point
├── game.c/h                # Core game logic and mechanics
├── hardware.c/h            # Hardware initialization and management
├── lcd.c/h                 # LCD display interface
├── seven_seg.c/h           # 7-segment display controller
├── push_buttons.c/h        # Button input handling
├── switches.c/h            # Switch state management
├── LCD/                    # LCD driver library
│   ├── LCD_Driver.c/h      # LCD hardware driver
│   ├── LCD_Hw.c/h          # Low-level LCD control
│   ├── LCD_Lib.c/h         # LCD initialization routines
│   ├── lcd_graphic.c/h     # Graphics primitives
│   ├── font.c/h            # Font rendering
│   └── terasic_lib.c/h     # Utility functions
├── FPGA/                   # FPGA configuration files
│   ├── DE10_Standard_Computer.v
│   ├── bcd_7segment.vhd
│   └── led_pause_override.v
├── Makefile                # Build configuration
└── address_map_arm.h       # Memory-mapped I/O addresses
```

## Technical Highlights

### Embedded Systems Concepts Demonstrated

1. **Memory-Mapped I/O**: Direct hardware register access for all peripherals
2. **Real-Time Processing**: Deterministic game loop with precise timing control
3. **Hardware Abstraction**: Clean separation between hardware drivers and application logic
4. **Resource Management**: Efficient memory usage and proper cleanup routines
5. **State Machine Design**: Robust game state management (init, play, pause, game over)

### Performance Optimizations

- Frame buffering for flicker-free LCD updates
- Efficient collision detection using bounding boxes
- Minimal dynamic memory allocation
- Hardware-accelerated 7-segment display updates via GPIO

### Code Quality

- Modular design with clear interface boundaries
- Comprehensive error handling and hardware initialization checks
- Well-documented code with detailed comments
- Consistent coding style and naming conventions

## Acknowledgments

This project was developed as part of **SWE-450 (Embedded Systems II)** coursework. Platform generation and game loop concepts inspired by my previous [Python Doodle Jump implementation](https://github.com/heyimdima/FoxJumpGame).

## License

This project is developed for educational purposes.

---

**Author**: Dima Bondar  
**Course**: SWE-450 - Embedded Systems II  
**Platform**: Intel DE10-Standard FPGA Board
