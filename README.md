# Doodle Jump - DE10-Standard FPGA Implementation

A simplified embedded implementation of the classic Doodle Jump game on the Intel DE10-Standard FPGA board, showcasing real-time hardware-software integration and embedded systems development.

## Table of Contents
- [Overview](#overview)
- [Features](#features)
- [Hardware Requirements](#hardware-requirements)
- [Software Requirements](#software-requirements)
- [System Architecture](#system-architecture)
- [FPGA Hardware Design](#fpga-hardware-design)
- [Game Mechanics](#game-mechanics)
- [Controls](#controls)
- [Project Structure](#project-structure)
- [Technical Highlights](#technical-highlights)

## Overview

This project implements a fully functional Doodle Jump game on the DE10-Standard FPGA board using bare-metal C programming. The player controls a character that continuously jumps upward, navigating randomly generated platforms while avoiding falling off the screen. The game demonstrates practical embedded systems concepts including memory-mapped I/O, real-time input processing, and efficient graphics rendering on resource-constrained hardware.

## Features

- **Real-time Gameplay**: Smooth 40 FPS game loop with gravity physics and collision detection
- **Dynamic Platform Generation**: Procedurally generated platforms for endless gameplay
- **Score Tracking**: Live score display on 7-segment displays
- **Pause Functionality**: Pause game using hardware switches
- **Hardware Integration**: Direct memory-mapped I/O for all peripherals
- **Game State Management**: Complete game initialization, play, pause, and game-over states
- **Custom FPGA Circuits**: VHDL and Verilog modules for hardware acceleration

## Hardware Requirements

- **Intel DE10-Standard FPGA Development Board**
  - Altera Cyclone V SoC (5CSXFC6D6F31C6)
  - ARM Cortex-A9 dual-core HPS
- **Peripherals Used:**
  - 128x64 pixel LCD display (game rendering)
  - 6-digit 7-segment display (score display)
  - 4 push buttons (player input)
  - 10 slide switches (pause control)
  - 10 LEDs (pause state indicator)

## Software Requirements

- **Intel SoC EDS (Embedded Development Suite)**
- **ARM GNU Toolchain** (`arm-linux-gnueabihf-gcc`)
- **Quartus Prime Lite Edition 21.1** (for FPGA configuration)
- **FileZilla** or similar SFTP client (for file transfer)
- **PuTTY** or similar SSH client (for terminal access)

## System Architecture

<img width="1833" height="1284" alt="SWE-450 Class Diagram" src="https://github.com/user-attachments/assets/4c3459fa-effd-4289-9dda-70fc7eece92f" />

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

<img width="3632" height="3168" alt="System Design" src="https://github.com/user-attachments/assets/5585f50c-4df4-4fa2-ac50-87fa0c299422" />

## FPGA Hardware Design

The project includes custom FPGA circuits implemented in VHDL and Verilog to interface with the board's peripherals.

### BCD to 7-Segment Decoder

<img width="1300" height="746" alt="digital-circuit-1" src="https://github.com/user-attachments/assets/2c632135-92e5-4ba1-b4dd-5c83fe378b3c" />

A custom VHDL module converts 4-bit Binary-Coded Decimal (BCD) input to 7-segment display representation. This decoder drives six 7-segment displays simultaneously, enabling real-time score updates with minimal CPU overhead.

**Key Features:**
- Active-low output for 7-segment common anode displays
- Supports digits 0-9 with invalid input handling
- Direct GPIO mapping for efficient hardware control
- Located in: `FPGA/bcd_7segment.vhd`

**Implementation:**
```vhdl
-- Converts 4-bit BCD input to 7-segment display pattern
-- Segments: {a, b, c, d, e, f, g}
-- Active-low output (0 = segment ON)
```

### LED Pause Indicator

<img width="928" height="370" alt="digital-circuit-2" src="https://github.com/user-attachments/assets/781437ec-0fea-4df8-8f68-6f8e103d09b3" />

A Verilog multiplexer circuit provides visual feedback for the game's pause state. When SW0 is engaged, all 10 LEDs illuminate; otherwise, they reflect normal system LED states.

**Implementation Details:**
- 2-to-1 multiplexer controlled by SW0 (pause switch)
- Overrides system LED signals during pause
- Provides clear visual indication of game state
- Located in: `FPGA/led_pause_override.v`

**Logic:**
```verilog
assign LEDR = switch ? 10'b1111111111 : system_leds;
```

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

### Game States
1. **Initialization**: Reset player position, score, and generate platforms
2. **Playing**: Active gameplay with physics and input processing
3. **Paused**: Game frozen, all LEDs illuminated
4. **Game Over**: Display final score with replay/exit options

## Controls

| Input | Function |
|-------|----------|
| **KEY3** (Left button) | Move player left |
| **KEY2** (Right button) | Move player right |
| **KEY1** (Replay button) | Restart game after game over |
| **KEY0** (Exit button) | Exit game |
| **SW0** (Switch) | Pause/Resume game (all LEDs illuminate when paused) |

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
├── address_map_arm.h       # Memory-mapped I/O addresses
├── LCD/                    # LCD driver library
│   ├── LCD_Driver.c/h      # LCD hardware driver
│   ├── LCD_Hw.c/h          # Low-level LCD control
│   ├── LCD_Lib.c/h         # LCD initialization routines
│   ├── lcd_graphic.c/h     # Graphics primitives (lines, circles, rectangles)
│   ├── font.c/h            # 8x16 font rendering engine
│   └── terasic_lib.c/h     # Timing and utility functions
├── FPGA/                   # FPGA configuration files
│   ├── DE10_Standard_Computer.v      # Top-level system integration (Verilog)
│   ├── DE10_Standard_Computer.sdc    # Timing constraints (50MHz clock)
│   ├── bcd_7segment.vhd              # BCD to 7-segment decoder (VHDL)
│   └── led_pause_override.v          # Pause LED multiplexer (Verilog)
├── Makefile                # Cross-compilation configuration
└── README.md               # This file
```

## Technical Highlights

### Embedded Systems Concepts Demonstrated

1. **Memory-Mapped I/O**: Direct hardware register access for all peripherals
   - GPIO manipulation for buttons, switches, and LEDs
   - SPI communication for LCD control
   - 7-segment display via GPIO bit-banging

2. **Real-Time Processing**: Deterministic game loop with precise timing control
   - 25ms frame time (40 FPS)
   - Non-blocking input polling
   - Consistent physics simulation

3. **Hardware Abstraction**: Clean separation between hardware drivers and application logic
   - Modular driver architecture
   - Platform-independent game logic
   - Reusable hardware interface APIs

4. **Resource Management**: Efficient memory usage and proper cleanup routines
   - Static memory allocation (no dynamic allocation in game loop)
   - Proper munmap() and file descriptor cleanup
   - Frame buffer management

5. **State Machine Design**: Robust game state management
   - Init → Play → Pause → Game Over → Replay/Exit
   - Event-driven state transitions
   - Clean state entry/exit handling

### FPGA Design Skills

- **VHDL Programming**: BCD-to-7-segment decoder with combinational logic
  - Case statement-based lookup table
  - Active-low output signaling
  - Invalid input handling

- **Verilog HDL**: Multiplexer design for LED control
  - Conditional assignment operators
  - Combinational logic synthesis
  - Signal routing and fan-out

- **Hardware Description**: Synthesizable RTL code for Cyclone V FPGA
  - Resource-efficient designs
  - Timing-aware implementation
  - Platform-specific optimizations

- **Timing Constraints**: SDC file configuration for reliable operation
  - Clock domain definitions (50MHz system clock)
  - Input/output delay constraints
  - False path specifications

### Performance Optimizations

- **Frame Buffering**: Flicker-free LCD updates with double-buffering
- **Efficient Collision Detection**: Bounding box algorithm with early exit
- **Minimal Dynamic Allocation**: Static buffers for all game objects
- **Hardware-Accelerated Display**: GPIO-based 7-segment updates (no CPU rendering)
- **Optimized Rendering**: Draw only changed regions when possible

### Code Quality

- **Modular Design**: Clear interface boundaries with header files
- **Error Handling**: Comprehensive initialization checks and failure recovery
- **Documentation**: Detailed inline comments explaining algorithms
- **Coding Standards**: Consistent naming conventions and formatting
- **Maintainability**: Separated concerns enable easy feature additions

### Skills Demonstrated

- **Low-Level Programming**: Direct memory access, bitwise operations, pointer manipulation
- **Embedded C**: Cross-compilation, static linking, bare-metal programming
- **Hardware Interfacing**: SPI, GPIO, memory-mapped registers
- **Digital Logic Design**: VHDL and Verilog HDL for custom FPGA circuits
- **Real-Time Systems**: Deterministic timing, interrupt-free polling architecture
- **Graphics Programming**: Pixel-level rendering, drawing primitives
- **Game Development**: Physics simulation, collision detection, procedural generation
- **Debugging**: Hardware debugging without traditional debuggers
- **Version Control**: Git-based development workflow
- **Documentation**: Technical writing and system architecture documentation

## Acknowledgments

This project was developed as part of **SWE-450 (Embedded Systems II)** coursework. Platform generation and game loop concepts inspired by my previous [Python Doodle Jump implementation](https://github.com/heyimdima/fox-jump-game).

Special thanks to:
- Terasic for LCD driver library components
- Intel/Altera for SoC EDS and development tools
- The embedded systems community for FPGA development resources

## License

This project is developed for educational purposes.

---

**Author**: Dima Bondar  
**Course**: SWE-450 - Embedded Systems II  
**Platform**: Intel DE10-Standard FPGA Board
